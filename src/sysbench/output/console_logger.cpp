#include <iostream>
#include <chrono>

#include "sysbench/report/report.hpp"

namespace sysbench {
namespace report {

    console_logger::console_logger(collector* rep)
        : _rep{std::move(rep)}
        , _thread{}
        , _done{false} {
        }

    console_logger::~console_logger() {
        std::cout << "destroying logger" << std::endl;
        _done.store(true);
        if (_thread.joinable()) {
            _thread.join();
        }
    }

    void console_logger::start() {
        _thread = std::thread([this]() {

           uint64_t total_inserts = 0, interval_inserts = 0;
           auto interval = std::chrono::seconds{2};
           
           while (!this->_done.load()) {
               uint64_t current_inserts = this->_rep->total_inserts();
               
               interval_inserts = current_inserts - total_inserts;
               total_inserts = current_inserts;
           
               std::cout << "Cum inserts: " << total_inserts
                         << " || Cum throughput(ips): " << total_inserts / interval.count()
                         << " || Int inserts: " << interval_inserts
                         << " || Int throughput(ips): " << interval_inserts / interval.count()
                         << std::endl;
               std::this_thread::sleep_for(interval);
           }
        });
    }

}
}
