#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <cstring> // for std::memcpy
#include <algorithm> // for std::max

// SHOULD CONSIDER:
/*
WavWriter.withAudio(...);
WavWriter.withCuePoints([...]));
WavWriter.save(path);
WavWriter.load(path);?

*/


class WavWriter {
public:
    WavWriter() {
        // Determine the endian-ness of the system
        int num = 1;
        littleEndian = (*(char *)&num == 1);
    }

    // Method to save audio data to a .wav file
    bool save(const std::string& fileName, const std::vector<float>& audioData, int sampleRate, int channelCount, bool shouldNormalize) {
        std::ofstream outFile(fileName, std::ios::binary);

        if (!outFile) {
            return false;
        }

        std::vector<float> normalizedData = audioData;
        if (shouldNormalize) {
            normalizeData(normalizedData);
        }

        // Write WAV header
        writeWavHeader(outFile, normalizedData.size(), sampleRate, channelCount);

        // Write audio data
        for (float sample : normalizedData) {
            writeFloat(outFile, sample);
        }

        outFile.close();
        return true;
    }

private:
    bool littleEndian;

    // Helper function to normalize audio data
    void normalizeData(std::vector<float>& data) {
        float maxVal = 0.0f;
        for (float sample : data) {
            maxVal = std::max(maxVal, std::abs(sample));
        }
        if (maxVal > 0.0f) {
            for (float& sample : data) {
                sample /= maxVal;
            }
        }
    }

    // Helper function to write WAV header
    void writeWavHeader(std::ofstream& stream, size_t dataSize, int sampleRate, int channelCount) {
        int byteRate = sampleRate * channelCount * sizeof(float);
        int blockAlign = channelCount * sizeof(float);

        stream << "RIFF";
        writeInt32(stream, static_cast<int32_t>(36 + dataSize));
        stream << "WAVE";

        stream << "fmt ";
        writeInt32(stream, static_cast<int32_t>(16));
        writeInt16(stream, static_cast<int16_t>(1));  // PCM format
        writeInt16(stream, static_cast<int16_t>(channelCount));
        writeInt32(stream, static_cast<int32_t>(sampleRate));
        writeInt32(stream, static_cast<int32_t>(byteRate));
        writeInt16(stream, static_cast<int16_t>(blockAlign));
        writeInt16(stream, static_cast<int16_t>(8 * sizeof(float)));  // Bits per sample

        stream << "data";
        writeInt32(stream, static_cast<int32_t>(dataSize));
    }

    // Helper function to write a 32-bit integer to the stream
    void writeInt32(std::ofstream& stream, int32_t value) {
        if (!littleEndian) {
            value = __builtin_bswap32(value);
        }
        stream.write(reinterpret_cast<const char*>(&value), sizeof(int32_t));
    }

    // Helper function to write a 16-bit integer to the stream
    void writeInt16(std::ofstream& stream, int16_t value) {
        if (!littleEndian) {
            value = __builtin_bswap16(value);
        }
        stream.write(reinterpret_cast<const char*>(&value), sizeof(int16_t));
    }

    // Helper function to write a float to the stream
    void writeFloat(std::ofstream& stream, float value) {
        if (!littleEndian) {
            uint32_t intRepresentation;
            std::memcpy(&intRepresentation, &value, sizeof(float));
            intRepresentation = __builtin_bswap32(intRepresentation);
            std::memcpy(&value, &intRepresentation, sizeof(float));
        }
        stream.write(reinterpret_cast<const char*>(&value), sizeof(float));
    }
};
