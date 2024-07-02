// temporary struct
#ifndef MODULES_HPP
#define MODULES_HPP

/* anthony
added #ifdef __cplusplus so that it works as a c header too
*/
#ifdef __cplusplus
#include <vector>
#include <iostream>
#include "systemc.h"
#include <systemc>
#include <string>
#include <cmath>

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

    CPU_L1_L2 (unsigned l1CacheLines, unsigned l2CacheLines, unsigned cacheLineSize, 
    unsigned l1CacheLatency, unsigned l2CacheLatency, unsigned memoryLatency) : l1CacheLines(l1CacheLines), l2CacheLines(l2CacheLines), cacheLineSize(cacheLineSize), l1CacheLatency(l1CacheLatency), l2CacheLatency(l2CacheLatency), memoryLatency(memoryLatency) {

    }




    /*Trang*/
    template<unsigned cacheLineSize, unsigned l1CacheLines>SC_MODULE(L1){

        sc_in<char> data_in_from_CPU[4];
        sc_in<char> data_in_from_L2[cacheLineSize];

        sc_in<sc_bv<32>> address;
        sc_out<sc_bv<32>> address_out;

        sc_in<bool> write_enable;
        sc_out<bool> write_enable_out;

        sc_out<char> data_out_to_CPU[4];
        sc_out<char> data_out_to_L2[cacheLineSize];
        
        sc_out<bool> hit;
        sc_in<bool> done_from_L2;
        sc_in<bool> clk;

        sc_out<bool> done;
        

        char cache_blocks[l1CacheLines][cacheLineSize];
        


        bool valid[l1CacheLines];
        uint32_t tags[l1CacheLines];

        unsigned l1CacheLatency;
        unsigned l2CacheLatency;
        unsigned memoryLatency;

        SC_CTOR(L1);
        L1(sc_module_name name, unsigned l1CacheLatency): sc_module(name),  l1CacheLatency(l1CacheLatency) {
            SC_CTHREAD(update, clk.pos());
        };



        void update(){
            while (true)
            {
                /*converts address from binary to decimal*/
                unsigned address_int = address->read().to_uint();

                /*extracts metadata bits from address*/
                /*using bit casting, because cache line size and number of cache lines are always power of 2*/
                size_t index = (address_int >> int(log2(l1CacheLines))) & (l1CacheLines-1);
                unsigned tag = address_int >> int(log2(cacheLineSize)-1) >> int(log2(l1CacheLines)-1);
                unsigned offset = address_int & (cacheLineSize-1);
                std::cout << tag << std::endl;
                /*write operation*/
                if(write_enable->read()){
                    if ((tags[index] == tag )&& (valid[index]))
                    /*write hit, write through*/
                    {
                        hit->write(true);
                        for (int i = 0; i < 4; i++){
                            /*write the input data to the matching cacheline */
                            cache_blocks[index][i + offset]= data_in_from_CPU[i].read();
                            std::cout << cache_blocks[index][i + offset] << std::endl;
                        }
                    }

                    /*no matter write miss or write hit, propagate to L2*/
                    
                    for (int i = 0; i < 4; i++) {
                        data_out_to_L2[i]->write(data_in_from_CPU[i]->read());
                    }
                    // data_out->write(data_in_from_CPU->read());
                    // std::cout << data_in_from_CPU->read() << std::endl;

                    address_out->write(address->read());
                    write_enable_out->write(write_enable->read());

                /*read operation*/
                } else{
                    /*cache hit*/
                    if (valid[index] && tags[index]==tag)
                    {
                        hit->write(true);
                        /// Bring the data to cpu
                        for (int i = 0; i < 4; i++) {
                            data_out_to_CPU[i]->write(cache_blocks[index][i + offset]);
                        }
                    }
                    /*
                    Read miss, propagate to L2, load cacheline from L2 to L1, and write to data_out_to_CPU
                    */
                    else{
                        address_out->write(address->read());
                        write_enable_out->write(write_enable->read());
                        while (!done_from_L2->read()) {
                            wait();
                        }
                        for (int i = 0; i < cacheLineSize; i++) {
                            cache_blocks[index][i] = data_in_from_L2[i]->read();
                        }
                    }
                   
                }

                /*waits for clock*/
                for (int i = 0; i < l1CacheLatency; i++) {
                    wait();
                }
                done->write(true);
                
            }
            
            



        }
    };

    int test_L1() {
        L1<64, 4> l1("l1", 1);
        char data[64] = {'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd',
                        'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd',
                        'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd',
                        'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd'};
        
        sc_signal<char> data_in[64];
        

        sc_signal<char> data_in_l2[64];

        sc_signal<char> data_out_to_CPU[4];
        sc_signal<char> data_out_to_L2[64];

        sc_signal<bool> hit;
        sc_signal<bool> done;
        sc_signal<sc_bv<32>> address;
        sc_bv<32> address_buffer;
        sc_signal<sc_bv<32>> address_out;

        
        sc_clock clk("clk", 1, SC_SEC);

        sc_signal<bool> done_from_L2;
        done_from_L2 = false;
        
        sc_signal<bool> we;
        we = true;
        sc_signal<bool> we_out;

        for (int i = 0; i < 4; i++) {
            data_in[i] = data[i];
            l1.data_in_from_CPU[i](data_in[i]);
            l1.data_out_to_CPU[i](data_out_to_CPU[i]);
        }

        for (int i = 0; i < 64; i++) {
            
            l1.data_in_from_L2[i](data_in_l2[i]);
            l1.data_out_to_L2[i](data_out_to_L2[i]);
        }
        

        l1.address(address);
        l1.address_out(address_out);
        l1.clk(clk);
        l1.write_enable(we);
        l1.write_enable_out(we_out);
        l1.hit(hit);
        l1.done(done);

        l1.done_from_L2(done_from_L2);


        sc_start(1, SC_SEC);
        for (int i = 0; i < 4; i++) {
            std::cout << data_out_to_L2[i];
        }
        std::cout << std::endl;
        // std::cout << data_in << std::endl;
        return 0;
    }



    /*Trang*/
    template<unsigned cacheLineSize, unsigned l2CacheLines>SC_MODULE(L2){
        sc_in<char> data_in_from_L1[cacheLineSize];
        sc_in<char> data_in_from_Mem[cacheLineSize];

        sc_in<sc_bv<32>> address;
        sc_out<sc_bv<32>> address_out;
        sc_in<bool> write_enable;
        sc_out<bool> write_enable_out;
        sc_out<char> data_out_to_L1[cacheLineSize];
        sc_out<char> data_out_to_Mem[cacheLineSize];
        sc_out<bool> hit;
        sc_in<bool> done_from_Mem;

        sc_out<bool> done;

        sc_in<bool> clk;

        char cache_blocks[l2CacheLines][cacheLineSize] ;

        bool valid[l2CacheLines];
        uint32_t tags[l2CacheLines];
      
        // char data_blocks[l2CacheLines][cacheLineSize];

        unsigned l2CacheLatency;

        SC_CTOR(L2);
        L2(sc_module_name name, unsigned l2CacheLatency): sc_module(name), l2CacheLatency(l2CacheLatency){
            SC_CTHREAD(update, clk.pos());
        }

        void update(){

            while (true)
            {
                /*converts address from binary to decimal*/
                unsigned address_int = address->read().to_uint();

                /*extracts metadata bits from address*/
                /*using bit casting, because cache line size and number of cache lines are always power of 2*/
                unsigned index = (address_int >> int(log2(l2CacheLines))) & (l2CacheLines-1);
                unsigned tag = address_int >> int(log2(cacheLineSize)-1) >> int(log2(l2CacheLines)-1);
                unsigned offset = address_int & (cacheLineSize-1);

                /*write operation*/
                if(write_enable->read()){
                    if (tags[index] == tag && valid[index])
                    // write hit, write through
                    {
                        hit->write(true);
                        for (int i=0; i<cacheLineSize;i++){
                            /*write the input data to the matching cacheline */
                            cache_blocks[index][i+offset]= data_in_from_L1[i]->read();
                        }
                    // propagate to Memory
                    }
                    for (int i = 0; i < cacheLineSize; i++)
                    {
                        data_out_to_Mem[i]->write(data_in_from_L1[i]->read());
                    }
                    
                    // data_out_to_Mem->write(data_in_from_L1->read());
                    address_out->write(address->read());
                    write_enable_out->write(write_enable->read());
                    


                /*read operation*/
                } else{
                    /*cache hit*/
                    if (valid[index] && tags[index]==tag)
                    {
                        hit->write(true);
                        /*bring the data from cache back to cpu*/
                        for (int i = 0; i < cacheLineSize; i++)
                        {//copy each byte from the cache line to vector tmp
                            data_out_to_L1[i]->write(cache_blocks[index][i]);
                        }

                    }
                    /*cache miss, propagate to mem*/
                    else{
                        address_out->write(address->read());
                        write_enable_out->write(write_enable->read());
                        while (!done_from_Mem->read()) {
                            wait();
                        }

                        for (int i = 0; i < cacheLineSize; i++) {
                            cache_blocks[index][i] = data_in_from_Mem[i]->read();
                        }
                    }
                    
                }

                /*waits for clock*/
                for (int i = 0; i < l2CacheLatency; i++) {
                        wait();
                }
                done->write(true);
                
            }
            
            



        }
    };

    int test_L2() {
        L2<64, 4> l2("l2", 1);
        char data[64] = {'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd',
                        'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd',
                        'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd',
                        'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd'};
        
        sc_signal<char> data_in[64];
        

        sc_signal<char> data_in_from_Mem[64];

        sc_signal<char> data_out_to_L1[64];
        sc_signal<char> data_out_to_Mem[64];

        sc_signal<bool> hit;
        sc_signal<sc_bv<32>> address;
        sc_bv<32> address_buffer;
        sc_signal<sc_bv<32>> address_out;

        
        sc_clock clk("clk", 1, SC_SEC);

        sc_signal<bool> done_from_Mem;
        done_from_Mem = false;
        
        sc_signal<bool> we;
        we = true;
        sc_signal<bool> we_out;

        sc_signal<bool> done;
        

        for (int i = 0; i < 64; i++) {
            data_in[i] = data[i];
            l2.data_in_from_L1[i](data_in[i]);
            l2.data_out_to_Mem[i](data_out_to_Mem[i]);
            l2.data_in_from_Mem[i](data_in_from_Mem[i]);
            l2.data_out_to_L1[i](data_out_to_L1[i]);
        }

        

        l2.address(address);
        l2.address_out(address_out);
        l2.clk(clk);
        l2.write_enable(we);
        l2.write_enable_out(we_out);
        l2.hit(hit);
        l2.done(done);

        l2.done_from_Mem(done_from_Mem);


        sc_start(1, SC_SEC);
        for (int i = 0; i < 64; i++) {
            std::cout << data_out_to_Mem[i];
        }
        std::cout << std::endl;
        // std::cout << data_in << std::endl;
        return 0;
        
    }


    // /* anthony
    //     MEMORY serves as the main memory of the computer
    // */
    // SC_MODULE(MEMORY) {
    //     sc_in<sc_bv<8*cacheLineSize>> data_i_from_CPUn;
    //     sc
    // _in<sc_bv<32>> address;
    //     sc_in<bool> write_enable;
    //     sc_in<bool> clock; 
    //     sc_out<sc_bv<8*cacheLineSize>> data_out;
        
    //     private char[1000000] memory_blocks;
    //     int latency;
    //     int count = 0;
    //     SC_CTOR(MEMORY);

    //     MEMORY(sc_module_name name, int latency) : sc_module(name), latency(latency) {
    //         SC_CTHREAD(update, clk.pos());
    //     }

    //     /* anthony
    //         This update() method will wait until the cycle takes 8 ticks, before executing the write/read
    //     */
        
    //     void update() {
    //         while (true) {

    //             /* anthony
    //                 Accessing memory: each cell has 4 bytes, which means that to convert address to index
    //                 will be address/4 + address % 4 
    //             */

    //             unsigned address_u = (address->read()).to_uint();

    //             if (!write_enable->read()) {
    //                 // Read data from memory
    //                 bool buffer_out[cacheLineSize*8] = {};

    //                 for (int i = 0; i < cacheLineSize*8; i += 8) {
    //                     char result = memory_blocks[address_u];

    //                     // Convert the result into an array of bits
    //                     for (int j=0; j<8; j++) buffer_out[j + i] = ((result & (1<<(j))) != 0);

    //                     // Change address
    //                     address_u++;
    //                 }

    //                 // Make temporary vector to write to data_out
    //                 sc_bv<8*cacheLineSize> buffer_vector;
    //                 buffer_vector = buffer_out;
                    
    //                 // Wait for the latency time
    //                 for (int i = 0; i < latency - 1; i++) {
    //                     wait();
    //                 }

    //                 // Write to data_out
    //                 data_out->write(buffer_vector);
    //             } else {
    //                 // Write data to memory
    //                 sc_bv<8*cacheLineSize> buffer_vector = data_i_from_CPUn->read();
    // /
    // /                 std::string temp = buffer_vector.to_string();

    //                 for (int i = 0; i < cacheLineSize*8; i += 8) {
    //                     char c = 0;

    //                     // Convert string to array of bytes which are written directly to temp
    //                     for (int j = 0; j < 8; j++) {
    //                         c |= (temp[j + i] == "0") ? 0 : (1<<j);
    //                     }

                        

    //                     // Write to memory
    //                     memory_blocks[address_u] = c;

    //                     // Change address
    //                     address_u++;

    //                 }

    //                 // Stall for the latency time
    //                 for (int i = 0; i < latency - 1; i++) {
    //                     wait();
    //                 }
    //             }

    //             wait();
    //         }
    //     }
    // }
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
