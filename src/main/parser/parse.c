// Lie Leon Alexius
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <getopt.h>
#include <string.h>

#include "parse.h"
#include "csv_parser.h"
#include "terminal_parser.h"

// Request and Result struct (TEMP)
#include "../placeHolder.hpp"

Config start_parse(int argc, char* argv[]) {

    // Parse User Input
    Config config = parse_user_input(argc, argv);

    // ========================================================================================

    // Parse .csv
    // Allocate memory for the requests array
    config.requests = malloc(config.numRequests * sizeof(struct Request));
    if (config.requests == NULL) {
        fprintf(stderr, "Error when initializing Request Struct in Config\n");
        exit(EXIT_FAILURE);
    }
    parse_csv(config.input_filename, config.requests, config.numRequests);

    // ========================================================================================

    // return finished Configuration
    return config;
}