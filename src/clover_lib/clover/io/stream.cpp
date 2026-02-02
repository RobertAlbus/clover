// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include "clover/io/stream.hpp"

#include <atomic>
#include <cstring>
#include <format>
#include <iostream>
#include <semaphore>
#include <stdexcept>

#include <pipewire/extensions/metadata.h>
#include <pipewire/pipewire.h>
#include <spa/param/audio/format-utils.h>

namespace clover::io {

struct stream::impl {
    stream::config cfg;

    pw_thread_loop* thread_loop = nullptr;
    pw_context* context         = nullptr;
    pw_core* core               = nullptr;
    pw_registry* registry       = nullptr;
    pw_metadata* metadata       = nullptr;

    pw_stream* output_stream = nullptr;
    pw_stream* input_stream  = nullptr;

    spa_hook core_listener{};
    spa_hook registry_listener{};
    spa_hook metadata_listener{};
    spa_hook output_stream_listener{};
    spa_hook input_stream_listener{};

    std::atomic<bool> running{false};
    std::atomic<bool> completed{false};
    std::atomic<bool> reconnect_pending{false};
    std::binary_semaphore completion_signal{0};

    int64_t clock_time          = 0;
    int negotiated_rate         = 0;
    uint32_t metadata_id        = 0;
    float* current_input_buffer = nullptr;

    stream* parent = nullptr;

    bool has_input() const {
        return cfg.channels_in > 0;
    }
    bool has_output() const {
        return cfg.channels_out > 0;
    }

    impl(stream::config c, stream* p) : cfg(c), parent(p) {
        pw_init(nullptr, nullptr);
    }

    ~impl() {
        cleanup();
        pw_deinit();
    }

    void cleanup() {
        if (thread_loop) {
            pw_thread_loop_lock(thread_loop);
        }

        if (output_stream) {
            spa_hook_remove(&output_stream_listener);
            pw_stream_destroy(output_stream);
            output_stream = nullptr;
        }

        if (input_stream) {
            spa_hook_remove(&input_stream_listener);
            pw_stream_destroy(input_stream);
            input_stream = nullptr;
        }

        if (metadata) {
            spa_hook_remove(&metadata_listener);
            pw_proxy_destroy(reinterpret_cast<pw_proxy*>(metadata));
            metadata = nullptr;
        }

        if (registry) {
            spa_hook_remove(&registry_listener);
            pw_proxy_destroy(reinterpret_cast<pw_proxy*>(registry));
            registry = nullptr;
        }

        if (core) {
            spa_hook_remove(&core_listener);
            pw_core_disconnect(core);
            core = nullptr;
        }

        if (context) {
            pw_context_destroy(context);
            context = nullptr;
        }

        if (thread_loop) {
            pw_thread_loop_unlock(thread_loop);
            pw_thread_loop_stop(thread_loop);
            pw_thread_loop_destroy(thread_loop);
            thread_loop = nullptr;
        }
    }

    void initialize() {
        thread_loop = pw_thread_loop_new("clover-audio", nullptr);
        if (!thread_loop) {
            throw std::runtime_error("Failed to create audio thread loop");
        }

        context = pw_context_new(pw_thread_loop_get_loop(thread_loop), nullptr, 0);
        if (!context) {
            pw_thread_loop_destroy(thread_loop);
            thread_loop = nullptr;
            throw std::runtime_error("Failed to create audio context");
        }

        if (pw_thread_loop_start(thread_loop) < 0) {
            pw_context_destroy(context);
            pw_thread_loop_destroy(thread_loop);
            context     = nullptr;
            thread_loop = nullptr;
            throw std::runtime_error("Failed to start audio thread loop");
        }

        pw_thread_loop_lock(thread_loop);

        core = pw_context_connect(context, nullptr, 0);
        if (!core) {
            pw_thread_loop_unlock(thread_loop);
            pw_context_destroy(context);
            pw_thread_loop_stop(thread_loop);
            pw_thread_loop_destroy(thread_loop);
            context     = nullptr;
            thread_loop = nullptr;
            throw std::runtime_error("Failed to connect to audio server");
        }

        static const pw_core_events core_events = {
                .version = PW_VERSION_CORE_EVENTS,
                .error   = on_core_error,
        };
        pw_core_add_listener(core, &core_listener, &core_events, this);

        registry = pw_core_get_registry(core, PW_VERSION_REGISTRY, 0);
        if (!registry) {
            pw_thread_loop_unlock(thread_loop);
            throw std::runtime_error("Failed to get audio registry");
        }

        static const pw_registry_events registry_events = {
                .version = PW_VERSION_REGISTRY_EVENTS,
                .global  = on_registry_global,
        };
        pw_registry_add_listener(registry, &registry_listener, &registry_events, this);

        pw_thread_loop_unlock(thread_loop);
    }

