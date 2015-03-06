#include <sstream>
#include <iomanip>
#include <chrono>

#include "sysbench/load/collector.hpp"

namespace sysbench {
namespace load {

    collector::collector(options opts)
        : _opts{std::move(opts)}
        , _insert_throughput{_opts.sample_resolution}
        , _insert_latency{1}
    {}

    void collector::ops_succeeded(metrics::duration dur, operation op_type, uint64_t num_ops) {
        switch (op_type) {
        case operation::k_insert:
            _insert_throughput.record(num_ops);
            _insert_latency.record(std::chrono::duration_cast<std::chrono::microseconds>(dur).count());
            break;
        }
    }

    void collector::ops_failed(operation op_type, uint64_t num_ops) {
    }

    std::string collector::report_string() const {
        std::stringstream ss;

        auto total_ops = _insert_throughput.get_total();
        auto total_seconds =
            std::chrono::duration_cast<std::chrono::seconds>(_insert_throughput.elapsed()).count();
        
        auto interval_ops = _insert_throughput.get_last();

        // TODO hack;
        if (total_seconds == 0) {
          return "";
        }

        double avg = _insert_latency.percentile(0.5) / 1000.0;
        double nn = _insert_latency.percentile(0.99) / 1000.0;
        double nnn = _insert_latency.percentile(0.999) / 1000.0;

        auto total_ips = total_ops / total_seconds;

        ss << std::setw(20) << std::right 
           << std::string{"total inserts: "} + std::to_string(total_ops);
        
        ss << std::setw(20) << std::right 
           << std::string{" || total ips: "} + std::to_string(total_ips);

        ss << std::setw(20) << std::right
           << std::string{" ||  interval ips: "} + std::to_string(interval_ops);
        
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

}
}
