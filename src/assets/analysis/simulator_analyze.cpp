// #include <systemc>

// Request and Result struct
#include "../../main/simulator.hpp"
#include "../../modules/modules.hpp"

// prevent the C++ compiler from mangling the function name
extern "C" {
    CPU_L1_L2* caches;

    void initialize_caches(unsigned l1CacheLines, unsigned l2CacheLines, unsigned cacheLineSize, 
        unsigned l1CacheLatency, unsigned l2CacheLatency, unsigned memoryLatency, 
        const char* tracefile,

        // Optimization flags
        unsigned prefetchBuffer, 
        unsigned storebackBuffer,
        unsigned conditionalBuffer) {
        // Initialize the Components        
        caches = new CPU_L1_L2(
            l1CacheLines, l2CacheLines, cacheLineSize, 
            l1CacheLatency, l2CacheLatency, memoryLatency, 
            tracefile, 
            prefetchBuffer, storebackBuffer
        );
        std::cout << "Finish in initializing caches" << std::endl;
    }
    
    /**
     * @brief Runs the cache simulation
     *
     * @param cycles The number of cycles for the simulation. Default value is `1000000`.
     * @param l1CacheLines The number of lines in L1 cache. Default value is `64`.
     * @param l2CacheLines The number of lines in L2 cache. Default value is `256`.
     * @param cacheLineSize The cache line size in bytes. Default value is `64`.
     * @param l1CacheLatency The latency for L1 cache in cycles. Default value is `4`.
     * @param l2CacheLatency The latency for L2 cache in cycles. Default value is `12`.
     * @param memoryLatency The latency for memory access in cycles. Default value is `100`.
     * @param numRequests The number of requests. Default value is `1000`.
     * @param requests A pointer to the array of Request structures.
     * @param tracefile The name of the trace file. Default value is "default_trace.vcd".
     * 
     * @return Result struct
     * 
     * @warning Not tested yet
     * @bug Not tested yet
     * 
     * @authors
     * Lie Leon Alexius
     * Anthony Tang
     */
    struct CacheStats* run_simulation_analyze(
        int cycles,
        size_t numRequests, struct Request* requests
    ) 
    {
        // Test the Requests
        // print_requests(numRequests, requests);

        

        CacheStats* result = (CacheStats*) malloc(sizeof(CacheStats));
        memset(result, 0, sizeof(CacheStats));
        // ========================================================================================
        
        std::cout << "Running simulation..." << std::endl;

        // Process the request
        for (size_t i = 0; i < numRequests; i++) {
            
            struct Request req = requests[i];

            

            // If req.we == -1, end simulation
            if (req.we == -1) {
                break;
            }
            
            // Send request to cache
            
            struct CacheStats tempResult = caches->send_request(req);

            
            result->cycles += tempResult.cycles;
            result->misses += tempResult.misses;
            result->hits += tempResult.hits;
            result->read_hits += tempResult.read_hits;
            result->read_misses += tempResult.read_misses;
            result->write_hits += tempResult.write_hits;
            result->write_misses += tempResult.write_misses;
            result->read_hits_L1 += tempResult.read_hits_L1;
            result->read_misses_L1 += tempResult.read_misses_L1;
            result->write_hits_L1 += tempResult.write_hits_L1;
            result->write_misses_L1 += tempResult.write_misses_L1;
            result->read_hits_L2 += tempResult.read_hits_L2;
            result->read_misses_L2 += tempResult.read_misses_L2;
            result->write_hits_L2 += tempResult.write_hits_L2;
            result->write_misses_L2 += tempResult.write_misses_L2;
        }

        unsigned memory_cycles = caches->finish_memory();
        result->cycles += memory_cycles;

        

        // ========================================================================================
        // assign Result
        
        result->primitiveGateCount = caches->get_gate_count(); // fetch the gate count
        caches->reset_cache();
        // stop the simulation and close the trace file
        // (tracefile != NULL) ? caches.close_trace_file() : caches.stop_simulation();
        
        // return the result
        return result;
    }
    
}

// The default sc_main implementation.
int sc_main(int argc, char* argv[])
{
    std::cout << "ERROR" << std::endl;
    return 1;
}