    void create_streams() {
        pw_thread_loop_lock(thread_loop);

        if (has_output()) {
            create_output_stream();
        }

        if (has_input()) {
            create_input_stream();
        }

        pw_thread_loop_unlock(thread_loop);
    }

    void create_output_stream() {
        auto props = pw_properties_new(
                PW_KEY_MEDIA_TYPE,
                "Audio",
                PW_KEY_MEDIA_CATEGORY,
                "Playback",
                PW_KEY_MEDIA_ROLE,
                "Music",
                PW_KEY_APP_NAME,
                "Clover",
                PW_KEY_NODE_NAME,
                "clover-output",
                nullptr);

        output_stream = pw_stream_new(core, "Clover Output", props);
        if (!output_stream) {
            throw std::runtime_error("Failed to create output stream");
        }

        static const pw_stream_events stream_events = {
                .version       = PW_VERSION_STREAM_EVENTS,
                .state_changed = on_output_state_changed,
                .param_changed = on_output_param_changed,
                .process       = on_output_process,
        };
        pw_stream_add_listener(output_stream, &output_stream_listener, &stream_events, this);

        uint8_t buffer[1024];
        spa_pod_builder b;
        spa_pod_builder_init(&b, buffer, sizeof(buffer));

        spa_audio_info_raw audio_info = {
                .format   = SPA_AUDIO_FORMAT_F32,
                .rate     = static_cast<uint32_t>(cfg.sample_rate),
                .channels = static_cast<uint32_t>(cfg.channels_out),
        };

        const spa_pod* params[1];
        params[0] = spa_format_audio_raw_build(&b, SPA_PARAM_EnumFormat, &audio_info);

        auto flags = static_cast<pw_stream_flags>(PW_STREAM_FLAG_AUTOCONNECT | PW_STREAM_FLAG_MAP_BUFFERS);

        if (pw_stream_connect(output_stream, PW_DIRECTION_OUTPUT, PW_ID_ANY, flags, params, 1) < 0) {
            throw std::runtime_error("Failed to connect output stream");
        }
    }

    void create_input_stream() {
        auto props = pw_properties_new(
                PW_KEY_MEDIA_TYPE,
                "Audio",
                PW_KEY_MEDIA_CATEGORY,
                "Capture",
                PW_KEY_MEDIA_ROLE,
                "Music",
                PW_KEY_APP_NAME,
                "Clover",
                PW_KEY_NODE_NAME,
                "clover-input",
                nullptr);

        input_stream = pw_stream_new(core, "Clover Input", props);
        if (!input_stream) {
            throw std::runtime_error("Failed to create input stream");
        }

        static const pw_stream_events stream_events = {
                .version       = PW_VERSION_STREAM_EVENTS,
                .state_changed = on_input_state_changed,
                .param_changed = on_input_param_changed,
                .process       = on_input_process,
        };
        pw_stream_add_listener(input_stream, &input_stream_listener, &stream_events, this);

        uint8_t buffer[1024];
        spa_pod_builder b;
        spa_pod_builder_init(&b, buffer, sizeof(buffer));

        spa_audio_info_raw audio_info = {
                .format   = SPA_AUDIO_FORMAT_F32,
                .rate     = static_cast<uint32_t>(cfg.sample_rate),
                .channels = static_cast<uint32_t>(cfg.channels_in),
        };

        const spa_pod* params[1];
        params[0] = spa_format_audio_raw_build(&b, SPA_PARAM_EnumFormat, &audio_info);

        auto flags = static_cast<pw_stream_flags>(PW_STREAM_FLAG_AUTOCONNECT | PW_STREAM_FLAG_MAP_BUFFERS);

        if (pw_stream_connect(input_stream, PW_DIRECTION_INPUT, PW_ID_ANY, flags, params, 1) < 0) {
            throw std::runtime_error("Failed to connect input stream");
        }
    }

