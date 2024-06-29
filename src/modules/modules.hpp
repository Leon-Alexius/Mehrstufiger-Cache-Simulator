// temporary struct
#ifndef MODULES_HPP
#define MODULES_HPP

/* anthony
added #ifdef __cplusplus so that it works as a c header too
*/
#ifdef __cplusplus
#include <systemc>
#include "systemc.h"
#include <string>

using namespace sc_core;
using namespace std;

extern "C" int run_simulation(
    int cycles, 
    unsigned l1CacheLines, 
    unsigned l2CacheLines, 
    unsigned cacheLineSize, 
    unsigned l1CacheLatency, 
    unsigned l2CacheLatency, 
    unsigned memoryLatency, 
    size_t numRequests, 
    struct Request* requests,
    const char* tracefile
);

struct CPU_L1_L2 {
    unsigned l1CacheLines;
    unsigned l2CacheLines;
    unsigned cacheLineSize;
    unsigned l1CacheLatency;
    unsigned l2CacheLatency;
    unsigned memoryLatency;
    size_t numRequests;
    // char data_blocks [][];
    // char[][] cache_blocks;

    CPU_L1_L2(unsigned l1CacheLines,
    unsigned l2CacheLines,
    unsigned cacheLineSize,
    unsigned l1CacheLatency,
    unsigned l2CacheLatency,
    unsigned memoryLatency) : l1CacheLines(l1CacheLines), l2CacheLines(l2CacheLines), cacheLineSize(cacheLineSize), l1CacheLatency(l1CacheLatency), l2CacheLatency(l2CacheLatency), memoryLatency(memoryLatency) {

    }
  
    // SC_MODULE(L1){
    //     sc_in<char[]> data_in;
    //     sc_in<sc_bv<32>> address;
    //     sc_in<sc_bv<32>> address_out;
    //     sc_in<bool> write_enable;
    //     sc_in<bool> clk;
    //     sc_out<bool> write_enable_out;
    //     sc_out<char[]> data_out;
    //     sc_out<bool> hit;
        
    //     unsigned l1CacheLines;
    //     unsigned l1CacheLatency;
    //     unsigned cacheLineSize;
    //     vector<bool> valid(l1CacheLines, false);
    //     vector<bool> dirty(l1CacheLines, false);
    //     vector<uint32_t> tags;

    //     unsigned L1latency;

    //     char cache_blocks[][];

    //     SC_CTOR(L1);
        
    //     L1(sc_module_name name, unsigned l1CacheLines, unsigned cacheLineSize, unsigned l1CacheLatency) : L1(name), l1CacheLines(l1CacheLines), cacheLineSize(cacheLineSize), l1CacheLatency(l1CacheLatency) { 
    //         SC_CTHREAD(update, clk.pos());
    //         cache_blocks = new char[l1CacheLines][cacheLineSize]
    //     }

    //     void update(){

    //         while (true)
    //         {
    //             /*converts address from binary to decimal*/
    //              unsigned address_int = address->read().to_uint();

    //             /*extracts metadata bits from address*/
    //             /*using bit casting, because cache line size and number of cache lines are always power of 2*/
    //             unsigned index = (address >> int(log2(l1CacheLines))) & (l1CacheLines-1);
    //             unsigned tag = address >> int(log2(cacheLineSize)-1) >> int(log2(l1CacheLines)-1);
    //             unsigned offset = address & (cacheLineSize-1);

    //             /*write operation*/
    //             if(write_enable->read()){
    //                 if (tags[index] == tag && valid[index] == true)
    //                 /*write hit, write through*/
    //                 {
    //                     hit->write(true);
    //                     for (int i=0; i<cacheLineSize;i++){
    //                         /*write the input data to the matching cacheline */
    //                         char data[cacheLineSize] = data_in.read()
    //                         cache_blocks[index][i]= 0;
    //                     }
    //                     dirty[index] = true;
    //                     break;
    //                 /*write miss, propagate to L2*/
    //                 } else{
    //                     data_out->write(data_in->read());
    //                     address_out->write(address_in->read());
    //                     write_enable_out->write(write_enable->read());
    //                 }


    //             /*read operation*/
    //             } else{
    //                 /*cache hit*/
    //                 if (valid[index] && tags[index]==tag)
    //                 {
    //                     hit->write(true);
    //                     /*bring the data from cache back to cpu*/
    //                     sc_bv<8*cache_line_size> tmp;
    //                     for (int i = 0; i < cache_line_size; i++)
    //                     {//copy each byte from the cache line to vector tmp
    //                         tmp.range(8*(i+1)-1, 8*i) = cache_blocks[index][i];
    //                     }
    //                     data_out->write(tmp);
    //                     break;
    //                 }
    //                 /*cache miss, propagate to L2*/
    //                 else{
    //                     address_out->write(address_in->read());
    //                     write_enable_out->write(write_enable->read());
    //                 }
                    
    //             }

    //             /*waits for clock*/
    //             for (int i = 0; i < L1latency - 1; i++) {
    //                 wait();
    //             }
                
    //         }

    //     }
    // };



    // /*Trang*/
    // SC_MODULE(L2){
    //     sc_in<char[]> data_in;
    //     sc_in<sc_bv<32>> address;
    //     sc_in<sc_bv<32>> address_out;
    //     sc_in<bool> write_enable;
    //     sc_out<bool> write_enable_out;
    //     sc_out<char[]> data_out;
    //     sc_bv<bool> hit;

    //     vector<bool> valid (l2CacheLines, false);
    //     vector<bool> dirty (l2CacheLines, false);
    //     vector<uint32_t> tags;

    //     unsigned L2latency;

