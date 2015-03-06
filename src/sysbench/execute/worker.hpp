#pragma once

#include <cstdint>

#include <mongocxx/client.hpp>

namespace sysbench {

namespace execute {

    class collector;
    class phase;
    struct options;
    
    class worker {
    public:
        worker(uint32_t worker_id, options* opts, phase* phase);
        void work(collector* collector);
    private:
        uint32_t _id;
        mongocxx::client _client;
        // non-owning
        const options* _opts;
        // non-owning
        phase* _phase;
    };
    
}
}
