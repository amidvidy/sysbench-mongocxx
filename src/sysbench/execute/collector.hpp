#pragma once

#include <atomic>
#include <chrono>
#include <cstdint>
#include <memory>
#include <thread>

#include "sysbench/execute/operations.hpp"
#include "sysbench/execute/options.hpp"
#include "sysbench/metrics/distribution.hpp"
#include "sysbench/metrics/series.hpp"
#include "sysbench/metrics/time.hpp"

namespace sysbench {
namespace execute {

    class collector {
        friend class scoped_operation;
    public:
        collector(options opts);
        
        std::string report_string() const;
    private:
        // these are threadsafe
        void ops_succeeded(metrics::duration dur, execute::operation op, uint64_t num_ops) const;
        void ops_failed(execute::operation op_type, uint64_t num_ops) const;

        options _opts;

        mutable metrics::series _distinct_range_throughput;
        mutable metrics::series _indexed_update_throughput;
        mutable metrics::series _insert_throughput;
        mutable metrics::series _order_range_throughput;
        mutable metrics::series _point_query_throughput;
        mutable metrics::series _simple_range_throughput;
        mutable metrics::series _sum_range_throughput;
        mutable metrics::series _transaction_throughput;
        mutable metrics::series _unindexed_update_throughput;

        mutable metrics::distribution _distinct_range_latency;
        mutable metrics::distribution _indexed_update_latency;
        mutable metrics::distribution _insert_latency;
        mutable metrics::distribution _order_range_latency;
        mutable metrics::distribution _point_query_latency;
        mutable metrics::distribution _simple_range_latency;
        mutable metrics::distribution _sum_range_latency;
        mutable metrics::distribution _transaction_latency;
        mutable metrics::distribution _unindexed_update_latency;
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
        metrics::time_point _start;
        bool _success;
    };

}  // namespace execute
}  // namespace sysbench
