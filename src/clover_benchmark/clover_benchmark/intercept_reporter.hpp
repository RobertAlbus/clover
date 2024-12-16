#pragma once

// Clover Audio Framework
// Copyright (C) 2023  Rob W. Albus
// Licensed under the GPLv3. See LICENSE for details.

#include <memory>
#include <unordered_map>

#include <benchmark/benchmark.h>

class InterceptReporter : public benchmark::BenchmarkReporter {
   private:
    std::unique_ptr<benchmark::BenchmarkReporter> default_reporter;

   public:
    static std::unordered_map<std::string, double> run_map;

    InterceptReporter();

    bool ReportContext(const Context& context) override;

    void ReportRuns(const std::vector<Run>& runs) override;

    void Finalize() override;
};
