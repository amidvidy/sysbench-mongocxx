#include <iostream>

#include <mongocxx/instance.hpp>

#include "sysbench/execute/phase.hpp"
#include "sysbench/load/phase.hpp"
#include "sysbench/load/collector.hpp"
#include "sysbench/execute/collector.hpp"
#include "sysbench/output/console_logger.hpp"

using namespace sysbench;

constexpr char banner[] = R"(
                _                     _    
               | |                   | |    
  ___ _   _ ___| |__   ___ _ __   ___| |__  
 / __| | | / __| '_ \ / _ \ '_ \ / __| '_ \ 
 \__ \ |_| \__ \ |_) |  __/ | | | (__| | | |
 |___/\__, |___/_.__/ \___|_| |_|\___|_| |_|
       __/ |                                
      |___/                             
)";

int main(int argc, char** argv) {
    try {
        mongocxx::instance driver{};
        load::options load_opts;
        execute::options exec_opts;

        // HACK
        exec_opts.docs_per_collection = load_opts.docs_per_collection;

        std::cout << banner << std::endl;

        {
            // LOAD PHASE
            load::phase phase{load_opts};
            load::collector collector{load_opts};
            output::console_logger<load::collector> logger{&collector};
            logger.start();
            phase.run(&collector);
        }

        {
            // EXECUTE PHASE
            execute::phase phase{exec_opts};
            execute::collector collector{exec_opts};
            output::console_logger<execute::collector> logger{&collector};
            logger.start();
            phase.run(&collector);
        }

    } catch (const std::exception& ex) {
        std::cout << "something terrible happened" << std::endl;
    }
}
