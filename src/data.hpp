#include <random>

namespace sysbench {
namespace data {

    using randgen = std::mt19937;

    constexpr auto long_mask =
        "###########-###########-###########-"
        "###########-###########-###########-"
        "###########-###########-###########-###########";

    constexpr auto short_mask =
        "###########-###########-###########-"
        "###########-###########";

    std::string random_string(const char* mask, randgen* r);
    
}
}
