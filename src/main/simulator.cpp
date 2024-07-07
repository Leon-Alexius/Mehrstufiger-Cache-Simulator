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
     */
    void print_result(size_t numRequests, struct Request* requests) {
        for (size_t i = 0; i < numRequests; i++) {
            struct Request r = requests[i];
            std::cout << "Adr: " << r.addr << " Data: " << r.data << " WE: " << r.we << std::endl;
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
     * @return `0` if the simulation is successful, `!0` else
     * 
     * @warning Not tested yet
     * @bug Not tested yet
     * 
     * @todo tracefile shouldn't be written if it is "default_trace.vcd"
     * 
     * @author Lie Leon Alexius
     */
    struct Result run_simulation(
        int cycles, 
        unsigned l1CacheLines, unsigned l2CacheLines, unsigned cacheLineSize, 
        unsigned l1CacheLatency, unsigned l2CacheLatency, unsigned memoryLatency, 
        size_t numRequests, struct Request* requests,
        const char* tracefile
    ) 
    {
        /*  Don't delete this! - Leon

            1. Read the struct Request, do the simulation
            End simulation is when you meet Request with {.we = -1}

            2. Assign result to the struct Result

            3. write tracefile IF AND ONLY IF filename is NOT "src/assets/vcd/default_trace.vcd"

            4. Don't forget to print Result and free Result that is made here. 
        */
        
        CPU_L1_L2 caches(l1CacheLines, l2CacheLines, cacheLineSize, l1CacheLatency, l2CacheLatency, memoryLatency, tracefile);
        // caches.test_L1(64, 4, 1);
        size_t cycle_count = 0;
        size_t miss_count = 0; 
        size_t hit_count = 0;
        
        
        for (int i = 0; i < numRequests; i++) {
            Request req = requests[i];
            std::cout << "Request num: " << i << std::endl;

            // If req.we == -1, end simulation
            if (req.we == -1) {
                break;
            }
            
            // Send request to cache
            Result res = caches.send_request(req);
            cycle_count += res.cycles;
        // print_result(numRequests, requests);

            miss_count += res.misses;
            hit_count += res.hits;
        }

        int gate_count = caches.get_gate_count();
        std::cout << "Cycles: " << cycle_count << std::endl;
        caches.close_trace_file();

        struct Result result = {cycle_count, 0, 0, 0};
        
        
        return result;
    }
}

// The default sc_main implementation.
int sc_main(int argc, char* argv[])
{
    std::cout << "ERROR" << std::endl;
    return 1;
}
