// Lie Leon Alexius
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <getopt.h>
#include <string.h>

#include "parser/parse.h"
#include "grapher/printer.h"
#include "simulator.hpp"

/**
 * @brief The run_simulation method in C++
 * @author Lie Leon Alexius
 */
extern int run_simulation(
    int cycles,
    unsigned l1CacheLines, unsigned l2CacheLines, unsigned cacheLineSize, 
    unsigned l1CacheLatency, unsigned l2CacheLatency, unsigned memoryLatency, 
    size_t numRequests, struct Request* requests,
    const char* tracefile
);

/**
 * @brief Simulation starts here
 * @warning don't delete the tests
 * @author Lie Leon Alexius
 */
int main(int argc, char* argv[]) {
    // run parser
    Config config = start_parse(argc, argv);

    // Test (test Request in simulator.cpp) - Don't delete this
    // 05/07/2024 - Default & Custom Values works perfectly fine
    printf("Cycles: %d\n", config.cycles); // 1000000
    printf("L1 Cache Line: %u\n", config.l1CacheLines); // 64
    printf("L2 Cache Line: %u\n", config.l2CacheLines); // 256
    printf("Cache Line Size: %u\n", config.cacheLineSize); // 64
    printf("L1 Cache Latency: %u\n", config.l1CacheLatency); // 4
    printf("L1 Cache Latency: %u\n", config.l2CacheLatency); // 12
    printf("Memory Latency: %u\n", config.memoryLatency); // 100
    printf("Num Requests: %lu\n", config.numRequests); // 1000
    printf("Tracefile: %s\n", config.tracefile); // "src/assets/vcd/default_trace.vcd"
    printf("Input Filename: %s\n", config.input_filename); // "src/assets/csv/test_valid.csv"
    printf("customNumRequest: %d\n", config.customNumRequest); // 0

    // run simulation
    int result = 
    run_simulation(
        config.cycles, 
        config.l1CacheLines, config.l2CacheLines, config.cacheLineSize, 
        config.l1CacheLatency, config.l2CacheLatency, config.memoryLatency, 
        config.numRequests, config.requests, 
        config.tracefile
    );

    // Process the simulation result
    if (result == 0) {
        printf("Simulation ended successfuly\n");
    } else {
        printf("Something is not right!\n");
    }

    // free the Config.requests
    free(config.requests);

    return 0;
}
