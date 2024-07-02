#ifndef PARSE_H
#define PARSE_H

#include <stdlib.h>

// Request and Result struct (TEMP)
#include "../placeHolder.hpp"

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
} Config;

Config start_parse(int argc, char* argv[]);

#endif // PARSE_H