    void schedule_reconnect() {
        if (reconnect_pending.exchange(true)) {
            return;  // Already pending
        }

        // Disconnect and reconnect streams
        if (output_stream) {
            pw_stream_disconnect(output_stream);
            spa_hook_remove(&output_stream_listener);
            pw_stream_destroy(output_stream);
            output_stream = nullptr;
        }

        if (input_stream) {
            pw_stream_disconnect(input_stream);
            spa_hook_remove(&input_stream_listener);
            pw_stream_destroy(input_stream);
            input_stream = nullptr;
        }

        // Recreate streams
        if (has_output()) {
            create_output_stream();
        }
        if (has_input()) {
            create_input_stream();
        }

        reconnect_pending = false;
    }

    void signal_completion() {
        if (!completed.exchange(true)) {
            running = false;
            if (parent->audio_complete_callback) {
                parent->audio_complete_callback();
            }
            completion_signal.release();
        }
    }

    // Static callbacks
    static void on_core_error(void* data, uint32_t id, int seq, int res, const char* message) {
        auto* self = static_cast<impl*>(data);
        std::cerr << std::format(
                "Audio core error: id={} seq={} res={} message={}\n", id, seq, res, message ? message : "");
        if (id == PW_ID_CORE && res == -EPIPE) {
            self->signal_completion();
        }
    }

    static void on_registry_global(
            void* data,
            uint32_t id,
            uint32_t permissions,
            const char* type,
            uint32_t version,
            const spa_dict* props) {
        auto* self = static_cast<impl*>(data);

        if (std::strcmp(type, PW_TYPE_INTERFACE_Metadata) != 0) {
            return;
        }

        const char* name = spa_dict_lookup(props, PW_KEY_METADATA_NAME);
        if (!name || std::strcmp(name, "default") != 0) {
            return;
        }

        if (self->metadata) {
            return;  // Already bound
        }

        self->metadata_id = id;
        self->metadata =
                static_cast<pw_metadata*>(pw_registry_bind(self->registry, id, type, PW_VERSION_METADATA, 0));

        if (self->metadata) {
            static const pw_metadata_events metadata_events = {
                    .version  = PW_VERSION_METADATA_EVENTS,
                    .property = on_metadata_property,
            };
            pw_metadata_add_listener(self->metadata, &self->metadata_listener, &metadata_events, self);
        }
    }

    static int on_metadata_property(
            void* data, uint32_t subject, const char* key, const char* type, const char* value) {
        auto* self = static_cast<impl*>(data);

        if (!key) {
            return 0;
        }

        // Check if default audio sink or source changed
        if (std::strcmp(key, "default.audio.sink") == 0 || std::strcmp(key, "default.audio.source") == 0) {
            if (self->running && !self->reconnect_pending) {
                self->schedule_reconnect();
            }
        }

        return 0;
    }

    static void on_output_state_changed(
            void* data, enum pw_stream_state old_state, enum pw_stream_state state, const char* error) {
        auto* self = static_cast<impl*>(data);

        switch (state) {
            case PW_STREAM_STATE_ERROR:
                std::cerr << std::format("Output stream error: {}\n", error ? error : "unknown");
                if (self->running && !self->reconnect_pending) {
                    self->schedule_reconnect();
                }
                break;
            case PW_STREAM_STATE_STREAMING:
            case PW_STREAM_STATE_PAUSED:
            case PW_STREAM_STATE_UNCONNECTED:
            case PW_STREAM_STATE_CONNECTING:
                break;
        }
    }

    static void on_output_param_changed(void* data, uint32_t id, const spa_pod* param) {
        auto* self = static_cast<impl*>(data);

        if (!param || id != SPA_PARAM_Format) {
            return;
        }

        spa_audio_info_raw audio_info;
        if (spa_format_audio_raw_parse(param, &audio_info) >= 0) {
            self->negotiated_rate = static_cast<int>(audio_info.rate);
        }
    }

