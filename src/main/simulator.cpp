// Lie Leon Alexius

#include <systemc>
#include "simulator.hpp"
#include "../modules/modules.hpp"

// prevent the C++ compiler from mangling the function name
extern "C" {
    #include "parser/parse.h"
    #include "grapher/printer.h"

    // custom config variable
    Config* config = NULL;

    /**
     * @brief Set the config
     * @author Lie Leon Alexius
     */
    void set_config(Config* c) {
        config = c;
    }
    
    /**
     * @brief updates the CacheStats
     * @note ignores `primitiveGateCount`
     * @author Lie Leon Alexius
     */
    void statsUpdater(CacheStats* cacheStats, CacheStats tempStats) {
        cacheStats->cycles += tempStats.cycles;
        cacheStats->misses += tempStats.misses;
        cacheStats->hits += tempStats.hits;
        cacheStats->read_hits += tempStats.read_hits;
        cacheStats->read_misses += tempStats.read_misses;
        cacheStats->write_hits += tempStats.write_hits;
        cacheStats->write_misses += tempStats.write_misses;
        cacheStats->read_hits_L1 += tempStats.read_hits_L1;
        cacheStats->read_misses_L1 += tempStats.read_misses_L1;
        cacheStats->write_hits_L1 += tempStats.write_hits_L1;
        cacheStats->write_misses_L1 += tempStats.write_misses_L1;
        cacheStats->read_hits_L2 += tempStats.read_hits_L2;
        cacheStats->read_misses_L2 += tempStats.read_misses_L2;
        cacheStats->write_hits_L2 += tempStats.write_hits_L2;
        cacheStats->write_misses_L2 += tempStats.write_misses_L2;
    }

    /**
     * @brief Runs the cache simulation
     * 
     * @note This function will try to print the result of the simulation
     *
     * @param cycles The number of cycles for the simulation.
     * @param l1CacheLines The number of lines in L1 cache.
     * @param l2CacheLines The number of lines in L2 cache.
     * @param cacheLineSize The cache line size in bytes.
     * @param l1CacheLatency The latency for L1 cache in cycles.
     * @param l2CacheLatency The latency for L2 cache in cycles.
     * @param memoryLatency The latency for memory access in cycles.
     * @param numRequests The number of requests.
     * @param requests A pointer to the array of Request structures.
     * @param tracefile The name of the trace file.
     * 
     * @return Result struct
     * 
     * @author Lie Leon Alexius
     */
    Result run_simulation(
        int cycles, 
        unsigned l1CacheLines, unsigned l2CacheLines, unsigned cacheLineSize, 
        unsigned l1CacheLatency, unsigned l2CacheLatency, unsigned memoryLatency, 
        size_t numRequests, struct Request* requests,
        const char* tracefile
    ) 
    {
        // Get the config if any (Optimization flags)
        unsigned int prefetchBuffer = 0; 
        unsigned int storebackBuffer = 0; 
        bool storebackBufferCondition= false;

        if (config != NULL) {
            prefetchBuffer = config->prefetchBuffer;
            storebackBuffer = config->storebackBuffer;
            storebackBufferCondition = config->storebackBufferCondition;
        }

        // Initialize the cache simulator       
        CPU_L1_L2 caches(
            l1CacheLines, l2CacheLines, cacheLineSize, 
            l1CacheLatency, l2CacheLatency, memoryLatency, 
            tracefile, 
            prefetchBuffer, storebackBuffer, storebackBufferCondition
        );

        // Initialize the cacheStats
        CacheStats* cacheStats = (CacheStats*) malloc(sizeof(CacheStats));
        cacheStats->cycles = 0;
        cacheStats->misses = 0;
        cacheStats->hits = 0;
        cacheStats->primitiveGateCount = 0;
        cacheStats->read_hits = 0;
        cacheStats->read_misses = 0;
        cacheStats->write_hits = 0;
        cacheStats->write_misses = 0;
        cacheStats->read_hits_L1 = 0;
        cacheStats->read_misses_L1 = 0;
        cacheStats->write_hits_L1 = 0;
        cacheStats->write_misses_L1 = 0;
        cacheStats->read_hits_L2 = 0;
        cacheStats->read_misses_L2 = 0;
        cacheStats->write_hits_L2 = 0;
        cacheStats->write_misses_L2 = 0;
        cacheStats->currentMemoryCycles = 0;

        // ========================================================================================

        // Cycle Limit Counter
        int original_cycles = cycles;

        // Flag: true if the simulator stopped due to exceeding the cycle limit
        bool simulatorForceTerminate = false;

        // Process the request
        for (size_t i = 0; i < numRequests; i++) {
            struct Request req = requests[i];

            // If req.we == -1, end simulation
            if (req.we == -1) {
                break;
            }
            
            // Send request to cache
            CacheStats tempResult = caches.send_request(req, original_cycles);

            // break if cycles already exceeded the limit
            if (original_cycles < 0) {
                simulatorForceTerminate = true;
                break;
            }

            // update the cacheStats
            statsUpdater(cacheStats, tempResult);
        }

        // Finish up the simulation (wait for memory write) if the simulator is not forced to terminate
        if (!simulatorForceTerminate) {
            unsigned int memory_cycles = caches.finish_memory(original_cycles);
            if (original_cycles < 0) {
                cacheStats->cycles = SIZE_MAX; 
            }
            else {
                cacheStats->cycles += memory_cycles;
            }
        }
        else {
            // if forced to stop, cycles need to be SIZE_MAX
            cacheStats->cycles = SIZE_MAX;
        }

        // Get gateCount
        cacheStats->primitiveGateCount = caches.get_gate_count();

        // stop the simulation and close the trace file
        (tracefile != NULL) ? caches.close_trace_file() : caches.stop_simulation();

        // ========================================================================================

        // build the Result
        Result result;
        result.cycles = cacheStats->cycles;
        result.hits = cacheStats->hits;
        result.misses = cacheStats->misses;
        result.primitiveGateCount = cacheStats->primitiveGateCount;

        // Print the simulation result
        if (config == NULL) {
            // Case this function is not called by "executor.c"
            config = (Config*) malloc(sizeof(Config));

            // check if allocation successful (else, skip printing output)
            if (config == NULL) {
                fprintf(stderr, "Memory allocation for Config failed, printing the result skipped\n");
                goto instant;
            }

            // create config
            config->cycles = cycles;
            config->l1CacheLines = l1CacheLines;
            config->l2CacheLines = l2CacheLines;
            config->cacheLineSize = cacheLineSize;
            config->l1CacheLatency = l1CacheLatency;
            config->l2CacheLatency = l2CacheLatency;
            config->memoryLatency = memoryLatency;
            config->numRequests = numRequests;
            config->tracefile = NULL;
            config->input_filename = NULL;
            config->requests = NULL;
            config->customNumRequest = true;
            config->prefetchBuffer = 0;
            config->storebackBuffer = 0;
            config->storebackBufferCondition = false;
            config->prettyPrint = true;

            // print the layout
            print_layout(config, cacheStats);
        }
        else {
            // This function is called by "executor.c"
            // print the layout
            print_layout(config, cacheStats);

            // Extra cleanup
            free(config->requests);
            config->requests = NULL;
        }

        // Cleanup Standard
        free(cacheStats);
        cacheStats = NULL;
        free(config);
        config = NULL;

        instant:        
        return result; // return the result
    }
}

/**
 * @brief The default sc_main implementation.
 * @author Lie Leon Alexius
 */
int sc_main(int argc, char* argv[])
{
    std::cout << "TEAM 150 - BUSY NOW" << std::endl;
    return 1;
}
