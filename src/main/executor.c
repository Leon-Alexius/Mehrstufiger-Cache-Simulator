// Lie Leon Alexius

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
extern Result run_simulation(
    int cycles,
    unsigned l1CacheLines, unsigned l2CacheLines, unsigned cacheLineSize, 
    unsigned l1CacheLatency, unsigned l2CacheLatency, unsigned memoryLatency, 
    size_t numRequests, struct Request* requests,
    const char* tracefile
);

/**
 * @brief Simulation starts here.
 * @param argc The number of command-line arguments.
 * @param argv An array of strings representing the command-line arguments.
 * 
 * @note
 * Most of the functionality here has been moved and combined to `simulator.cpp`
 * Printing the result of the simulation is also triggered from there.
 * 
 * @author Lie Leon Alexius
 */
int main(int argc, char* argv[]) {
    // run parser to get the config
    Config* config = start_parse(argc, argv);

    // Send the config to the simulator
    set_config(config);

    // run simulation (ignored return value due to the fact that it is not used - read @note)
    run_simulation(
        config->cycles, 
        config->l1CacheLines, config->l2CacheLines, config->cacheLineSize, 
        config->l1CacheLatency, config->l2CacheLatency, config->memoryLatency, 
        config->numRequests, config->requests, 
        config->tracefile
    );

    return 0;
}
