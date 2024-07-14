// Leon - L1 moved here

#ifndef CACHE_L1_HPP
#define CACHE_L1_HPP

#ifdef __cplusplus // added #ifdef __cplusplus so that it works as a c header - anthony
#include <systemc>
#include <vector>

#include "../main/simulator.hpp" // the struct moved here - Leon

// using namespace directives won't get carried over. 
using namespace sc_core;
using namespace std;

/**
* @brief L1 represents a L1 cache in a memory hierarchy system
* @details This L1 module handles read and write operations to and from CPU and L2 cache
* its write miss policy is no-write-allocate, no-fetch-on-write, and no-write-before-hit
* its write hit policy is write-through
* 
* @author Van Trang Nguyen
*/
SC_MODULE(L1){

    sc_in<char*> data_in_from_CPU;      // Data input from CPU for write
    sc_in<char*> data_in_from_L2;       // Data input from L2 cache to L1

    sc_in<uint32_t> address;            // Address input for read/write 
    sc_out<uint32_t> address_out;       // Address propagated to L2

    sc_in<bool> write_enable;           // Write-enable signal for a write operation
    sc_out<bool> write_enable_out;      // Write-enable signal propagated to L2 cache

    sc_in<char*> data_out_to_CPU;       // Read data to the CPU
    sc_in<char*> data_out_to_L2;        // Data propagated from CPU to the L2 cache
    
    sc_out<bool> hit;                   // Cache hit signal
    sc_in<bool> done_from_L2;           // Signal indicating the completion operation in L2
    sc_in<bool> clk;                    // Clock signal for synchronization

    sc_out<bool> done;                  // Signal indicating the completion of an operation in the current L1
    
    sc_in<bool> valid_in;               // valid marker from CPU
    sc_out<bool> valid_out;             // marks if the command propagated to L2 is valid
    
    vector<vector<char>> cache_blocks;  // Cache blocks represented in a 2D vector
    
    vector<bool> valid;                 // Vector indicating the validity of cache lines
    vector<uint32_t> tags;              // Vector storing the tags for each cache line

    unsigned cacheLineSize;             // Size of each cache line
    unsigned l1CacheLines;              // Number of cache lines in the L1 cache
    unsigned l1CacheLatency;            // Latency of L1 cache in clock cycles

    // Optimization - Leon
    unsigned int log2_cacheLineSize = 0;    // log2(cacheLineSize)
    unsigned int log2_l1CacheLines = 0;     // log2(l1CacheLines)

    /**
     * @brief Constructor for L1 cache module.
     *
     * @param name The name of the module.
     * @param cacheLineSize The size of each cache line.
     * @param l1CacheLines The number of cache lines in the L1 cache.
     * @param l1CacheLatency The latency of the L1 cache in clock cycles.
     *
     * @authors 
     * Van Trang Nguyen
     * Lie Leon Alexius
     */
    SC_CTOR(L1);
    L1(sc_module_name name, unsigned cacheLineSize, unsigned l1CacheLines, unsigned l1CacheLatency) : sc_module(name), cacheLineSize(cacheLineSize), l1CacheLines(l1CacheLines), l1CacheLatency(l1CacheLatency) {
        cache_blocks.resize(l1CacheLines, vector<char> (cacheLineSize));
        valid.resize(l1CacheLines);
        tags.resize(l1CacheLines);

        /*
            Optimization - Leon
            log2() is equivalent as shifting n times

            Ex: 0001.0000 (16) returns 4
            0000.1000 - 1
            0000.0100 - 2
            0000.0010 - 3
            0000.0001 - 4
            0000.0000 - stop
        */
        while ((cacheLineSize >>= 1) > 0) {
            log2_cacheLineSize++;
        }
        while ((l1CacheLines >>= 1) > 0) {
            log2_l1CacheLines++;
        }

        /*
            Use SC_THREAD() instead of C_THREAD() because:
            C_THREAD() wait(SC_ZERO_TIME) waits for 1 Cycle not 1 Delta + Depracated
        */
        SC_THREAD(update);
        sensitive << clk.pos();
    };

    /**
     * @brief Main update method for the L1 cache.
     * @details 
     * This method implements the main functionality of the cache, 
     *      which is handling read and write operations while maintaining cache coherence.
     * The operations are synchronized with the clock signal.
     */
    void update(){
        wait(); // wait for next clk event (refer to SC_START(ZERO) in CPU_L1_L2)
        // wait();
        while (true)
        {
            
            
            hit->write(false);
            done->write(false);
            
            wait(SC_ZERO_TIME);
            wait(SC_ZERO_TIME);
            
            // wait until cpu's signal is valid
            while (!valid_in->read()) {
                wait();
            }
            
            unsigned int address_int = address->read();

            /*
                extracts metadata bits from address
                using bit operations, because cache line size and number of cache lines are always power of 2

                Here log is log base 2:
                - Offset = log(cache_line_size)
                - Index = log(cache_line_count)
                - Tag = Address_Bit_Count - Index - Offset

                Example: (32 bits Address Size, 64 Byte cacheLineSize, 64 cacheLines)
                1111.0101.1010.1100.0110.0000.0101.1010
                - Offset = 01.1010 (log(64) = 6; 63 = 11.1111)
                - Index = 0000.01 (same reason)
                - Tag = 1111.0101.1010.1100.0110 (rest of bits)

                Optimized - Leon (with log)
                Should be faster because we don't calculate log2() every call to update()
                1. index was (address_int >> int(log2(cacheLineSize))) & (l1CacheLines - 1);
                2. tag was address_int >> (log2_cacheLineSize + log2_l1CacheLines);
            */
            unsigned int offset = address_int & (cacheLineSize - 1);
            unsigned int index = (address_int >> log2_cacheLineSize) & (l1CacheLines - 1);
            unsigned int tag = address_int >> (log2_cacheLineSize + log2_l1CacheLines);
            
            // Tags and Data is only accessible after l1 latency cycles.
            for (unsigned i = 0; i < l1CacheLatency; i++) {
                wait();
            }

            // write operation
            if (write_enable->read()){
                
                // write hit, write through
                if ((tags[index] == tag) && (valid[index]))
                {   
                    hit->write(true);
                    // write the input data to the matching cacheline
                    for (int i = 0; i < 4; i++){
                        cache_blocks[index][i + offset] = data_in_from_CPU->read()[i];
                    }
                }

                // no matter write miss or write hit, propagate to L2
                for (int i = 0; i < 4; i++) {
                    data_out_to_L2->read()[i] = data_in_from_CPU->read()[i];
                }
                
                // Signal to L2, then mark as valid propagation
                address_out->write(address->read());
                write_enable_out->write(write_enable->read());
                valid_out->write(true);

                // Wait until L2 is done, mark as invalid propagation
                while (!done_from_L2->read()) {
                    wait();
                }
                valid_out->write(false);
            } 

            // read operation
            else {

                // cache hit
                if (valid[index] && tags[index]==tag)
                {
                    hit->write(true);                    
                }

                // Read miss, propagate to L2, load cacheline from L2 to L1, and write to data_out_to_CPU
                else {
                    // Signal to L2, then mark as valid propagation
                    address_out->write(address->read());
                    write_enable_out->write(write_enable->read());
                    valid_out->write(true);

                    // Wait until L2 is done
                    wait(SC_ZERO_TIME);
                    while (!done_from_L2->read()) {
                        wait();
                        wait(SC_ZERO_TIME);
                        wait(SC_ZERO_TIME);
                        wait(SC_ZERO_TIME);
                        wait(SC_ZERO_TIME);
                        // std::cout << "DONE FROM L2 " << done_from_L2->read() << std::endl;
                    }
                    valid_out->write(false);



                    // Write the data to the appropriate CacheLine
                    // Data that is sent by L2 is a whole cacheLine
                    for (unsigned i = 0; i < cacheLineSize; i++) {
                        cache_blocks[index][i] = data_in_from_L2->read()[i];
                    }
                    valid[index] = true; // set data is valid
                    tags[index] = tag; // update tag
                }

                // Write data to bus for the CPU (4 Bytes)
                for (unsigned i = 0; i < 4; i++) {
                    data_out_to_CPU->read()[i] = cache_blocks[index][i + offset];
                }
            }

            
            done->write(true); // signal as done
            // std::cout << "DONE FROM L1 " << sc_time_stamp().to_seconds() << std::endl;
            // wait(SC_ZERO_TIME);
            // wait(SC_ZERO_TIME);
            wait();
            
            // wait(); // wait for next clk event
           
        }
    }
};

#endif
#endif
