// Lie Leon Alexius
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <getopt.h>
#include <string.h>

#include "parser/parse.h"
#include "grapher/printer.h"

// TEMPORARY
#include "placeHolder.hpp"

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
 * @author Lie Leon Alexius
 */
int main(int argc, char* argv[]) {
    // run parser
    Config config = start_parse(argc, argv);

    // EXPERIMENTAL
    testPrinter();

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
        printf("Success\n");
    } else {
        printf("Something is not right!\n");
    }

    return 0;
}
