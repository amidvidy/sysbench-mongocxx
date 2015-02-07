#include <cstdint>

#include <string>

namespace sysbench {
namespace load {

    struct options {
        uint32_t ncollections;
        std::string database_name;

        uint32_t writer_threads;
        uint64_t documents_per_collection;
        uint64_t documents_per_insert;
        uint64_t inserts_per_feedback;
        uint64_t seconds_per_feedback;

        std::string compression_type;
        uint32_t basement_size;

        std::string logfile;
        std::string index_technology;
        std::string write_concern;

        std::string host;
        int16_t port;

        std::string username;
        std::string password;
    };

    class loader {
    public:
        loader(options opts);
        loader(loader&&);
        loader& operator=(loader&&);
        ~loader();
    private:
        options _opts;
    };

}  // namespace load
}  // namespace sysbench