    //     SC_CTOR(L2) { 
    //         SC_CTHREAD(update, clk.pos());
    //     }

    //     void update(){

    //         while (true)
    //         {
    //             /*converts address from binary to decimal*/
    //              unsigned address_int = address->read().to.uint();

    //             /*extracts metadata bits from address*/
    //             /*using bit casting, because cache line size and number of cache lines are always power of 2*/
    //             unsigned index = (address >> int(log2(l2CacheLines))) & (l2CacheLines-1);
    //             unsigned tag = address >> int(log2(cache_line_size)-1) >> int(log2(l2CacheLines)-1);
    //             unsigned offset = address & (cache_line_size-1);

    //             /*write operation*/
    //             if(write_enable->read()){
    //                 if (tags[index] == tag && valid[index])
    //                 /*write hit, write through*/
    //                 {
    //                     hit->write(true);
    //                     for (int i=0; i<cache_line_size;i++){
    //                         /*write the input data to the matching cacheline */
    //                         cache_blocks[index][i]= data_in->read().range(8*(i+1)-1,8*1).to_uint();
    //                     }
    //                     dirty[index] = true;
    //                     break;
    //                 /*write miss, propagate to L2*/
    //                 } else{
    //                     data_out->write(data_in->read());
    //                     address_out->write(address_in->read());
    //                     write_enable_out->write(write_enable->read());
    //                 }


    //             /*read operation*/
    //             } else{
    //                 /*cache hit*/
    //                 if (valid[index] && tags[index]==tag)
    //                 {
    //                     hit->write(true);
    //                     /*bring the data from cache back to cpu*/
    //                     sc_bv<8*cache_line_size> tmp;
    //                     for (int i = 0; i < cache_line_size; i++)
    //                     {//copy each byte from the cache line to vector tmp
    //                         tmp.range(8*(i+1)-1, 8*i) = cache_blocks[index][i];
    //                     }
    //                     data_out->write(tmp);
    //                     break;
    //                 }
    //                 /*cache miss, propagate to L2*/
    //                 else{
    //                     address_out->write(address_in->read());
    //                     write_enable_out->write(write_enable->read());
    //                 }
                    
    //             }

    //             /*waits for clock*/
    //             for (int i = 0; i < L2latency - 1; i++) {
    //                     wait();
    //             }
                
    //         }

    //     }
    // };


    /* anthony
        MEMORY serves as the main memory of the computer
    */
    template<unsigned cacheLineSize> SC_MODULE(MEMORY) {
        sc_in<char> data_in[cacheLineSize];
        sc_in<sc_bv<32>> address;
        sc_in<bool> write_enable;
        sc_in<bool> clock; 
        sc_out<char> data_out[cacheLineSize];

        char memory_blocks[1000000];
        int latency;
        int count = 0;

        SC_CTOR(MEMORY);
        MEMORY(sc_module_name name, int latency) : sc_module(name), latency(latency) {
            SC_CTHREAD(update, clock.pos());
        }

        /* anthony
            This update() method will wait until the cycle takes 8 ticks, before executing the write/read
        */
        
        void update() {
            while (true) {

                /* anthony
                    Accessing memory: each cell has 4 bytes, which means that to convert address to index
                    will be address/4 + address % 4 
                */

                unsigned address_u = (address->read()).to_uint();

                if (!write_enable->read()) {
                    // Read data from memory
                    char buffer_out[cacheLineSize];

                    for (int i = 0; i < latency - 1; i++) {
                        wait();
                    }

                    for (int i = 0; i < cacheLineSize; i++) {
                        char result = memory_blocks[address_u];
                        
                        // Write the result into data_outs
                        data_out[i]->write(result);

                        // Change address
                        address_u++;
                    }

                    // Make temporary vector to write to data_out
                    // char* buffer_vector;
                    // buffer_vector = buffer_out;
                    
                    // Wait for the latency time
                    

                    // std::cout << buffer_out << std::endl;

                    // Write to data_out
                    // data_out->write(buffer_out);
                } else {
                    // Write data to memory
                    // char* buffer_vector = data_in->read();
                    // char* temp = buffer_vector;

                    for (int i = 0; i < cacheLineSize; i++) {
                        char c = data_in[i]->read();

                        // Write to memory
                        memory_blocks[address_u] = c;

                        // Change address
                        address_u++;

                    }

                    // Stall for the latency time
                    for (int i = 0; i < latency - 1; i++) {
                        wait();
                    }
                }

                wait();
            }
        }
    };

    int test_memory() {
        MEMORY<64> memory("memory", 1);
        char data[64] = {'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd',
        'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd',
        'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd',
        'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd'};
        
        // data_in signal
        sc_signal<char> mem[64];
        
        // data_out signal
        sc_signal<char> out[64];
        
        // Address
        sc_signal<sc_bv<32>> address;
        sc_bv<32> addr_buffer;
        
        // Clock
        sc_clock clk("clk", 1, SC_SEC);
        
        // Write enable signal
        sc_signal<bool> we;
        we = true;

        // Write to each mem signal and then bind each data_in signal to each mem signal
        for (int i = 0; i < 64; i++) {
            mem[i] = data[i];
            memory.data_in[i](mem[i]);
        }

        // Bind each data_out signal to out signal
        for (int i = 0; i < 64; i++) {
            memory.data_out[i](out[i]);
        }


        // Bind the address, the clock, and the write enable
        memory.address(address);
        memory.clock(clk);
        memory.write_enable(we);

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

struct Request {
    __uint32_t addr;
    __uint32_t data;
    int we;
};

struct Result {
    size_t cycles;
    size_t misses;
    size_t hits;
    size_t primitiveGateCount;
};

#endif
