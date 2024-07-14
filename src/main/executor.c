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
 * @brief The set_config method in C++ to set the config in C++ from C
 * @author Lie Leon Alexius
 */
extern void set_config(Config* c);

/**
 * @brief The run_simulation method in C++
 * @author Lie Leon Alexius
 */
extern Result* run_simulation(
    int cycles,
    unsigned l1CacheLines, unsigned l2CacheLines, unsigned cacheLineSize, 
    unsigned l1CacheLatency, unsigned l2CacheLatency, unsigned memoryLatency, 
    size_t numRequests, struct Request* requests,
    const char* tracefile
);

/**
 * @brief test the config
 * @author Lie Leon Alexius
 */
void test_config(Config* config) {
    printf(" ================================= START TEST CONFIG =================================\n");
    printf("Cycles: %d\n", config->cycles); // 1000000
    printf("L1 Cache Line: %u\n", config->l1CacheLines); // 64
    printf("L2 Cache Line: %u\n", config->l2CacheLines); // 256
    printf("Cache Line Size: %u\n", config->cacheLineSize); // 64
    printf("L1 Cache Latency: %u\n", config->l1CacheLatency); // 4
    printf("L2 Cache Latency: %u\n", config->l2CacheLatency); // 12
    printf("Memory Latency: %u\n", config->memoryLatency); // 100
    printf("Num Requests: %lu\n", config->numRequests); // 1000
    printf("Tracefile: %s\n", config->tracefile); // "src/assets/vcd/default_trace.vcd"
    printf("Input Filename: %s\n", config->input_filename); // "src/assets/csv/test_valid.csv"
    printf("customNumRequest: %d\n", config->customNumRequest); // 0 (false)
    printf("Prefetch Buffer: %u\n", config->prefetchBuffer); // 0
    printf("Storeback Buffer: %u\n", config->storebackBuffer); // 0
    printf("Pretty Print: %d\n", config->prettyPrint); // 1 (true)
    printf(" ================================== END TEST CONFIG ==================================\n\n");
}

/**
 * @brief Simulation starts here.
 *
 * @details
 * The executor program is responsible for executing a given command with the specified arguments.
 * It takes command-line arguments as input and returns an exit status.
 *
 * @param argc The number of command-line arguments.
 * @param argv An array of strings representing the command-line arguments.
 * 
 * @warning don't delete the tests
 * 
 * @author Lie Leon Alexius
 */
int main(int argc, char* argv[]) {
    // run parser
    Config* config = start_parse(argc, argv);

    // Test (test Request is in simulator.cpp)
    // test_config(config);

    // Send the config
    set_config(config);

    // run simulation
    Result* result =
    run_simulation(
        config->cycles, 
        config->l1CacheLines, config->l2CacheLines, config->cacheLineSize, 
        config->l1CacheLatency, config->l2CacheLatency, config->memoryLatency, 
        config->numRequests, config->requests, 
        config->tracefile
    );

    // Print the layout and result
    print_layout(config, result);

    // Print End of Simulation
    printf("Simulation has ended\n");

    // cleanup
    free(config->requests);
    config->requests = NULL;
    free(config);
    config = NULL;
    free(result->cacheStats);
    result->cacheStats = NULL;
    free(result);
    result = NULL;

    return 0;
}
