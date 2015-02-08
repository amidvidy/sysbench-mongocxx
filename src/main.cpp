#include <iostream>

#include <load.hpp>

int main(int argc, char** argv) {
    sysbench::load::options opts;
    sysbench::load::loader loader{opts};
    loader.load();
}
