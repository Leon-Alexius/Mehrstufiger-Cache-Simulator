#ifndef TERMINAL_PARSER_H
#define TERMINAL_PARSER_H

// include the struct - which is included in parse.h
#include "parse.h" 

void print_help();
Config parse_user_input(int argc, char* argv[]);

#endif // TERMINAL_PARSER_H