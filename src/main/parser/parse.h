#ifndef PARSE_H
#define PARSE_H

#include <stdlib.h>
#include <stdbool.h>

// Request and Result struct
#include "../simulator.hpp"

// Config struct
typedef struct {
    int cycles;
    unsigned int l1CacheLines;
    unsigned int l2CacheLines;
    unsigned int cacheLineSize;
    unsigned int l1CacheLatency;
    unsigned int l2CacheLatency;
    unsigned int memoryLatency;
    size_t numRequests;
    const char* tracefile;
    const char* input_filename;
    struct Request* requests;
    bool customNumRequest;

    // Optimization flags
    unsigned int prefetchBuffer;  // How many cacheLines does prefetchBuffer have
    unsigned int storebackBuffer; // How many cacheLines does storebackBuffer have
    bool storebackBufferCondition; // (during Read) false = always flush, true = flush only if tag exists or interrupt

    bool prettyPrint; // default is true, prints the details of the simulator
} Config;

Config* start_parse(int argc, char* argv[]);

#endif // PARSE_H