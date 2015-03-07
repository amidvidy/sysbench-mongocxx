#pragma once

#include <chrono>
#include <cstdint>

#include <tbb/atomic.h>
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
        void record(int64_t n) const;
        int64_t get_total() const;
        int64_t get_last() const;

    private:
        std::size_t current_bucket() const;
        mutable tbb::atomic<int64_t> _total{0};
        mutable tbb::concurrent_vector<tbb::atomic<int64_t>> _data;
        std::chrono::milliseconds _tick;
        clock::time_point _start;
    };

}  // namespace metrics
}  // namespace sysbench
