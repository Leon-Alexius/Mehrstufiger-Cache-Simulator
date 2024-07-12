// Leon - L2 moved here

#ifndef CACHE_L2_HPP
#define CACHE_L2_HPP

#ifdef __cplusplus // added #ifdef __cplusplus so that it works as a c header - anthony
#include <systemc>
#include <vector>

#include "../main/simulator.hpp" // the struct moved here - Leon

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

    unsigned cacheLineSize;                 // Size of each cache line
    unsigned l2CacheLines;                  // Number of cache lines in the L2 cache
    unsigned l2CacheLatency;                // Latency of L2 cache in clock cycles

    // Optimization - Leon
    unsigned int log2_cacheLineSize = 0;    // log2(cacheLineSize)
    unsigned int log2_l2CacheLines = 0;     // log2(l2CacheLines)

    unsigned int buffer_size;
    sc_fifo<char*>* prefetch_buffer;

    

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
    L2(sc_module_name name, unsigned cacheLineSize, unsigned l2CacheLines, unsigned l2CacheLatency, sc_fifo<char*>* prefetch_buffer) : sc_module(name), cacheLineSize(cacheLineSize), l2CacheLines(l2CacheLines), l2CacheLatency(l2CacheLatency), prefetch_buffer(prefetch_buffer) {
        cache_blocks.resize(l2CacheLines, vector<char> (cacheLineSize));
        valid.resize(l2CacheLines);
        tags.resize(l2CacheLines);
        
        // Optimization - Leon
        while ((cacheLineSize >>= 1) > 0) {
            log2_cacheLineSize++;
        }
        while ((l2CacheLines >>= 1) > 0) {
            log2_l2CacheLines++;
        }

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
            wait(SC_ZERO_TIME);
            wait(SC_ZERO_TIME);
           
            done->write(false);
            hit->write(false);

            // wait until L1's signal is valid
            while (!valid_in->read()) {
                wait();
            }

            unsigned address_int = address->read();

            // extracts metadata bits from address - optimized (see L1)
            unsigned int offset = address_int & (cacheLineSize - 1);
            unsigned int index = (address_int >> log2_cacheLineSize) & (l2CacheLines - 1);
            unsigned int tag = address_int >> (log2_cacheLineSize + log2_l2CacheLines);

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

                // Wait until RAM is done, mark as invalid propagation
                while (!done_from_Mem->read()) {
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

                // Read miss, propagate to mem
                else 
                {
                    // Signal to RAM, then mark as valid propagation
                    address_out->write(address->read()); 
                    write_enable_out->write(write_enable->read());
                    valid_out->write(true);

                    // Wait until RAM is done
                    while (!done_from_Mem->read()) {
                        wait();
                    }
                    valid_out->write(false);
                    
                    // Write the data from RAM to the appropriate CacheLine
                    // Data that is sent by RAM is a whole cacheLine
                    for (unsigned i = 0; i < cacheLineSize; i++) {
                        cache_blocks[index][i] = data_in_from_Mem->read()[i];
                    }
                    valid[index] = true; // set data is valid
                    tags[index] = tag; // update tag


                                        //Optimization: Prefetching - Trang
                    //Prefetching - load 4 cache lines starting from address
                    for (int i = 0; i < 4; i++)
                    {
                        std::cout << "Fetching line " << i << std::endl;
                        prefetch_next_line(address_int + i*(cacheLineSize));
                    }
                    
                    //load the prefetched lines into cache
                    if (prefetch_buffer != nullptr)
                    {
                        int j = 1;
                        
                        while (prefetch_buffer->num_available() > 0) {
                            char* prefetch_data = new char[cacheLineSize];
                            uint32_t address_new = address_new + j*cacheLineSize;
                            unsigned int index_new = (address_new >> log2_cacheLineSize) & (l2CacheLines - 1);
                            unsigned int tag_new = address_new >> (log2_cacheLineSize + log2_l2CacheLines);
                            //assign the next line to prefetch_data
                            std::cout << "waiting" << std::endl;
                            prefetch_buffer->read(prefetch_data);
                            //load the line stored in prefetch_data into cache
                            for (unsigned i = 0; i < cacheLineSize; i++) {
                                cache_blocks[index_new][i] = prefetch_data[i];
                                std::cout << "0";
                            }
                            std::cout << std::endl;
                            tags[index_new] = tag_new;
                            valid[index_new] = true;
                            j++;
                        }
                    }
                    

                }

                //bring the read data back to L1
                for (unsigned i = 0; i < 4; i++) {
                    data_out_to_L1->read()[i] = cache_blocks[index][i];
                }
            }

            // Delays up to latency L2 (-1 for done.write(true))
            for (unsigned i = 0; i < l2CacheLatency - 1; i++) {
                wait();
            }
            done->write(true); // signal as done
            wait(); // wait for next clk event
        }
    }

    //load the next line into the buffer - Trang
     void prefetch_next_line(uint32_t address_of_line) {
        address_out->write(address_of_line);
        write_enable_out->write(false);
        valid_out->write(true);

        wait(SC_ZERO_TIME);
        wait(SC_ZERO_TIME);
        while (!done_from_Mem->read()) {
            wait();
            wait(SC_ZERO_TIME);
            wait(SC_ZERO_TIME);
            std::cout << "WAITING FOR MEM " << std::endl;
        }
        valid_out->write(false);

        char* prefetched_line = new char[cacheLineSize];
        for (unsigned i = 0; i < cacheLineSize; i++) {
            prefetched_line[i] = data_in_from_Mem->read()[i];
        }
        prefetch_buffer->write(prefetched_line);
    }

        int test_L2() {
        sc_fifo<char*>* fifo = new sc_fifo<char*>(4);
        L2 l2("l1cache", 64,4,1, fifo);
        char data[64] = {'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd',
        'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd',
        'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd',
        'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd'};
        
        // data_in signal
        sc_signal<char> in[64];
        
        // data_out signal
        sc_signal<char> out[64];
        
        // Address
        sc_signal<uint32_t> address;

        
        // Clock
        sc_clock clk("clk", 1, SC_SEC);
        
        // Write enable signal
        sc_signal<bool> we;
        we = false;

        // cacheLineSize =64
        for (int i = 0; i < 64; i++) {
            in[i] = data[i];
            l2.data_in_from_L1->read()[i]=(in[i]);
        }

        // Bind each data_out signal to out signal
        for (int i = 0; i < 64; i++) {
            l2.data_out_to_Mem->read()[i]=(out[i]);
        }


        // Bind the address, the clock, and the write enable
        l2.address(address);
        l2.clk(clk);
        l2.write_enable(we);

        // Start for 1 cycle to let write take place
        sc_start(1, SC_SEC);

        // Begin read by setting write_enable to false
        we = false;

        // Start simulation for 1 cycle
        sc_start(1, SC_SEC);
        
        // Read from the out signals
        for (int i = 0; i < 64; i++) {
            std::cout << out[i];
        }
        std::cout << std::endl;

        return 0;
      }
    

};

#endif
#endif