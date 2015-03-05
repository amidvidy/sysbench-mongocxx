namespace sysbench {

    enum class load_op {
        k_insert,
    };

    enum class execute_op {
        k_point_query,
        k_simple_range,
        k_sum_range,
        k_order_range,
        k_district_range,
        k_indexed_update,
        k_nonindexed_update,
        k_insert
   };

}