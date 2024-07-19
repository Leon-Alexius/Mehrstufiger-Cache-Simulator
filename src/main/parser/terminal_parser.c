// Lie Leon Alexius

#include "terminal_parser.h"

/**
 * @brief Method for -h or --help
 * @note Shortest possible input is `./cache filename.csv`
 * @warning DO NOT CHANGE THIS
 * @author Lie Leon Alexius
 */
void print_help() {
    printf("Usage: ./cache [OPTIONS] path/to/file/filename.csv\n");
    printf("Options:\n");
    printf("  -c, --cycles <num>                The number of cycles to be simulated (default: 2_147_483_647 ~ INT32_MAX)\n");
    printf("      --cacheline-size <num>        The size of a cache line in bytes (default: 64)\n");
    printf("      --l1-lines <num>              The number of cache lines of the L1 cache (default: 64)\n");
    printf("      --l2-lines <num>              The number of cache lines of the L2 cache (default: 256)\n");
    printf("      --l1-latency <num>            The latency of the L1 cache in cycles (default: 4)\n");
    printf("      --l2-latency <num>            The latency of the L2 cache in cycles (default: 12)\n");
    printf("      --memory-latency <num>        The latency of the main memory in cycles (default: 100)\n");
    printf("      --tf=<filepath>               Output file for a trace file with all signals (default: None)\n");
    printf("      --num-requests <num>          Number of request to read from .csv file (default: all requests)\n");
    printf("      --prefetch-buffer <num>       The number of cache lines in the prefetch buffer (default: 0)\n");
    printf("      --storeback-buffer <num>      The number of cache lines in the storeback buffer (default: 0)\n");
    printf("      --storeback-condition <bool>  The condition for storeback buffer (default: false)\n");
    printf("      --pretty-print <bool>         Pretty print the output (default: true)\n");
    printf("  -h, --help                        Display this help and exit\n");
}

/**
 * @brief 
 * This function parses the user inputs and set-up the configuration.
 * 
 * @details
 * Each of the variables has default values:
 *  1. cycles = INT32_MAX (default simulation cycles)
 *  2. l1CacheLines = 64 (default number of lines in L1 cache)
 *  3. l2CacheLines = 256 (default number of lines in L2 cache)
 *  4. cacheLineSize = 64 (default cache line size in bytes)
 *  5. l1CacheLatency = 4 (default latency for L1 cache in cycles)
 *  6. l2CacheLatency = 12 (default latency for L2 cache in cycles)
 *  7. memoryLatency = 100 (default latency for memory access in cycles)
 *  8. numRequests = 0 (default number of requests is all requests in the file)
 *  9. tracefile = NULL (default trace file name) 
 *  10. customNumRequest = false (flag for custom number of requests)
 *  11. prefetchBuffer = 0 (default prefetch buffer)
 *  12. storebackBuffer = 0 (default storeback buffer)
 *  13. prettyPrint = true (default pretty print flag)
 *  14. storebackBufferCondition = false (default storeback buffer condition)
 * 
 * @author Lie Leon Alexius
 */
