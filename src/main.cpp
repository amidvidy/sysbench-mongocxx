#include <iostream>

#include "load.hpp"
#include "metrics.hpp"
#include "report.hpp"

using namespace sysbench;

int main(int argc, char** argv) {
    try {
        metrics::collector collector{};
        load::options opts;
        load::load_phase load_phase{opts};
        report::console_logger logger{&collector};
        logger.start();
        load_phase.load(&collector);
    } catch (const std::exception& ex) {
        std::cout << "something terrible happened" << std::endl;
    }
}
