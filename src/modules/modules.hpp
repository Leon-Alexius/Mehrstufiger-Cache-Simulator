// temporary struct
#ifndef MODULES_HPP
#define MODULES_HPP


#ifdef __cplusplus
#include <vector>
#include <iostream>
#include "systemc.h"
#include <systemc>
#include <string>
#include <cmath>
/* anthony
added #ifdef __cplusplus so that it works as a c header too
*/
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




    /*Trang*/
    template<unsigned cache_line_size>SC_MODULE(L1){

        sc_in<char> data_in[cache_line_size];
        sc_in<sc_bv<32>> address;
        sc_out<sc_bv<32>> address_out;
        sc_in<bool> write_enable;
        sc_out<bool> write_enable_out;
        sc_out<char> data_out[cache_line_size];
        sc_signal<bool> hit;
        sc_in<bool> clk;

        char** cache_blocks ;
        
        char** data_blocks;


        std::vector<bool> valid;
        std::vector<unsigned char> dirty;
        std::vector<uint32_t> tags;

        unsigned L1latency;

        SC_CTOR(L1);
        L1(sc_module_name name, std::vector<bool> valid, std::vector<unsigned char> dirty,std::vector<uint32_t> tags): sc_module(name), valid(l1CacheLines, false), dirty(l1CacheLines, false), tags(l1CacheLines,0){
            cache_blocks   =new char*[l1CacheLines];
            for (int i = 0; i < l1CacheLines; i++)
            {
              cache_blocks[i] = new char[cache_line_size];
            };

            data_blocks   =new char*[l1CacheLines];
            for (int i = 0; i < l1CacheLines; i++)
            {
              data_blocks[i] = new char[cache_line_size];
            };
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
                unsigned tag = address_int >> int(log2(cache_line_size)-1) >> int(log2(l1CacheLines)-1);
                unsigned offset = address_int & (cache_line_size-1);

                /*write operation*/
                if(write_enable->read()){
                    if ((tags[index] == tag )&& (valid[index]))
                    /*write hit, write through*/
                    {
                        hit=true;
                        for (int i=0; i<cache_line_size;i++){
        
                            /*write the input data to the matching cacheline */
                            cache_blocks[index][i]= data_in[i].read();
                        }
                        dirty[index] = true;
                        break;
                    /*write miss, propagate to L2*/
                    } else{
                        data_out->write(data_in->read());
                        address_out->write(address->read());
                        write_enable_out->write(write_enable->read());
                    }


                /*read operation*/
                } else{
                    /*cache hit*/
                    if (valid[index] && tags[index]==tag)
                    {
                        hit=true;
                        /*bring the data from cache back to cpu*/
                        sc_bv<8*cache_line_size> tmp;
                        for (int i = 0; i < cache_line_size; i++)
                        {//copy each byte from the cache line to vector tmp
                            tmp.range(8*(i+1)-1, 8*i) = cache_blocks[index][i];
                        }
                        data_out->write(tmp);
                        break;
                    }
                    /*cache miss, propagate to L2*/
                    else{
                        address_out->write(address->read());
                        write_enable_out->write(write_enable->read());
                    }
                    
                }

                /*waits for clock*/
                for (int i = 0; i < L1latency - 1; i++) {
                        wait();
                }
                
            }
            
            



        }
    };



    /*Trang*/
    template<unsigned cache_line_size>SC_MODULE(L2){
        sc_in<char> data_in[cache_line_size];
        sc_in<sc_bv<32>> address;
        sc_out<sc_bv<32>> address_out;
        sc_in<bool> write_enable;
        sc_out<bool> write_enable_out;
        sc_out<char> data_out[cache_line_size];
        sc_signal<bool> hit;
        sc_in<bool> clk;

        std::vector<bool> valid;
        std::vector<unsigned char> dirty;
        std::vector<uint32_t> tags;

        char** cache_blocks ;
      
        char** data_blocks;

        unsigned L2latency;

        SC_CTOR(L2);
        L2(sc_module_name name, std::vector<bool> valid, std::vector<unsigned char> dirty,std::vector<uint32_t> tags): sc_module(name), valid(l2CacheLines, false), dirty(l2CacheLines, false), tags(l2CacheLines,0){
            cache_blocks   =new char*[l2CacheLines];
            for (int i = 0; i < l2CacheLines; i++)
            {
                cache_blocks[i] = new char[cache_line_size];
            };


            data_blocks   =new char*[l2CacheLines];
            for (int i = 0; i < l2CacheLines; i++)
            {
                data_blocks[i] = new char[cache_line_size];
            };

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
                unsigned tag = address_int >> int(log2(cache_line_size)-1) >> int(log2(l2CacheLines)-1);
                unsigned offset = address_int & (cache_line_size-1);

                /*write operation*/
                if(write_enable->read()){
                    if (tags[index] == tag && valid[index])
                    /*write hit, write through*/
                    {
                        hit=true;
                        for (int i=0; i<cache_line_size;i++){
                            /*write the input data to the matching cacheline */
                            cache_blocks[index][i]= data_in->read().range(8*(i+1)-1,8*1).to_uint();
                        }
                        dirty[index] = true;
                        break;
                    /*write miss, propagate to L2*/
                    } else{
                        data_out->write(data_in->read());
                        address_out->write(address->read());
                        write_enable_out->write(write_enable->read());
                    }


                /*read operation*/
                } else{
                    /*cache hit*/
                    if (valid[index] && tags[index]==tag)
                    {
                        hit=true;
                        /*bring the data from cache back to cpu*/
                        sc_bv<8*cache_line_size> tmp;
                        for (int i = 0; i < cache_line_size; i++)
                        {//copy each byte from the cache line to vector tmp
                            tmp.range(8*(i+1)-1, 8*i) = cache_blocks[index][i];
                        }
                        data_out->write(tmp);
                        break;
                    }
                    /*cache miss, propagate to mem*/
                    else{
                        address_out->write(address->read());
                        write_enable_out->write(write_enable->read());
                    }
                    
                }

                /*waits for clock*/
                for (int i = 0; i < L2latency - 1; i++) {
                        wait();
                }
                
            }
            
            



        }
    };

    /* anthony
        MEMORY serves as the main memory of the computer
    */
    SC_MODULE(MEMORY) {
        sc_in<sc_bv<8*cacheLineSize>> data_in;
        sc_in<sc_bv<32>> address;
        sc_in<bool> write_enable;
        sc_in<bool> clock; 
        sc_out<sc_bv<8*cacheLineSize>> data_out;
        
        private char[1000000] memory_blocks;
        int latency;
        int count = 0;
        SC_CTOR(MEMORY);

        MEMORY(sc_module_name name, int latency) : sc_module(name), latency(latency) {
            SC_CTHREAD(update, clk.pos());
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
                    bool buffer_out[cacheLineSize*8] = {};

                    for (int i = 0; i < cacheLineSize*8; i += 8) {
                        char result = memory_blocks[address_u];

                        // Convert the result into an array of bits
                        for (int j=0; j<8; j++) buffer_out[j + i] = ((result & (1<<(j))) != 0);

                        // Change address
                        address_u++;
                    }

                    // Make temporary vector to write to data_out
                    sc_bv<8*cacheLineSize> buffer_vector;
                    buffer_vector = buffer_out;
                    
                    // Wait for the latency time
                    for (int i = 0; i < latency - 1; i++) {
                        wait();
                    }

                    // Write to data_out
                    data_out->write(buffer_vector);
                } else {
                    // Write data to memory
                    sc_bv<8*cacheLineSize> buffer_vector = data_in->read();
                    std::string temp = buffer_vector.to_string();

                    for (int i = 0; i < cacheLineSize*8; i += 8) {
                        char c = 0;

                        // Convert string to array of bytes which are written directly to temp
                        for (int j = 0; j < 8; j++) {
                            c |= (temp[j + i] == "0") ? 0 : (1<<j);
                        }

                        

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
    }
}
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
