#include <chrono>
#include <memory>
#include <cstdint>

namespace sysbench {
namespace report {

    using clock = std::chrono::high_resolution_clock;
    using duration = std::chrono::duration<uint64_t>;

    enum class load_ops {
        insert
    };

    enum class execute_ops {
        point_query,
        simple_range,
        sum_range,
        order_range,
        district_range,
        indexed_update,
        nonindexed_update,
        insert
    };

    class reporter {
        reporter();
        friend class scoped_op;
        scoped_operation start_op();
    private:
        void _op_succeeded(duration dur);
        void _op_failed(duration dur)
    }

    // a scoped operation
    class reporter::scoped_operation {
    public:
        operation(reporter);
        ~operation();
        void succeeded();
    private:
        clock::time_point _start;
        bool _success;
    }

}
}
