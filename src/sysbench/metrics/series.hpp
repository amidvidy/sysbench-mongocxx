#pragma once

#include <atomic>
#include <chrono>
#include <cstdint>

//#include <tbb/concurrent_vector.h>

#include "sysbench/metrics/time.hpp"

namespace sysbench {
namespace metrics {

    class series {
    public:
        series(std::chrono::milliseconds interval);
        series(std::chrono::milliseconds interval, metrics::time_point start);
    private:
        std::atomic<uint64_t> _total{0};
        std::chrono::milliseconds _interval;
        clock::time_point _start;
    };

}  // namespace metrics
}  // namespace sysbench
