#include <iostream>

#include "execute.hpp"
#include "load.hpp"
#include "metrics.hpp"
#include "report.hpp"

using namespace sysbench;

int main(int argc, char** argv) {
    try {
        metrics::collector collector{};

        load::options load_opts;
        execute::options exec_opts;
        
        load::load_phase load_phase{load_opts};
        
        execute::execute_phase execute_phase{exec_opts};
        report::console_logger logger{&collector};
        logger.start();
        load_phase.run(&collector);
    } catch (const std::exception& ex) {
        std::cout << "something terrible happened" << std::endl;
    }
}
