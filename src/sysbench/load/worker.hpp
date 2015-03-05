#pragma once

#include <cstdint>

#include <string>
#include <vector>

#include <mongocxx/client.hpp>

namespace sysbench {

namespace metrics {
    class collector;
}  // namespace metrics
    
namespace load {

    struct options;

    class worker {
    public:
        worker(uint32_t worker_id, options* opts);

        void work(metrics::collector* collector);
    private:
        uint32_t _id;
        mongocxx::client _client;
        // non-owning
        const options* _opts;
    };
    
}  // namespace load
}  // namespace worker
