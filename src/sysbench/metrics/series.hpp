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
        series(std::chrono::milliseconds tick);
        series(std::chrono::milliseconds tick, metrics::time_point start);

        duration elapsed() const;
        std::chrono::milliseconds tick() const;

        // these are threadsafe
        void record(uint64_t n) const;
        uint64_t get_total() const;
        uint64_t get_current() const;

    private:
        std::size_t current_bucket() const;
        mutable std::atomic<uint64_t> _total{0};
        mutable tbb::concurrent_vector<std::atomic<uint64_t>> _data;
        std::chrono::milliseconds _tick;
        clock::time_point _start;
    };

}  // namespace metrics
}  // namespace sysbench
