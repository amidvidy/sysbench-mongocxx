#pragma once

#include <cstdint>

#include <string>
#include <vector>

#include <mongocxx/client.hpp>

namespace sysbench {
namespace worker {

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
    
}
}
