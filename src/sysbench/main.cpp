#include <iostream>

#include "sysbench/execute/phase.hpp"
#include "sysbench/load/phase.hpp"
#include "sysbench/metrics/metrics.hpp"
#include "sysbench/report/report.hpp"

using namespace sysbench;

int main(int argc, char** argv) {
    try {
        
        load::options load_opts;
        execute::options exec_opts;

        // HACK
        exec_opts.docs_per_collection = load_opts.docs_per_collection;

        {
            // LOAD PHASE
            load::phase phase{load_opts};
            metrics::collector collector{};
            report::console_logger logger{&collector};
            logger.start();
            phase.run(&collector);
        }

        {
            // EXECUTE PHASE
            execute::phase phase{exec_opts};
            metrics::collector collector{};
            report::console_logger logger{&collector};
            logger.start();
            phase.run(&collector);
        }
        
    } catch (const std::exception& ex) {
        std::cout << "something terrible happened" << std::endl;
    }
}