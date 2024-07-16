// Lie Leon Alexius

#ifndef CSV_PARSER_H
#define CSV_PARSER_H

#include "parse.h"

int parse_csv(const char* input_filename, struct Request* requests, int numRequests, bool customReq);

#endif // CSV_PARSER_H
