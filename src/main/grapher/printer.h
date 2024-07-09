#ifndef PRINTER_H
#define PRINTER_H

void print_layout(int cycles, int l1CacheLines, int l2CacheLines, int cacheLineSize, int l1CacheLatency, int l2CacheLatency, int memoryLatency, int numRequests);
int load_image(const char *filename);

// Test Case
int testPrinter();

#endif // PRINTER_H
