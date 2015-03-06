#pragma once

#include <string>
#include <chrono>

namespace sysbench {
namespace execute {

    struct options {
        uint32_t num_collections{1};
        uint32_t writer_threads{8};
        uint32_t seconds_per_feedback{1};

        uint32_t num_point_selects{1};
        uint32_t num_simple_ranges{1};
        uint32_t num_sum_ranges{1};
        uint32_t num_order_ranges{1};
        uint32_t num_distinct_ranges{1};
        uint32_t num_indexed_updates{1};
        uint32_t num_unindexed_updates{1};
        uint32_t num_inserts{1};

        uint32_t range_length{10};

        std::chrono::milliseconds sample_resolution{1000};
        std::chrono::seconds log_interval{1};
        std::chrono::minutes phase_total_time{10};

        uint32_t docs_per_collection{1000};

        std::string database_name{"sysbenchdb"};
    };

}  // execute
}  // sysbench
