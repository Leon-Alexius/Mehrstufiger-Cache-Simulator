// Lie Leon Alexius
#include <systemc>

// Request and Result struct
// #include "../modules/modules.hpp"
#include "placeHolder.hpp"

// prevent the C++ compiler from mangling the function name
extern "C" {
    /**
     * @brief Runs the cache simulation
     *
     * @param[in] cycles Default value is `1000`.
     * 
     * @return `0` if the simulation is successful, `!0` else
     * 
     * @note Please comment out test(s) if not working. This is used for testing `input_parser`
     * 
     * @warning Not tested yet
     * @bug Not tested yet
     * 
     * @todo Finish dependency modules (Cache, L1, L2)
     * @todo tracefile shouldn't be written if it is "default_trace.vcd"
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
        // CPU_L1_L2 caches(l1CacheLines, l2CacheLines, cacheLineSize, l1CacheLatency, l2CacheLatency, memoryLatency);
        // std::cout << "test" << std::endl;
        // sc_start();
        // caches.test_memory();
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
