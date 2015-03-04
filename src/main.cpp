#include <iostream>

#include <load.hpp>

int main(int argc, char** argv) {
    sysbench::load::options opts;
    opts.docs_per_collection = 1000;
    opts.docs_per_insert = 10;
    sysbench::load::loader loader{opts};
    loader.load();
}
