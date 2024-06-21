#include <stdio.h>
#include <stdlib.h>

// Temporary
struct Request {
    
};

// The run_simulation method in C++
extern int run_simulation(
    int cycles, 
    unsigned l1CacheLines, unsigned l2CacheLines, unsigned cacheLineSize, 
    unsigned l1CacheLatency, unsigned l2CacheLatency, unsigned memoryLatency, 
    size_t numRequests, struct Request* requests,
    const char* tracefile
    );

int main(int argc, char* argv[]) {
    // check if arguments count is at least 10
    if(argc < 11) {
        printf("Insufficient arguments provided.\n");
        return 1;
    }

    // parse inputs
    int cycles = atoi(argv[1]);
    unsigned l1CacheLines = atoi(argv[2]);
    unsigned l2CacheLines = atoi(argv[3]);
    unsigned cacheLineSize = atoi(argv[4]);
    unsigned l1CacheLatency = atoi(argv[5]);
    unsigned l2CacheLatency = atoi(argv[6]);
    unsigned memoryLatency = atoi(argv[7]);
    size_t numRequests = atoi(argv[8]);

    struct Request requests[numRequests];

    // Initialize 'requests' array here as needed
    const char* tracefile = argv[9];

    // run simulation
    int result = 
    run_simulation(
        cycles, 
        l1CacheLines, 
        l2CacheLines, 
        cacheLineSize, 
        l1CacheLatency, 
        l2CacheLatency, 
        memoryLatency, 
        numRequests, 
        requests, 
        tracefile
    );

    // Process the simulation result
    if (result != 0) {
        printf("Success");
    }
    else {
        printf("Something is not right!");
    }

    return 0;
}