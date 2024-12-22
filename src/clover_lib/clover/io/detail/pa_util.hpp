#pragma once

// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

namespace clover::io::pa_util {

void pa_initialize();
void pa_terminate();
void handle_pa_error(int err);
void print_pa_error(int err);

}  // namespace clover::io::pa_util