#pragma once

#include <atomic>
#include <chrono>
#include <cstdint>

#include <tbb/concurrent_vector.h>

#include "sysbench/metrics/time.hpp"

namespace sysbench {
namespace metrics {

    class series {
    public:
        series(std::chrono::milliseconds interval);
        series(std::chrono::milliseconds interval, metrics::time_point start);

        // threadsafe
        void record(uint64_t n) const;

    private:
        mutable std::atomic<uint64_t> _total{0};
        mutable tbb::concurrent_vector<std::atomic<uint64_t>> _data;
        std::chrono::milliseconds _interval;
        clock::time_point _start;
    };

}  // namespace metrics
}  // namespace sysbench
