// Lie Leon Alexius
#include <stdio.h>
#include <stdlib.h>

// Temporary
#include "placeHolder.hpp"

// The run_simulation method in C++
extern int run_simulation(
    int cycles, 
    unsigned l1CacheLines, unsigned l2CacheLines, unsigned cacheLineSize, 
    unsigned l1CacheLatency, unsigned l2CacheLatency, unsigned memoryLatency, 
    size_t numRequests, struct Request* requests,
    const char* tracefile
    );

/*  
    The main function is the entry point of the program.
    
    This function parses the user inputs and set the variables accordingly.

    Each of the variables has default values:
    1. cycles = 1000000 (default simulation cycles)
    2. l1CacheLines = 64 (default number of lines in L1 cache)
    3. l2CacheLines = 256 (default number of lines in L2 cache)
    4. cacheLineSize = 64 (default cache line size in bytes)
    5. l1CacheLatency = 4 (default latency for L1 cache in cycles)
    6. l2CacheLatency = 12 (default latency for L2 cache in cycles)
    7. memoryLatency = 100 (default latency for memory access in cycles)
    8. numRequests = 1000 (default number of requests)
    9. tracefile = "default_trace.vcd" (default trace file name)

    Source:
    https://d-nb.info/978930487/34
*/
int main(int argc, char* argv[]) {
    // Default values
    int cycles = 1000000;
    unsigned l1CacheLines = 64;
    unsigned l2CacheLines = 256;
    unsigned cacheLineSize = 64;
    unsigned l1CacheLatency = 4;
    unsigned l2CacheLatency = 12;
    unsigned memoryLatency = 100;
    size_t numRequests = 1000;
    const char* tracefile = "default_trace.txt";

    // Override default values if arguments are provided
    if (argc > 1) cycles = atoi(argv[1]);
    if (argc > 2) l1CacheLines = atoi(argv[2]);
    if (argc > 3) l2CacheLines = atoi(argv[3]);
    if (argc > 4) cacheLineSize = atoi(argv[4]);
    if (argc > 5) l1CacheLatency = atoi(argv[5]);
    if (argc > 6) l2CacheLatency = atoi(argv[6]);
    if (argc > 7) memoryLatency = atoi(argv[7]);
    if (argc > 8) numRequests = atoi(argv[8]);
    if (argc > 9) tracefile = argv[9];

    struct Request requests[numRequests];

    // Initialize the requests array or read from tracefile
    // Assuming the tracefile provides the request data
    // Implementation of reading requests from tracefile goes here

    // run simulation
    int result = 
    run_simulation(
        cycles, 
        l1CacheLines, l2CacheLines, cacheLineSize, 
        l1CacheLatency, l2CacheLatency, memoryLatency, 
        numRequests, requests, 
        tracefile
    );

    // Process the simulation result
    if (result == 0) {
        printf("Success\n");
    }
    else {
        printf("Something is not right!\n");
    }

    return 0;
}
