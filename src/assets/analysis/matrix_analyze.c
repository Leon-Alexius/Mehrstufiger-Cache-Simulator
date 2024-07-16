#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <getopt.h>
#include <string.h>

#include "../../main/parser/parse.h"
#include "../../main/parser/csv_parser.h"
#include "../../main/parser/terminal_parser.h"
#include "../../modules/modules.hpp"


extern CacheStats* run_simulation_analyze(
    int cycles,
    size_t numRequests, struct Request* requests
);

extern void initialize_caches(
    unsigned l1CacheLines, unsigned l2CacheLines, unsigned cacheLineSize, 
    unsigned l1CacheLatency, unsigned l2CacheLatency, unsigned memoryLatency,
    const char* tracefile,
     // Optimization flags
    unsigned prefetchBuffer, 
    unsigned storebackBuffer,
    bool conditionalBuffer
);

int calculateLinesAnalyze(const char* input_filename) {
    FILE* file = fopen(input_filename, "r");
    if (file == NULL) {
        fprintf(stderr, "In calculateLines - failed to open file: %s\n", input_filename);
        exit(EXIT_FAILURE);
    }

    int count = 0; // count should be incremented only if a line contains at least one character
    char ch;
    bool isLineNotEmpty = false;
    while ((ch = fgetc(file)) != EOF) {
        if (ch != '\n') {
            isLineNotEmpty = true; // Current line has content
        } 
        else {
            if (isLineNotEmpty) {
                count++; // Increment count for non-empty line
                isLineNotEmpty = false; // Reset for the next line
            }
        }
    }

    fclose(file);
    return count + 1; // + 1 for last row (not having `\n`)
}

int main() {

    const char* input_filename[17] = {
        "examples/ijk/ijk.csv",
        "examples/ijk/ijk_opt1.csv",
        "examples/ijk/ijk_opt2.csv",
        "examples/ikj/ikj.csv",
        "examples/ikj/ikj_opt1.csv",
        "examples/jik/jik.csv",
        "examples/jik/jik_opt1.csv",
        "examples/jik/jik_opt2.csv",
        "examples/jki/jki.csv",
        "examples/jki/jki_opt1.csv",
        "examples/jki/jki_opt2.csv",
        "examples/kij/kij.csv",
        "examples/kij/kij_opt1.csv",
        "examples/kij/kij_opt2.csv",
        "examples/kji/kji.csv",
        "examples/kji/kji_opt1.csv",
        "examples/kji/kji_opt2.csv"
    };

    // Cache attributes
    unsigned l1CacheLines = 4;
    unsigned l2CacheLines = 16;
    unsigned cacheLineSize = 16;
    unsigned l1CacheLatency = 4;
    unsigned l2CacheLatency = 12;
    unsigned memoryLatency = 100;
    const char* tracefile = NULL;
    unsigned prefetchBuffer = 0;
    unsigned storebackBuffer = 0;
    bool conditionalBuffer = false;
    
    initialize_caches(
            l1CacheLines, l2CacheLines, cacheLineSize,
            l1CacheLatency, l2CacheLatency, memoryLatency,
            tracefile,
            prefetchBuffer,
            storebackBuffer,
            conditionalBuffer
    );

    FILE *fpt;

    char csv_file_name[200];
    sprintf(csv_file_name, "src/assets/analysis/mul_%d_%d_%d_%d_%d_%d_%d_%d_%d.csv", l1CacheLines, l2CacheLines, cacheLineSize, l1CacheLatency, l2CacheLatency, memoryLatency, prefetchBuffer, storebackBuffer, conditionalBuffer);
    fpt = fopen(csv_file_name, "w+");
    fprintf(fpt, "matrix, cycles, misses, hits, read-hits, read-miss, write-hits, write-miss, read-hits-l1, read-miss-l1, write-hits-l1, write-miss-l1, read-hits-l2, read-miss-l2, write-hits-l2, write-miss-l2\n");
    for (int i = 0; i < 17; i++) {
        int cycles = INT_MAX;
        
        // Parsing requests
        // unsigned numRequests = calculateLinesAnalyze(input_filename[i])
        unsigned numRequests = 1000;
        
        struct Request* requests = malloc((numRequests + 1) * sizeof(struct Request));
        
        parse_csv(input_filename[i], requests, numRequests, false);
        
        CacheStats* stats = run_simulation_analyze(
            cycles,
            numRequests, requests
        );

        // printf("%d\n", stats.cycles);

        fprintf(fpt, "%s, %lu, %lu, %lu, %lu, %lu, %lu, %lu, %lu, %lu, %lu, %lu, %lu, %lu, %lu, %lu\n",
        input_filename[i], stats->cycles, stats->misses, stats->hits, 
        stats->read_hits, stats->read_misses, stats->write_hits, stats->write_misses, 
        stats->read_hits_L1, stats->read_misses_L1, stats->write_hits_L1, stats->write_misses_L1, 
        stats->read_hits_L2, stats->read_misses_L2, stats->write_hits_L2, stats->write_misses_L2);
        free(stats);
    }

    fclose(fpt);
}