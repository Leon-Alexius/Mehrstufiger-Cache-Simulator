// Lie Leon Alexius
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <getopt.h>
#include <string.h>

// Request and Result struct
// #include "../modules/modules.hpp"
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
    DO NOT CHANGE THIS
    Method for -h or --help
    Shortest possible input is `./cache filename.csv`
*/
void print_help() {
    printf("Usage: ./cache [OPTIONS] filename.csv\n");
    printf("Options:\n");
    printf("  -c, --cycles <num>           The number of cycles to be simulated (default: 1000000)\n");
    printf("      --cacheline-size <num>   The size of a cache line in bytes (default: 64)\n");
    printf("      --l1-lines <num>         The number of cache lines of the L1 cache (default: 64)\n");
    printf("      --l2-lines <num>         The number of cache lines of the L2 cache (default: 256)\n");
    printf("      --l1-latency <num>       The latency of the L1 cache in cycles (default: 4)\n");
    printf("      --l2-latency <num>       The latency of the L2 cache in cycles (default: 12)\n");
    printf("      --memory-latency <num>   The latency of the main memory in cycles (default: 100)\n");
    printf("      --tf=<filepath>          Output file for a trace file with all signals (default: default_trace.vcd)\n");
    printf("  -h, --help                   Display this help and exit\n");
}

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

    // Filename for the input CSV file
    const char* input_filename = NULL;

    // ==========================================================================================
    // ============================== START PARSING USER INPUT ==================================
    // ==========================================================================================

    // Long options array
    struct option long_options[] = {
        // {const char *name; int has_arg; int *flag; int val;}
        {"cycles", required_argument, 0, 'c'},
        {"cacheline-size", required_argument, 0, 0},
        {"l1-lines", required_argument, 0, 0},
        {"l2-lines", required_argument, 0, 0},
        {"l1-latency", required_argument, 0, 0},
        {"l2-latency", required_argument, 0, 0},
        {"memory-latency", required_argument, 0, 0},
        {"tf", required_argument, 0, 0},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}
    };

    int opt; // option flag input
    int long_index = 0; // the index for long_options 
    char* endptr; // parsing error checker

    // (arg count, arg array, legitimate option characters, long options, long options index)
    // https://linux.die.net/man/3/getopt_long
    while ((opt = getopt_long(argc, argv, "c:h", long_options, &long_index)) != -1) {
        switch (opt) {
            case 'c':
                errno = 0;
                cycles = strtol(optarg, &endptr, 10);

                // Check for errors during conversion then print it
                if (errno != 0 || *endptr != '\0') {
                    fprintf(stderr, "Invalid input for cycles\n");
                    exit(EXIT_FAILURE);
                }
                break;
            case 'h':
                print_help();
                exit(EXIT_SUCCESS);
                break;
            case 0:
                if (strcmp("cacheline-size", long_options[long_index].name) == 0) {
                    errno = 0;
                    cacheLineSize = strtoul(optarg, &endptr, 10);

                    // Check for errors during conversion then print it
                    if (errno != 0 || *endptr != '\0') {
                        fprintf(stderr, "Invalid input for cacheline-size\n");
                        exit(EXIT_FAILURE);
                    }
                } 
                else if (strcmp("l1-lines", long_options[long_index].name) == 0) {
                    errno = 0;
                    l1CacheLines = strtoul(optarg, &endptr, 10);

                    // Check for errors during conversion then print it
                    if (errno != 0 || *endptr != '\0') {
                        fprintf(stderr, "Invalid input for l1-lines\n");
                        exit(EXIT_FAILURE);
                    }
                } 
                else if (strcmp("l2-lines", long_options[long_index].name) == 0) {
                    errno = 0;
                    l2CacheLines = strtoul(optarg, &endptr, 10);

                    // Check for errors during conversion then print it
                    if (errno != 0 || *endptr != '\0') {
                        fprintf(stderr, "Invalid input for l2-lines\n");
                        exit(EXIT_FAILURE);
                    }
                } 
                else if (strcmp("l1-latency", long_options[long_index].name) == 0) {
                    errno = 0;
                    l1CacheLatency = strtoul(optarg, &endptr, 10);

                    // Check for errors during conversion then print it
                    if (errno != 0 || *endptr != '\0') {
                        fprintf(stderr, "Invalid input for l1-latency\n");
                        exit(EXIT_FAILURE);
                    }
                } 
                else if (strcmp("l2-latency", long_options[long_index].name) == 0) {
                    errno = 0;
                    l2CacheLatency = strtoul(optarg, &endptr, 10);

                    // Check for errors during conversion then print it
                    if (errno != 0 || *endptr != '\0') {
                        fprintf(stderr, "Invalid input for l2-latency\n");
                        exit(EXIT_FAILURE);
                    }
                } 
                else if (strcmp("memory-latency", long_options[long_index].name) == 0) {
                    errno = 0;
                    memoryLatency = strtoul(optarg, &endptr, 10);

                    // Check for errors during conversion then print it
                    if (errno != 0 || *endptr != '\0') {
                        fprintf(stderr, "Invalid input for memory-latency\n");
                        exit(EXIT_FAILURE);
                    }
                } 
                else if (strcmp("tf", long_options[long_index].name) == 0) {
                    tracefile = optarg;
                }
                break;
            default:
                // if flag is undefined, print_help then exit
                print_help();
                exit(EXIT_FAILURE);
        }
    }

    // Get the remaining argument: the input filename
    // The variable optind is the index of the next element to be processed in argv.
    // https://linux.die.net/man/3/optind
    if (optind < argc) {
        input_filename = argv[optind];
    } 
    else {
        fprintf(stderr, "Input filename is required\n");
        print_help();
        exit(EXIT_FAILURE);
    }

    // Error checking: Validate input_filename
    // check if NULL
    if (input_filename == NULL) {
        fprintf(stderr, "Input filename is missing\n");
        print_help();
        exit(EXIT_FAILURE);
    }
    
    // Check if the filename ends with .csv
    size_t len = strlen(input_filename);
    if (len <= 4 || strcmp(input_filename + len - 4, ".csv") != 0) {
        fprintf(stderr, "Invalid filename. Filename should end with .csv\n");
        print_help();
        exit(EXIT_FAILURE);
    }

    // ==========================================================================================
    // =============================== END PARSING USER INPUT ===================================
    // ==========================================================================================

    // ==========================================================================================
    // ================================= START PARSING CSV ======================================
    // ==========================================================================================

    /*
        CSV Data (Strict rule):
        1. The 1st Column = R || W
        2. The 2nd Column = Address (either Hexadecimal or Decimal)
        3. The 3rd Column = Value (empty if R or some(v) if W)

        Exception:
        Throws Exc at stderr if there are any deviation from the rules
    */

    // initialize the struct
    struct Request requests[numRequests];

    // Read requests from the input CSV file
    FILE* file = fopen(input_filename, "r");
    if (!file) {
        // No need to close file here since fopen will return NULL
        perror("Failed to open input file");
        exit(EXIT_FAILURE);
    }
    else {
        // Read the requests from the file
        // setup buffer
        char rw[10]; 
        for (int i = 0; i < numRequests && !feof(file); i++) {
            fscanf(file, "%s %u", rw, &requests[i].addr);
            if (strcmp(rw, "Write") == 0) {
                requests[i].we = 1;
                fscanf(file, "%u", &requests[i].data);
            }
            else {
                requests[i].we = 0;
                requests[i].data = -76;
            }
        }

        fclose(file);
    }

    // ==========================================================================================
    // ================================== END PARSING CSV =======================================
    // ==========================================================================================

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
    } else {
        printf("Something is not right!\n");
    }

    return 0;
}
