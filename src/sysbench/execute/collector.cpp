#include <chrono>
#include <sstream>
#include <iomanip>

#include "sysbench/execute/collector.hpp"

namespace sysbench {
namespace execute {

   namespace {
     auto count_micros = [](metrics::duration dur) { 
        return std::chrono::duration_cast<std::chrono::microseconds>(dur).count();
     };
   }  // namespace

   collector::collector(options opts)
     : _opts{std::move(opts)}
     
     , _distinct_range_throughput{_opts.sample_resolution}
     , _indexed_update_throughput{_opts.sample_resolution}
     , _insert_throughput{_opts.sample_resolution}
     , _order_range_throughput{_opts.sample_resolution}
     , _point_query_throughput{_opts.sample_resolution}
     , _simple_range_throughput{_opts.sample_resolution}
     , _sum_range_throughput{_opts.sample_resolution}
     , _transaction_throughput{_opts.sample_resolution}
     , _unindexed_update_throughput{_opts.sample_resolution}
    
     , _distinct_range_latency{1}
     , _indexed_update_latency{1}
     , _insert_latency{1}
     , _order_range_latency{1}
     , _point_query_latency{1}
     , _simple_range_latency{1}
     , _sum_range_latency{1}
     , _transaction_latency{1}
     , _unindexed_update_latency{1}
    {}

    void collector::ops_succeeded(metrics::duration dur, operation op_type, uint64_t num_ops) const {
        switch (op_type) {
        case operation::k_transaction:
            _transaction_throughput.record(num_ops);
            _transaction_latency.record(count_micros(dur));
            break;
        case operation::k_point_query:
            _point_query_throughput.record(num_ops);
            _point_query_latency.record(count_micros(dur));
            break;
        case operation::k_simple_range:
            _simple_range_throughput.record(num_ops);
            _simple_range_latency.record(count_micros(dur));
            break;
        case operation::k_sum_range:
            _sum_range_throughput.record(num_ops);
            _sum_range_latency.record(count_micros(dur));
            break;
        case operation::k_order_range:
            _order_range_throughput.record(num_ops);
            _order_range_latency.record(count_micros(dur));
            break;
        case operation::k_distinct_range:
            _distinct_range_throughput.record(num_ops);
            _distinct_range_latency.record(count_micros(dur));
            break;
        case operation::k_indexed_update:
            _indexed_update_throughput.record(num_ops);
            _indexed_update_latency.record(count_micros(dur));
            break;
        case operation::k_unindexed_update:
            _unindexed_update_throughput.record(num_ops);
            _unindexed_update_latency.record(count_micros(dur));
            break;
        case operation::k_insert:
            _insert_throughput.record(num_ops);
            _insert_latency.record(count_micros(dur));
            break;
        }
    }

    void collector::ops_failed(operation op_type, uint64_t num_ops) const {
    }

    std::string collector::report_string() const {
        std::stringstream ss;

        auto total_ops = _transaction_throughput.get_total();
        auto total_seconds =
            std::chrono::duration_cast<std::chrono::seconds>(_transaction_throughput.elapsed()).count();
        
        auto interval_ops = _transaction_throughput.get_last();

        // TODO hack;
        if (total_seconds == 0) {
          return "";
        }

        double avg = _transaction_latency.percentile(0.5) / 1000.0;
        double nn = _transaction_latency.percentile(0.99) / 1000.0;
        double nnn = _transaction_latency.percentile(0.999) / 1000.0;

        auto total_ips = total_ops / total_seconds;

        ss << std::setw(20) << std::right 
           << std::string{"total transaction: "} + std::to_string(total_ops);
        
        ss << std::setw(20) << std::right 
           << std::string{" || total tps: "} + std::to_string(total_ips);

        ss << std::setw(20) << std::right
           << std::string{" ||  interval tps: "} + std::to_string(interval_ops);
        
        ss << std::setw(20) << std::right
           << std::string{" ||  avg latency: "} + std::to_string(avg) + std::string("ms");

        ss << std::setw(20) << std::right
           << std::string{" ||  99% latency: "} + std::to_string(nn) + std::string("ms");

        ss << std::setw(20) << std::right
           << std::string{" ||  99.9% latency: "} + std::to_string(nnn) + std::string("ms");
        

        return ss.str();
    }

    scoped_operation::scoped_operation(collector *collector, operation op_type)
        : _rep{std::move(collector)}
        , _op_type{std::move(op_type)}
        , _num_ops{1}
        , _start{metrics::clock::now()}
        , _success{false}
    {}

    scoped_operation::scoped_operation(collector *collector, operation op_type, uint64_t num_ops)
        : _rep{std::move(collector)}
        , _op_type{std::move(op_type)}
        , _num_ops{num_ops}
        , _start{metrics::clock::now()}
        , _success{false}
    {}

    scoped_operation::~scoped_operation() {
        if (_success) {
            auto end = metrics::clock::now();
            _rep->ops_succeeded(end - _start, _op_type, _num_ops);
        } else {
            _rep->ops_failed(_op_type, _num_ops);
        }
    }

    void scoped_operation::succeeded() {
        _success = true;
    }


}  // execute
}  // sysbench
