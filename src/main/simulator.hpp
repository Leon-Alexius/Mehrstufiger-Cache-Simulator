// Lie Leon Alexius
// Don't delete this! This is linked to literally everywhere

#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

#include <stdint.h>
#include <stddef.h>

/**
 * @brief Request contains the address, data, and write-enabled flag
 * @warning Don't add anything to this struct
 * @author Task Pdf, Leon
 */
struct Request {
    __uint32_t addr;
    __uint32_t data;
    int we; // write enabled 1 or write disabled 0
};

/**
 * @brief Result contains `cycles`, `misses`, `hits`, `primitiveGateCount`
 * @warning Don't add anything to this struct
 * @author Task Pdf, Leon
 */
typedef struct {
    size_t cycles; // How many cycles needed to finish the simulation
    size_t misses; // Total cache misses count during simulation
    size_t hits; // Total cache hits count during simulation
    size_t primitiveGateCount; // Total Gate used in the simulation (Excluded Memory)
} Result;

/**
 * @brief Output of the simulator
 * @authors
 * Lie Leon Alexius
 * Alexander Anthony Tang
 */
typedef struct {
    size_t cycles; // cycle count for this specific request
    size_t misses; // 0 or 1 - this specific request is a cache miss
    size_t hits; // 0 or 1 - this specific request is a cache hit
    size_t primitiveGateCount; // Total Gate used in the simulation (Excluded Memory)
    size_t read_hits; // 0 or 1 - read hit in L1 or L2
    size_t read_misses; // 0 or 1 - read miss in L1 or L2
    size_t write_hits; // 0 or 1 - write hit in L1 or L2
    size_t write_misses; // 0 or 1 - write miss in L1 or L2
    size_t read_hits_L1; // 0 or 1 - read hit in L1
    size_t read_misses_L1; // 0 or 1 - read miss in L1
    size_t write_hits_L1; // 0 or 1 - write hit in L1
    size_t write_misses_L1; // 0 or 1 - write miss in L1
    size_t read_hits_L2; // 0 or 1 - read hit in L2
    size_t read_misses_L2; // 0 or 1 - read miss in L2
    size_t write_hits_L2; // 0 or 1 - write hit in L2
    size_t write_misses_L2; // 0 or 1 - write miss in L2
    size_t currentMemoryCycles; // cycles needed to finish currently queued memory writes
} CacheStats;

#endif
