#include "data.hpp"

namespace sysbench {
namespace data {

    std::string random_string(const char* mask, randgen* r) {
        // TODO, why is std::rand() so much faster than all the
        // stuff in <random> ??
        std::uniform_int_distribution<int> distribution(0, 25);
        std::string s{mask};
        for (std::size_t i = 0; i < s.length(); ++i) {
            if (s[i] == '#') {
                s[i] = 'a' + static_cast<char>(distribution(*r));
            }
        }
        return s;
    }
    
}
}
