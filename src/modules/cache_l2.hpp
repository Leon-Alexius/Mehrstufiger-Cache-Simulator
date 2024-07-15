// Leon - L2 moved here

#ifndef CACHE_L2_HPP
#define CACHE_L2_HPP

#ifdef __cplusplus // added #ifdef __cplusplus so that it works as a c header - anthony
#include <systemc>
#include <vector>

#include "../main/simulator.hpp" // the struct moved here - Leon
#include "storeback_buffer.hpp"
#include "prefetch_buffer.hpp"

// using namespace directives won't get carried over. 
using namespace sc_core;
using namespace std;

/**
 * @brief L2 represents an L2 cache in a memory hierarchy system.
 * @details L2 handles read and write operations to and from L1 cache and main memory.
 *
 * @author Van Trang Nguyen
 */
SC_MODULE(L2){
    sc_in<char*> data_in_from_L1;           // Data input from L1 cache for write
    sc_in<char*> data_in_from_Mem;          // Data input from memory to L2

    sc_in<uint32_t> address;                // Address input for read/write operations
    sc_out<uint32_t> address_out;           // Address propagated to memory

    sc_in<bool> write_enable;               // Write-enable signal for a write operation
    sc_out<bool> write_enable_out;          // Write-enable signal propagated to memory

    sc_in<char*> data_out_to_L1;            // Data output to L1 cache
    sc_in<char*> data_out_to_Mem;           // Data output to memory

    sc_out<bool> hit;                       // Cache hit signal
    sc_in<bool> done_from_Mem;              // Signal indicating the completion of an operation in memory
    sc_in<bool> clk;                        // Clock signal for synchronization

    sc_out<bool> done;                      // Signal indicating the completion of an operation in L2 cache
    sc_in<bool> valid_in;                   // valid marker from L1
    sc_out<bool> valid_out;                 // marks if the command propagated to RAM is valid

    vector<vector<char>> cache_blocks;      // A 2D vector representing the cache blocks

    vector<bool> valid;                     // Vector indicating the validity of cache lines
    vector<uint32_t> tags;                  // Vector storing the tags for each cache line

    // We will use a WTCB (Write Through with Conditional Flush buffer)
    STOREBACK* storeback;
    PREFETCH* prefetch;


    unsigned cacheLineSize;                 // Size of each cache line
    unsigned l2CacheLines;                  // Number of cache lines in the L2 cache
    unsigned l2CacheLatency;                // Latency of L2 cache in clock cycles

    // Optimization - Leon
    unsigned int log2_cacheLineSize = 0;    // log2(cacheLineSize)
    unsigned int log2_l2CacheLines = 0;     // log2(l2CacheLines)
    unsigned int power_of_two = 1;
    unsigned int buffer_size;
    

    

   /**
    * @brief Constructor for L2 cache module.
    *
    * @param name The name of the module.
    * @param cacheLineSize The size of each cache line.
    * @param l2CacheLines The number of cache lines in the L2 cache.
    * @param l2CacheLatency The latency of the L2 cache in clock cycles.
    *
    * @authors 
    * Van Trang Nguyen
    * Lie Leon Alexius
    */
    SC_CTOR(L2);
    L2(sc_module_name name, unsigned cacheLineSize, unsigned l2CacheLines, unsigned l2CacheLatency, PREFETCH* prefetch, STOREBACK* storeback) : sc_module(name), cacheLineSize(cacheLineSize), l2CacheLines(l2CacheLines), l2CacheLatency(l2CacheLatency), storeback(storeback), prefetch(prefetch) {
        cache_blocks.resize(l2CacheLines, vector<char> (cacheLineSize));
        valid.resize(l2CacheLines);
        tags.resize(l2CacheLines);
        
        
        // Optimization - Leon
        while ((cacheLineSize >>= 1) > 0) {
            log2_cacheLineSize++;
        }
        l2CacheLines -= 1;
        while ((l2CacheLines >>= 1) > 0) {
            log2_l2CacheLines++;
        }
        log2_l2CacheLines++;

        power_of_two <<= log2_l2CacheLines;

        SC_THREAD(update);
        sensitive << clk.pos();
    }

   /**
    * @brief Main update method for the L2 cache.
    * @details This method implements the main functionality of the cache, handling read and write operations while maintaining cache coherence.
    * The operations are synchronized with the clock signal.
    */
    void update(){
        wait(); // wait for next clk event
        while (true) { 
            
            
           
            done->write(false);
            hit->write(false);

            wait(SC_ZERO_TIME);
            wait(SC_ZERO_TIME);

            // wait until L1's signal is valid
            while (!valid_in->read()) {
                wait();
                wait(SC_ZERO_TIME);
            }

            unsigned address_int = address->read();

            // extracts metadata bits from address - optimized (see L1)
            unsigned int offset = address_int & (cacheLineSize - 1);
            unsigned int index = ((address_int >> log2_cacheLineSize) & (power_of_two - 1)) % (l2CacheLines);
            unsigned int tag = address_int >> (log2_cacheLineSize + log2_l2CacheLines - (power_of_two != l2CacheLines));

            // Tags and data is only accessible after l2 latency cyles
            // Here it is -1 so that the simulation logic stays consistent -
            // the data is available after the wait() in the very end, so + 1.
            for (unsigned i = 0; i < l2CacheLatency; i++) {
                wait();
            }
            
            // write operation
            if(write_enable->read()){
                
                // write hit, write through
                if (tags[index] == tag && valid[index])
                {
                    hit->write(true);
                    // write the input data to the matching cacheline 
                    for (unsigned i=0; i<4;i++){
                        cache_blocks[index][i+offset]= data_in_from_L1->read()[i];
                    }
                }
                
                //no matter write miss or hit, continues to propagate to Memory
                for (unsigned i = 0; i < 4; i++){    
                    data_out_to_Mem->read()[i] = data_in_from_L1->read()[i];
                }

                // Signal to RAM, then mark as valid propagation
                address_out->write(address->read());
                write_enable_out->write(write_enable->read());
                valid_out->write(true);



                if (storeback != nullptr) {
                    char* new_data = new char[4]();
                    //write to buffer
                    for (unsigned i = 0; i < 4; i++){    
                        new_data[i] = data_out_to_Mem->read()[i];
                    }
                    while (!storeback->write(new_data, address, (address_int >> log2_cacheLineSize))) {
                        wait(SC_ZERO_TIME);
                        wait();
                        if (!valid_in->read()) break;
                    }
                } else {
                    // Wait until RAM is done, mark as invalid propagation
                    while (!done_from_Mem->read()) {
                        wait();
                        wait(SC_ZERO_TIME);
                        wait(SC_ZERO_TIME);
                    }
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

                // Read miss, propagate to mem
                else 
                {
                    // If there is a storeback buffer -> check the tag in the storeback buffer if the tag is there or not
                    if (storeback != nullptr && storeback->in_buffer((address_int >> log2_cacheLineSize))) {
                        // If yes, flush all contents of the buffer into the memory
                        // NOTE: We can also flush the data with the same tag, while leaving the others,
                        // but this overcomplicates the structure of the buffer and will not make it
                        // FIFO again.
                        while (!done_from_Mem->read()) {
                            wait();
                            wait(SC_ZERO_TIME);
                            wait(SC_ZERO_TIME);
                        }
                    }
                    valid_out->write(true);


                    // Signal to RAM, then mark as valid propagation
                    address_out->write(address->read()); 
                    write_enable_out->write(write_enable->read());

                    // Wait until RAM is done
                    while (!done_from_Mem->read()) {
                        wait();
                        wait(SC_ZERO_TIME);
                        wait(SC_ZERO_TIME);
                    }
                    
                    
                    
                    valid_out->write(false);

                    
                    
                    // Write the data from RAM to the appropriate CacheLine
                    // Data that is sent by RAM is a whole cacheLine
                    for (unsigned i = 0; i < cacheLineSize; i++) {
                        cache_blocks[index][i] = data_in_from_Mem->read()[i];
                    }
                    valid[index] = true; // set data is valid
                    tags[index] = tag; // update tag

                    
                    //load the prefetched lines into cache
                    if (prefetch != nullptr) {
                        read_from_prefetch();
                    }
                    
                }

                //bring the read data back to L1
                for (unsigned i = 0; i < 4; i++) {
                    data_out_to_L1->read()[i] = cache_blocks[index][i];
                }
            
            }

            
            
            done->write(true); // signal as done
            wait(SC_ZERO_TIME);
            wait(SC_ZERO_TIME);
            wait(); // wait for next clk event
        }
    }

    void read_from_prefetch() {

        char* data;
        uint32_t address_u;

        for (int i = 0; i < prefetch->capacity; i++) {
            
            // If no write was underway, then read from buffer. But if the buffer is empty, then
            // memory has finished its task and will await further instructions.

            while (!prefetch->read(data, address_u)) {
                wait();
                wait(SC_ZERO_TIME);
                wait(SC_ZERO_TIME);
                std::cout << sc_time_stamp().to_seconds() << std::endl;
            }

            uint32_t address_new = address_u;
            unsigned int index_new = ((address_new >> log2_cacheLineSize)) % (l2CacheLines);
            unsigned int tag_new = address_new >> (log2_cacheLineSize + log2_l2CacheLines);
            
            // Write to memory
            for (unsigned i = 0; i < cacheLineSize; i++) {
                cache_blocks[index_new][i] = data[i];
            }

            tags[index_new] = tag_new;
            valid[index_new] = true;

            // Free the pointer from data
            delete[] data;
        }
        
        return;
    }

    

    
    
};

#endif
#endif
