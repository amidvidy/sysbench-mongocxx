#include <iostream>

#include "execute.hpp"
#include "load.hpp"
#include "metrics.hpp"
#include "report.hpp"

using namespace sysbench;

int main(int argc, char** argv) {
    try {
        
        load::options load_opts;
        execute::options exec_opts;

        // HACK
        exec_opts.docs_per_collection = load_opts.docs_per_collection;

        {
            // LOAD PHASE
            load::load_phase load_phase{load_opts};
            metrics::collector<metrics::load_op> load_collector{};
            report::console_logger logger{&load_collector};
            logger.start();
            load_phase.run(&collector);
        }

        {
            // EXECUTE PHASE
            execute::execute_phase execute_phase{exec_opts};
            metrics::collector<metrics::execute_op> execute_collector{};
            report::console_logger logger{&execute_collector{}};
            logger.start();
            execute_phase.run(&exec_collector);
        }
        
    } catch (const std::exception& ex) {
        std::cout << "something terrible happened" << std::endl;
    }
}
