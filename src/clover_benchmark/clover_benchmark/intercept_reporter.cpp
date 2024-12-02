// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// It is licensed under the GPLv3. See LICENSE for details.

#include <memory>
#include <unordered_map>

#include <benchmark/benchmark.h>

#include "clover_benchmark/intercept_reporter.hpp"

std::unordered_map<std::string, ::benchmark::BenchmarkReporter::Run> InterceptReporter::run_map;

InterceptReporter::InterceptReporter() : default_reporter(benchmark::CreateDefaultDisplayReporter()) {
}

bool InterceptReporter::ReportContext(const Context& context) {
    return default_reporter->ReportContext(context);
}

void InterceptReporter::ReportRuns(const std::vector<Run>& runs) {
    run_map.reserve(runs.size());

    for (const auto& run : runs) {
        run_map[run.benchmark_name()] = run;
    }

    default_reporter->ReportRuns(runs);
}

void InterceptReporter::Finalize() {
    default_reporter->Finalize();
}
