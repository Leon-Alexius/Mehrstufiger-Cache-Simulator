#ifndef PARSE_H
#define PARSE_H

#include <stdlib.h>
#include <stdbool.h>

// Request and Result struct
#include "../simulator.hpp"

// Config struct
typedef struct {
    int cycles;
    unsigned l1CacheLines;
    unsigned l2CacheLines;
    unsigned cacheLineSize;
    unsigned l1CacheLatency;
    unsigned l2CacheLatency;
    unsigned memoryLatency;
    size_t numRequests;
    const char* tracefile;
    const char* input_filename;
    struct Request* requests;
    bool customNumRequest;
} Config;

Config start_parse(int argc, char* argv[]);

#endif // PARSE_H