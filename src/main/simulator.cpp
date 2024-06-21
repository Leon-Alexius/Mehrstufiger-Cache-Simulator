// Lie Leon Alexius
#include <systemc>

// to avoid errors - implement later
#include "placeHolder.hpp"

// simulation runner
int run_simulation(
    int cycles, 
    unsigned l1CacheLines, unsigned l2CacheLines, unsigned cacheLineSize, 
    unsigned l1CacheLatency, unsigned l2CacheLatency, unsigned memoryLatency, 
    size_t numRequests, struct Request* requests,
    const char* tracefile
    ) 
    {
        return 0;
    }

// The default sc_main implementation.
int sc_main(int argc, char* argv[])
{
    std::cout << "ERROR" << std::endl;
    return 1;
}