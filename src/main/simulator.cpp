// Lie Leon Alexius
#include <systemc>

// Request and Result struct
// #include "../modules/modules.hpp"
#include "placeHolder.hpp"

// prevent the C++ compiler from mangling the function name
extern "C" {
    // simulation runner
    int run_simulation(
        int cycles, 
        unsigned l1CacheLines, unsigned l2CacheLines, unsigned cacheLineSize, 
        unsigned l1CacheLatency, unsigned l2CacheLatency, unsigned memoryLatency, 
        size_t numRequests, struct Request* requests,
        const char* tracefile
    ) 
    {
        // do simulation and return success (0) or failure (!0)
        CPU_L1_L2 caches(l1CacheLines, l2CacheLines, cacheLineSize, l1CacheLatency, l2CacheLatency, memoryLatency);
        
        
        std::cout << "test" << std::endl;
        // sc_start();
        caches.test_memory();
        // std::cout << caches.test_memory() << std::endl;
        // std::cout << mem.read() << std::endl;
        return 0;
    }
}

// The default sc_main implementation.
int sc_main(int argc, char* argv[])
{
    std::cout << "ERROR" << std::endl;
    return 1;
}