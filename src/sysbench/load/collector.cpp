#include <sstream>
#include <iomanip>

#include "sysbench/load/collector.hpp"

namespace sysbench {
namespace load {

    collector::collector(options opts)
        : _opts{std::move(opts)}
        , _inserts{_opts.sample_resolution}
    {}

    void collector::ops_succeeded(metrics::duration dur, operation op_type, uint64_t num_ops) {
        switch (op_type) {
        case operation::k_insert:
            _inserts.record(num_ops);
            break;
        }
    }

    void collector::ops_failed(operation op_type, uint64_t num_ops) {
    }

    std::string collector::report_string() const {
        std::stringstream ss;

        auto total_ops = _inserts.get_total();
        auto total_seconds =
            std::chrono::duration_cast<std::chrono::seconds>(_inserts.elapsed()).count();

        // TODO hack;
        if (total_seconds == 0) {
            return "";
        }

        auto total_ips = total_ops / total_seconds;

        ss << std::setw(15) << std::right << std::string{"total inserts: "} + std::to_string(total_ops);
        ss << std::setw(15) << std::right << std::string{" || total ips: "} + std::to_string(total_ips);
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
