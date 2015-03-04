#include "metrics.hpp"

namespace sysbench {
namespace metrics {

    void collector::ops_succeeded(duration dur, load_op op_type, uint64_t num_ops) {
        //_inserts.fetch_add(num_ops, std::memory_order_relaxed);
        switch (op_type) {
        case load_op::k_insert:
            _inserts.fetch_add(num_ops);
            break;
        }
    }

    void collector::ops_failed(load_op op_type, uint64_t num_ops) {
    }

    scoped_operation::scoped_operation(collector *collector, load_op op_type)
        : _rep{std::move(collector)}
        , _op_type{std::move(op_type)}
        , _num_ops{1}
        , _start{clock::now()}
        , _success{false}
    {}

    scoped_operation::scoped_operation(collector *collector, load_op op_type, uint64_t num_ops)
        : _rep{std::move(collector)}
        , _op_type{std::move(op_type)}
        , _num_ops{num_ops}
        , _start{clock::now()}
        , _success{false}
    {}

    scoped_operation::~scoped_operation() {
        if (_success) {
            auto end = clock::now();
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