Config* parse_user_input(int argc, char* argv[]) {
    // Shortest possible input is `./cache filename.csv`
    if (argc < 2) {
        fprintf(stderr, "Invalid input: Filename is missing\n");
        print_help();
        exit(EXIT_FAILURE);
    }

    /*
        NOTES - DO NOT DELETE!
        Faster way to check: Input filename should be the last argument (positional argument)

        Alternative: using optind, but need to set optstring[0] to '+' and POSIXLY_CORRECT to 1
        with downside of: need to loop through the arguments, then check if `optind + 1 != argc`
        https://www.gnu.org/software/libc/manual/html_node/Getopt-Long-Options.html
    */
    const char* input_filename = argv[argc - 1];
    size_t len = strlen(input_filename);
    if (len <= 4 || strcmp(input_filename + len - 4, ".csv") != 0) {
        fprintf(stderr, "Filename should be the last argument and ends with .csv\n");
        print_help();
        exit(EXIT_FAILURE);
    }

    // Default Values
    int cycles = INT32_MAX;
    unsigned int l1CacheLines = 64;
    unsigned int l2CacheLines = 256;
    unsigned int cacheLineSize = 64;
    unsigned int l1CacheLatency = 4;
    unsigned int l2CacheLatency = 12;
    unsigned int memoryLatency = 100;
    size_t numRequests = 0; // unsigned integer
    const char* tracefile = NULL;
    bool customNumRequest = false;
    bool prettyPrint = true;

    // Optimization flags
    unsigned int prefetchBuffer = 0;
    unsigned int storebackBuffer = 0;
    bool storebackBufferCondition = false;

    // ========================================================================================

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
        {"num-requests", required_argument, 0, 0},
        {"prefetch-buffer", required_argument, 0, 0}, // Optimization: Prefetch Buffer
        {"storeback-buffer", required_argument, 0, 0}, // Optimization: Storeback Buffer
        {"storeback-condition", required_argument, 0, 0}, // Optimization: Conditional Storeback Buffer
        {"pretty-print", required_argument, 0, 'p'}, // New: Pretty Print Option
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}
    };

    int opt; // option flag input
    int long_index = 0; // the index for long_options 
    char* endptr; // parsing error checker

    // ========================================================================================
    
    /*  
        NOTES - DO NOT DELETE!
        optind: index of the next option (or non-option) to be processed in argument vector 
            (auto set to 1 by getopt_long, then incremented by getopt_long to the next option i.e. +2)
        argv[0]: ./cache
        argv[1]: --some-option-1
        argv[2]: some-argument 
        argv[3]: --some-option-2

        getopt_long() permutates the argv so that all non-option arguments are at the end
        Thus, optind will always point to the first non-option argument.

        https://linux.die.net/man/3/optind
    */

    // (arg count, arg array, legitimate option characters, long options, long options index)
    // https://linux.die.net/man/3/getopt_long
    while ((opt = getopt_long(argc, argv, "c:p:h", long_options, &long_index)) != -1) {        
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
            case 'p':
                if (strcmp("true", optarg) == 0) {
                    prettyPrint = 1;
                } 
                else if (strcmp("false", optarg) == 0) {
                    prettyPrint = 0;
                } 
                else {
                    fprintf(stderr, "Invalid input for pretty-print\n");
                    exit(EXIT_FAILURE);
                }
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
                    // No conversion needed for tracefile, directly assign optarg - no error check needed
                    tracefile = optarg;
                }
                else if (strcmp("num-requests", long_options[long_index].name) == 0) {
                    errno = 0;
                    numRequests = strtoul(optarg, &endptr, 10);
                    customNumRequest = true;

                    // Check for errors during conversion then print it
                    if (errno != 0 || *endptr != '\0') {
                        fprintf(stderr, "Invalid input for num-requests\n");
                        exit(EXIT_FAILURE);
                    }
                } 
                // Optimization: Prefetch Buffer
                else if (strcmp("prefetch-buffer", long_options[long_index].name) == 0) {
                    errno = 0;
                    prefetchBuffer = strtoul(optarg, &endptr, 10);

                    // Check for errors during conversion then print it
                    if (errno != 0 || *endptr != '\0') {
                        fprintf(stderr, "Invalid input for prefetch-buffer\n");
                        exit(EXIT_FAILURE);
                    }
                } 
                // Optimization: Storeback Buffer
                else if (strcmp("storeback-buffer", long_options[long_index].name) == 0) {
                    errno = 0;
                    storebackBuffer = strtoul(optarg, &endptr, 10);

                    // Check for errors during conversion then print it
                    if (errno != 0 || *endptr != '\0') {
                        fprintf(stderr, "Invalid input for storeback-buffer\n");
                        exit(EXIT_FAILURE);
                    }
                }
                // Optimization: Conditional Storeback Buffer
                else if (strcmp("storeback-condition", long_options[long_index].name) == 0) {
                    errno = 0;

                    if (strcmp("true", optarg) == 0) {
                    storebackBufferCondition = 1;
                    } 
                    else if (strcmp("false", optarg) == 0) {
                        storebackBufferCondition = 0;
                    } 
                    else {
                        fprintf(stderr, "Invalid input for storeback-condition\n");
                        exit(EXIT_FAILURE);
                    }
                }
                break;
            default:
                // if flag is undefined, print_help then exit
                print_help();
                exit(EXIT_FAILURE);
        }
    }

    // ========================================================================================

    // More than 1 filename check
    if (optind + 1 != argc) {
        fprintf(stderr, "Invalid input: Only one filename is allowed\n");
        print_help();
        exit(EXIT_FAILURE);
    }

    // ========================================================================================

    // Invalid cases check - throw error then quit
    // 1. If L1 cache size is greater than L2 cache size
    // 2. If L1 latency is greater than L2 latency or L2 latency is greater than memory latency
    // 3. if any of the cacheLines is set to 0 or cacheLineSize is less than 1 byte
    // 4. Cycles to simulate is less than 0

    if (l1CacheLines > l2CacheLines) {
        fprintf(stderr, "Invalid input: L1 cache lines count is greater than L2 cache lines count\n");
        exit(EXIT_FAILURE);
    }

    if (l1CacheLatency > l2CacheLatency || l2CacheLatency > memoryLatency) {
        fprintf(stderr, "Invalid input: L1 latency is greater than L2 latency or L2 latency is greater than memory latency\n");
        exit(EXIT_FAILURE);
    }

    if (l1CacheLines == 0 || l2CacheLines == 0 || cacheLineSize == 0) {
        fprintf(stderr, "Invalid input: L1 cache lines, L2 cache lines or cache line size is set to 0\n");
        exit(EXIT_FAILURE);
    }

    if (cycles < 0) {
        fprintf(stderr, "Invalid input: Cycles to simulate is less than 0\n");
        exit(EXIT_FAILURE);
    }

    // ========================================================================================

    Config* config = (Config*) malloc(sizeof(Config));

    // Check if memory allocation is successful
    if (config == NULL) {
        fprintf(stderr, "Error when allocating Config Struct\n");
        exit(EXIT_FAILURE);
    }

    // set-up the config
    config->cycles = cycles;
    config->l1CacheLines = l1CacheLines;
    config->l2CacheLines = l2CacheLines;
    config->cacheLineSize = cacheLineSize;
    config->l1CacheLatency = l1CacheLatency;
    config->l2CacheLatency = l2CacheLatency;
    config->memoryLatency = memoryLatency;
    config->numRequests = numRequests;
    config->tracefile = tracefile;
    config->input_filename = input_filename;
    config->requests = NULL;
    config->customNumRequest = customNumRequest;
    config->prefetchBuffer = prefetchBuffer; // Optimization: Prefetch Buffer
    config->storebackBuffer = storebackBuffer; // Optimization: Storeback Buffer
    config->storebackBufferCondition = storebackBufferCondition; // Optimization: Conditional Storeback Buffer
    config->prettyPrint = prettyPrint;

    return config;
}
