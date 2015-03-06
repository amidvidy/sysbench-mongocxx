#pragma once

#include <cstdint>
#include <ostream>

namespace sysbench {
namespace output {

    enum class color : std::uint32_t {
        k_fg_red = 31,
        k_fg_green = 32,
        k_fg_blue = 34,
        k_fg_default = 39,
        k_bg_red = 41,
        k_bg_green = 42,
        k_bg_blue = 44,
        k_bg_default = 49
    };

    inline std::ostream& operator<<(std::ostream& os, const color& color) {
        return os << "\033[" << static_cast<std::uint32_t>(color) << "m";
    }

}  // namespace output
}  // namespace colors

