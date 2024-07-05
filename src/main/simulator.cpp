// Lie Leon Alexius
#include <systemc>

// Request and Result struct
#include "../modules/modules.hpp"
// #include "placeHolder.hpp"

// prevent the C++ compiler from mangling the function name
extern "C" {
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
    int run_simulation(
        int cycles, 
        unsigned l1CacheLines, unsigned l2CacheLines, unsigned cacheLineSize, 
        unsigned l1CacheLatency, unsigned l2CacheLatency, unsigned memoryLatency, 
        size_t numRequests, struct Request* requests,
        const char* tracefile
    ) 
    {
        CPU_L1_L2 caches(l1CacheLines, l2CacheLines, cacheLineSize, l1CacheLatency, l2CacheLatency, memoryLatency);
        caches.test_L1(cacheLineSize, l1CacheLines, l1CacheLatency);

        return 0;
    }
}

// The default sc_main implementation.
int sc_main(int argc, char* argv[])
{
    std::cout << "ERROR" << std::endl;
    return 1;
}
