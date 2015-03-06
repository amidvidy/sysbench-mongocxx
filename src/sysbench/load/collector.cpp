#include "sysbench/load/collector.hpp"

namespace sysbench {
namespace load {

    collector::collector(options opts)
        : _opts{std::move(opts)}
    {}

    void collector::ops_succeeded(metrics::duration dur, operation op_type, uint64_t num_ops) {
        switch (op_type) {
        case operation::k_insert:
            _inserts.fetch_add(num_ops);
            break;
        }
    }

    void collector::ops_failed(operation op_type, uint64_t num_ops) {
    }

    uint64_t collector::total_inserts() {
        return _inserts.load();
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
