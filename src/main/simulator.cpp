// Lie Leon Alexius
#include <systemc>

// Request and Result struct
#include "simulator.hpp"
#include "../modules/modules.hpp"

// prevent the C++ compiler from mangling the function name
extern "C" {
    /**
     * @brief For testing the request
     * 
     * @details
     * "test_valid.csv"
     * Adr: 1 Data: 0 WE: 0
     * Adr: 2 Data: 100 WE: 1
     * Adr: 3 Data: 0 WE: 0
     * Adr: 4 Data: 100 WE: 1
     * Adr: 5 Data: 0 WE: 0
     * Adr: 6 Data: 200 WE: 1
     * Adr: 2 Data: 0 WE: 0
     * Adr: 3 Data: 0 WE: 0
     * Adr: 3 Data: 300 WE: 1
     * Adr: 4 Data: 0 WE: 0
     * Adr: 0 Data: 0 WE: -1
     * 
     * @author Lie Leon Alexius
     */
    void print_requests(size_t numRequests, struct Request* requests) {
        for (size_t i = 0; i < numRequests; i++) {
            struct Request r = requests[i];
            std::cout << "Adr: " << r.addr << " Data: " << r.data << " WE: " << r.we << std::endl;

            // stop printing
            if (r.we == -1) {
                break;
            }
        }  
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
    Result* run_simulation(
        int cycles, 
        unsigned l1CacheLines, unsigned l2CacheLines, unsigned cacheLineSize, 
        unsigned l1CacheLatency, unsigned l2CacheLatency, unsigned memoryLatency, 
        size_t numRequests, struct Request* requests,
        const char* tracefile
    ) 
    {
        // Test the Requests
        // print_requests(numRequests, requests);

        // Initialize the Components        
        CPU_L1_L2 caches(l1CacheLines, l2CacheLines, cacheLineSize, l1CacheLatency, l2CacheLatency, memoryLatency, tracefile);
        size_t cycleCount = 0;
        size_t missCount = 0; 
        size_t hitCount = 0;
        size_t gateCount = 0;

        // ========================================================================================
        
        // Process the request
        for (size_t i = 0; i < numRequests; i++) {
            struct Request req = requests[i];

            // If req.we == -1, end simulation
            if (req.we == -1) {
                break;
            }
            
            // Send request to cache
            Result tempResult = caches.send_request(req);
            cycleCount += tempResult.cycles;
            missCount += tempResult.misses;
            hitCount += tempResult.hits;
            gateCount += tempResult.primitiveGateCount;
        }

        // stop the simulation and close the trace file
        caches.close_trace_file();

        // ========================================================================================

        // assign Result
        Result* result = (Result*) malloc(sizeof(Result));
        result->cycles = cycleCount;
        result->hits = hitCount;
        result->misses = missCount;
        result->primitiveGateCount = gateCount;
        
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
