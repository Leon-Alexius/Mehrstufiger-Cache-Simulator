#ifndef PRINTER_H
#define PRINTER_H

#include "../simulator.hpp"
#include "../parser/parse.h"

void print_layout(Config* config, Result* Result);
int load_image(const char *filename);

// Test Case
int testPrinter();

#endif // PRINTER_H
