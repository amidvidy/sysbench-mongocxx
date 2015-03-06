#pragma once

#include <atomic>
#include <chrono>
#include <cstdint>
#include <memory>
#include <thread>

#include "sysbench/execute/operations.hpp"

namespace sysbench {
namespace execute {

    using clock = std::chrono::high_resolution_clock;
    using duration = clock::duration;
    using time_point = clock::time_point;
    
    class collector {
        friend class scoped_operation;
    public:
        uint64_t count(execute::operation op_type) const;
    private:
        // these are threadsafe
        void ops_succeeded(duration dur, execute::operation op, uint64_t num_ops) const;
        void ops_failed(execute::operation op_type, uint64_t num_ops) const;

        mutable std::atomic<uint64_t> _transactions;
        mutable std::atomic<uint64_t> _point_queries;
        mutable std::atomic<uint64_t> _simple_ranges;
        mutable std::atomic<uint64_t> _sum_ranges;
        mutable std::atomic<uint64_t> _order_ranges;
        mutable std::atomic<uint64_t> _distinct_ranges;
        mutable std::atomic<uint64_t> _indexed_updates;
        mutable std::atomic<uint64_t> _nonindexed_updates;
        mutable std::atomic<uint64_t> _inserts;
    };

    class scoped_operation {
    public:
        scoped_operation(collector* collector, execute::operation op_type);
        scoped_operation(collector* collector, execute::operation op_type, uint64_t num_ops);
        ~scoped_operation();
        void succeeded();
    private:
        // non-owning
        collector* _rep;
        execute::operation _op_type;
        uint64_t _num_ops;
        time_point _start;
        bool _success;
    };
    
}  // namespace execute
}  // namespace sysbench
