#pragma once

namespace sysbench {
namespace execute {

    enum class operation {
        k_transaction,
        k_point_query,
        k_simple_range,
        k_sum_range,
        k_order_range,
        k_distinct_range,
        k_indexed_update,
        k_unindexed_update,
        k_insert
   };

}
}
