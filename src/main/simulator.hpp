// Lie Leon Alexius
// Don't delete this! This is linked to literally everywhere

#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

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
 * @brief Output of the simulator
 * @authors
 * Alexander Anthony Tang
 * Lie Leon Alexius
 */
typedef struct {
    size_t cycles;
    size_t misses;
    size_t hits;
    size_t read_hits;
    size_t read_misses;
    size_t write_hits;
    size_t write_misses;
    size_t read_hits_L1;
    size_t read_misses_L1;
    size_t write_hits_L1;
    size_t write_misses_L1;
    size_t read_hits_L2;
    size_t read_misses_L2;
    size_t write_hits_L2;
    size_t write_misses_L2;
} CacheStats;

/**
 * @brief Result contains `cycles`, `misses`, `hits`, `primitiveGateCount`
 * @author Task Pdf - edited Leon
 */
typedef struct {
    size_t cycles;
    size_t misses;
    size_t hits;
    size_t primitiveGateCount;
    CacheStats* cacheStats;
} Result;

#endif
