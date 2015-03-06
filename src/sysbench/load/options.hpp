#pragma once

#include <cstdint>
#include <string>
#include <chrono>

namespace sysbench {
namespace load {

    struct options {
        // TODO switch some to unsigned
        uint32_t num_collections{16};
        std::string database_name{"sysbenchdb"};

        uint32_t writer_threads{1};
        uint64_t docs_per_collection{1000};
        uint64_t docs_per_insert{100};

        std::chrono::milliseconds sample_resolution{1000};
        std::chrono::seconds log_interval{10};

        std::string logfile;
        std::string index_technology;
        std::string write_concern{"safe"};

        std::string host{"localhost"};
        int16_t port{27017};

        std::string username{"myuser"};
        std::string password{"mypass"};
    };

}  // namespace load
}  // namespace sysbench
