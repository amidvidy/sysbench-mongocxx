#include <iostream>

#include <load.hpp>
#include <metrics.hpp>

using namespace sysbench;

int main(int argc, char** argv) {
    metrics::collector collector{};
    load::options opts;
    opts.docs_per_collection = 1000;
    opts.docs_per_insert = 10;
    load::loader loader{opts};
    loader.load(&collector);
}
