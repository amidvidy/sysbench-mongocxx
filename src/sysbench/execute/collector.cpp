#include "sysbench/execute/collector.hpp"

namespace sysbench {
namespace execute {

    void collector::ops_succeeded(duration dur, operation op_type, uint64_t num_ops) const {
        switch (op_type) {
        case operation::k_transaction:
            _transactions.fetch_add(num_ops);
            break;
        case operation::k_point_query:
            _point_queries.fetch_add(num_ops);
            break;
        case operation::k_simple_range:
            _simple_ranges.fetch_add(num_ops);
            break;
        case operation::k_sum_range:
            _sum_ranges.fetch_add(num_ops);
            break;
        case operation::k_order_range:
            _order_ranges.fetch_add(num_ops);
            break;
        case operation::k_distinct_range:
            _distinct_ranges.fetch_add(num_ops);
            break;
        case operation::k_indexed_update:
            _indexed_updates.fetch_add(num_ops);
            break;
        case operation::k_nonindexed_update:
            _nonindexed_updates.fetch_add(num_ops);
            break;
        case operation::k_insert:
            _inserts.fetch_add(num_ops);
            break;
        }
    }

    void collector::ops_failed(operation op_type, uint64_t num_ops) const {
    }

    uint64_t collector::count(operation op_type) const {
        switch (op_type) {
        case operation::k_transaction:
            return _transactions.load();
        case operation::k_point_query:
            return _point_queries.load();
        case operation::k_simple_range:
            return _simple_ranges.load();
        case operation::k_sum_range:
            return _sum_ranges.load();
        case operation::k_order_range:
            return _order_ranges.load();
        case operation::k_distinct_range:
            return _distinct_ranges.load();
        case operation::k_indexed_update:
            return _indexed_updates.load();
        case operation::k_nonindexed_update:
            return _nonindexed_updates.load();
        case operation::k_insert:
            return _inserts.load();
        }
    }

    scoped_operation::scoped_operation(collector *collector, operation op_type)
        : _rep{std::move(collector)}
        , _op_type{std::move(op_type)}
        , _num_ops{1}
        , _start{clock::now()}
        , _success{false}
    {}

    scoped_operation::scoped_operation(collector *collector, operation op_type, uint64_t num_ops)
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

    
}  // execute
}  // sysbench
