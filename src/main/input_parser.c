// Lie Leon Alexius
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

// Request and Result struct
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
    After that, it will calls the simulator to run the simulation.

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
    const char* tracefile = "default_trace.vcd";

    // Start parsing the input
    // Override default values if arguments are provided
    char* endptr;

    if (argc > 1) {
        // To distinguish success/failure after call
        errno = 0; 
        cycles = strtol(argv[1], &endptr, 10);

        // Check for errors during conversion then print it
        if (errno == ERANGE && (cycles == LONG_MAX || cycles == LONG_MIN)) {
            perror("Number is out of range of \"long integer\"!");
            exit(EXIT_FAILURE);
        }
        else if (errno != 0 && cycles == 0) {
            perror("Error happened during strtol(argv[1]) - cycles ");
            exit(EXIT_FAILURE);
        }

        // if pointer ends up to argv[1] then no valid input
        if (endptr == argv[1]) {
            fprintf(stderr, "No digits were found\n");
            exit(EXIT_FAILURE);
        }
    }

    if (argc > 2) {
        errno = 0;
        l1CacheLines = strtoul(argv[2], &endptr, 10);

        // Check for errors during conversion then print it
        if (errno == ERANGE || *endptr != '\0') {
            perror("Invalid input for l1CacheLines");
            exit(EXIT_FAILURE);
        }
    }

    if (argc > 3) {
        errno = 0;
        l2CacheLines = strtoul(argv[3], &endptr, 10);

        // Check for errors during conversion then print it
        if (errno == ERANGE || *endptr != '\0') {
            perror("Invalid input for l2CacheLines");
            exit(EXIT_FAILURE);
        }
    }

    if (argc > 4) {
        errno = 0;
        cacheLineSize = strtoul(argv[4], &endptr, 10);

        // Check for errors during conversion then print it
        if (errno == ERANGE || *endptr != '\0') {
            perror("Invalid input for cacheLineSize");
            exit(EXIT_FAILURE);
        }
    }

    if (argc > 5) {
        errno = 0;
        l1CacheLatency = strtoul(argv[5], &endptr, 10);

        // Check for errors during conversion then print it
        if (errno == ERANGE || *endptr != '\0') {
            perror("Invalid input for l1CacheLatency");
            exit(EXIT_FAILURE);
        }
    }

    if (argc > 6) {
        errno = 0;
        l2CacheLatency = strtoul(argv[6], &endptr, 10);

        // Check for errors during conversion then print it
        if (errno == ERANGE || *endptr != '\0') {
            perror("Invalid input for l2CacheLatency");
            exit(EXIT_FAILURE);
        }
    }

    if (argc > 7) {
        errno = 0;
        memoryLatency = strtoul(argv[7], &endptr, 10);

        // Check for errors during conversion then print it
        if (errno == ERANGE || *endptr != '\0') {
            perror("Invalid input for memoryLatency");
            exit(EXIT_FAILURE);
        }
    }

    if (argc > 8) {
        errno = 0;
        numRequests = strtoul(argv[8], &endptr, 10);

        // Check for errors during conversion then print it
        if (errno == ERANGE || *endptr != '\0') {
            perror("Invalid input for numRequests");
            exit(EXIT_FAILURE);
        }
    }

    if (argc > 9) {
        tracefile = argv[9];
    }

    // the requests
    struct Request requests[numRequests];

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
