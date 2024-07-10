// Leon - Memory moved here

#ifndef MEMORY_HPP
#define MEMORY_HPP

#ifdef __cplusplus // added #ifdef __cplusplus so that it works as a c header - anthony
#include <systemc>

#include "../main/simulator.hpp" // the struct moved here - Leon

// using namespace directives won't get carried over. 
using namespace sc_core;
using namespace std;

/**
 * @brief MEMORY represents the main memory in a memory hierarchy system.
 * @details MEMORY handles read and write operations directly from L2 cache.
 * 
 * @note 
 * memory_blocks[] cover all possible memory address, not just 1_000_000 (1 MB)
 * Assume that address is 0x0000.0000 till 0xFFFF.FFFF (4_294_967_295)
 *
 * @author Alexander Anthony Tang
 */
SC_MODULE(MEMORY) {
    sc_in<char*> data_in_from_L2;       // Data input from L2 cache for write (4 Bytes)
    sc_in<char*> data_out_to_L2;        // Data output to L2 cache for read (cacheLineSize Bytes)  
    sc_in<uint32_t> address;            // Address input for read/write operations
    sc_in<bool> write_enable;           // Write-enable signal for a write operation
    sc_in<bool> clock;                  // Clock signal for synchronization

    sc_out<bool> done;                  // Signal indicating the completion of an operation in the current memory
    sc_in<bool> valid_in;               // Whether Signal propagated from L2 to Memory is valid

    char memory_blocks[4294967296];     // Memory blocks represented by an array of char -> canonically 1 Mebibyte = 1048576 Bytes
    unsigned int latency;               // Latency of memory in clock cycles

    unsigned int cacheLineSize;         // Size of each cache line

   /**
    * @brief Constructor for MEMORY module.
    *
    * @param name Name of the module.
    * @param cacheLineSize Size of each cache line.
    * @param memoryLatency Latency of the memory in clock cycles.
    *
    * @author Alexander Anthony Tang
    */
    SC_CTOR(MEMORY);
    MEMORY(sc_module_name name, unsigned int cacheLineSize, unsigned int latency) 
    : sc_module(name), latency(latency), cacheLineSize(cacheLineSize) {
        SC_THREAD(update);
        sensitive << clock.pos();
    }

   /**
    * @brief Main update method for the MEMORY.
    * 
    * @details This method implements the main functionality of the memory, handling read and write operations while maintaining memory coherence.
    * The operations are synchronized with the clock signal.
    * This update() method will wait until the cycle takes 8 ticks, before executing the write/read
    *
    * @author Alexander Anthony Tang
    */
    void update() {
        wait();
        while (true) {
            wait(SC_ZERO_TIME);
            wait(SC_ZERO_TIME);

            

            // mark as not done, and wait for signal from L2 is valid
            done->write(false);
            while(!valid_in->read()) {
                wait();
                wait(SC_ZERO_TIME);
            }

            // Wait to sync with latency
            for (unsigned i = 0; i < latency; i++) {
                wait();
            }

            // get the address
            unsigned int address_u = address->read();

            // Case: Read
            if (!write_enable->read()) {
                // Load the data to the Bus, Load the whole cacheLine
                for (unsigned i = 0; i < cacheLineSize; i++) {
                    data_out_to_L2->read()[i] = memory_blocks[address_u];
                    address_u++;
                }
            } 

            // Case: Write
            else {
                // Write data to memory (in_Bus is 4 Bytes - data is 4 Bytes)
                for (unsigned i = 0; i < 4; i++) {
                    memory_blocks[address_u] = data_in_from_L2->read()[i];
                    address_u++;
                }
            }

            

            // Signal as done, and wait for next clk
            done->write(true);
            wait();
        }
    }
};

#endif
#endif