    static void on_output_process(void* data) {
        auto* self = static_cast<impl*>(data);

        if (!self->running || self->completed) {
            return;
        }

        pw_buffer* b = pw_stream_dequeue_buffer(self->output_stream);
        if (!b) {
            return;
        }

        spa_buffer* buf = b->buffer;
        float* output   = static_cast<float*>(buf->datas[0].data);
        if (!output) {
            pw_stream_queue_buffer(self->output_stream, b);
            return;
        }

        int channels_out = self->cfg.channels_out;
        int channels_in  = self->cfg.channels_in;

        uint32_t n_frames = buf->datas[0].maxsize / (sizeof(float) * channels_out);
        if (b->requested) {
            n_frames = std::min(n_frames, static_cast<uint32_t>(b->requested));
        }

        // Zero the buffer first
        std::memset(output, 0, n_frames * sizeof(float) * channels_out);

        float* input_ptr = self->current_input_buffer;

        for (uint32_t i = 0; i < n_frames; i++) {
            if (!self->parent->audio_callback) {
                break;
            }

            callback_args args{
                    .clock_time   = self->clock_time,
                    .channels_in  = channels_in,
                    .channels_out = channels_out,
                    .input        = input_ptr,
                    .output       = output + (static_cast<size_t>(i) * channels_out),
            };

            callback_status status = self->parent->audio_callback(args);
            self->clock_time++;

            if (input_ptr) {
                input_ptr += channels_in;
            }

            if (status == callback_status::end) {
                // Fill remaining with silence
                size_t remaining = (n_frames - i - 1) * sizeof(float) * channels_out;
                if (remaining > 0) {
                    std::memset(output + ((i + 1) * channels_out), 0, remaining);
                }
                self->signal_completion();
                break;
            }
        }

        buf->datas[0].chunk->offset = 0;
        buf->datas[0].chunk->stride = sizeof(float) * channels_out;
        buf->datas[0].chunk->size   = n_frames * sizeof(float) * channels_out;

        pw_stream_queue_buffer(self->output_stream, b);
    }

    static void on_input_state_changed(
            void* data, enum pw_stream_state old_state, enum pw_stream_state state, const char* error) {
        auto* self = static_cast<impl*>(data);

        if (state == PW_STREAM_STATE_ERROR) {
            std::cerr << std::format("Input stream error: {}\n", error ? error : "unknown");
            if (self->running && !self->reconnect_pending) {
                self->schedule_reconnect();
            }
        }
    }

    static void on_input_param_changed(void* data, uint32_t id, const spa_pod* param) {
        // Could capture input sample rate if needed
    }

    static void on_input_process(void* data) {
        auto* self = static_cast<impl*>(data);

        if (!self->running || self->completed) {
            return;
        }

        pw_buffer* b = pw_stream_dequeue_buffer(self->input_stream);
        if (!b) {
            return;
        }

        spa_buffer* buf   = b->buffer;
        float* input_data = static_cast<float*>(buf->datas[0].data);

        // Store for output process to use
        self->current_input_buffer = input_data;

        pw_stream_queue_buffer(self->input_stream, b);
    }
};

stream::stream(config cfg) : pimpl(std::make_unique<impl>(cfg, this)) {
    if (cfg.channels_in <= 0 && cfg.channels_out <= 0) {
        throw std::runtime_error("Stream requires at least input or output channels > 0");
    }

    pimpl->initialize();
    pimpl->create_streams();
}

stream::~stream() {
    stop();
}

void stream::start() {
    if (pimpl->running.exchange(true)) {
        return;  // Already running
    }

    pimpl->completed  = false;
    pimpl->clock_time = 0;
}

void stream::stop() {
    if (!pimpl->running.exchange(false)) {
        return;  // Not running
    }

    pimpl->signal_completion();
}

void stream::wait_to_complete() {
    pimpl->completion_signal.acquire();
}

bool stream::is_running() const {
    return pimpl->running;
}

int stream::negotiated_sample_rate() const {
    return pimpl->negotiated_rate ? pimpl->negotiated_rate : pimpl->cfg.sample_rate;
}

}  // namespace clover::io
