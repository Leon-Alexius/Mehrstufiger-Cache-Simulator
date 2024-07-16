// Lie Leon Alexius

#include "printer.h"

/**
 * @brief Prints the result and layout of the simulator (if `pretty_print` flag is `true`)
 * @author Lie Leon Alexius
 */
void print_layout(Config* config, CacheStats* cacheStats) {
    if (config->prettyPrint) {
        printf(
            "Team 150 - Cache Simulator\n"
            "An Overview of our simulation:\n\n"
            "┌────────────────────────────────────────────────────────────────┐\n"
            "|                            Processor                           |\n"
            "| ┌────────────────────────────────────────────────────────────┐ |\n"
            "| | Max Simulated Cycles: %-36d | |\n"
            "| | Cache Line Size: %-41d | |\n"
            "| └────────────────────────────────────────────────────────────┘ |\n"
            "| ┌────────────────────────────────────────────────────────────┐ |\n"
            "| |                          L1 Cache                          | |\n"
            "| | Lines: %-8d            | Latency: %-7d              | |\n"
            "| | Read Hits: %-8zu        | Read Misses: %-7zu          | |\n"
            "| | Write Hits: %-8zu       | Write Misses: %-7zu         | |\n"
            "| └────────────────────────────────────────────────────────────┘ |\n"
            "| ┌────────────────────────────────────────────────────────────┐ |\n"
            "| |                          L2 Cache                          | |\n"
            "| | Lines: %-8d            | Latency: %-7d              | |\n"
            "| | Read Hits: %-8zu        | Read Misses: %-7zu          | |\n"
            "| | Write Hits: %-8zu       | Write Misses: %-7zu         | |\n"
            "| └────────────────────────────────────────────────────────────┘ |\n"
            "| Number of Requests Processed: %-32zu |\n"
            "└────────────────────────────────┬───────────────────────────────┘\n"
            "                                 ↓                                \n"
            "┌────────────────────────────────────────────────────────────────┐\n"
            "|                           Buffers                              |\n"
            "| ┌────────────────────────────────────────────────────────────┐ |\n"
            "| | Prefetch Buffer: %-10d  |                             | |\n"
            "| | Storeback Buffer: %-10d | Conditional: %-14d | |\n"
            "| └────────────────────────────────────────────────────────────┘ |\n"
            "└────────────────────────────────┬───────────────────────────────┘\n"
            "                                 ↓                                \n"
            "┌────────────────────────────────────────────────────────────────┐\n"
            "|                               RAM                              |\n"
            "| Memory Latency: %-46d |\n"
            "| Number of RAM Requests: %-38zu |\n"
            "| Number of Read Requests: %-37zu |\n"
            "| Number of Write Requests: %-36zu |\n"
            "└────────────────────────────────────────────────────────────────┘\n\n",
            config->cycles, 
            config->cacheLineSize,
            config->l1CacheLines, config->l1CacheLatency, 
            cacheStats->read_hits_L1, cacheStats->read_misses_L1,
            cacheStats->write_hits_L1, cacheStats->write_misses_L1,
            config->l2CacheLines, config->l2CacheLatency, 
            cacheStats->read_hits_L2, cacheStats->read_misses_L2,
            cacheStats->write_hits_L2, cacheStats->write_misses_L2,
            config->numRequests,
            config->prefetchBuffer, config->storebackBuffer, config->storebackBufferCondition,
            config->memoryLatency, 
            (cacheStats->read_misses_L2 + cacheStats->write_hits + cacheStats->write_misses),
            cacheStats->read_misses_L2, 
            (cacheStats->write_hits + cacheStats->write_misses)
        );
    }

    printf("Number of Cycles Simulated: %zu \n", cacheStats->cycles);
    printf("Number of Cache Hits: %zu   \n", cacheStats->hits);
    printf("Number of Cache Misses: %zu \n", cacheStats->misses);
    printf("Number of Gates: %zu  \n", cacheStats->primitiveGateCount);
}
