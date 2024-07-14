// Lie Leon Alexius
// Don't delete this! This is linked to literally everywhere

#ifndef SIMULATOR_ANALYZE_HPP
#define SIMULATOR_ANALYZE_HPP

#include <stdint.h>
#include <stddef.h>

/**
 * @brief Request contains the address, data, and write-enabled flag
 * @author Task Pdf
 */
struct Request {
    __uint32_t addr;
    __uint32_t data;
    int we; // write enabled 1 or write disabled 0
};

/**
 * @brief Result contains `cycles`, `misses`, `hits`, `primitiveGateCount`
 * @author Task Pdf - edited Leon
 */
typedef struct {
    size_t cycles;
    size_t misses;
    size_t hits;
    size_t primitiveGateCount;
} Result;

#endif
