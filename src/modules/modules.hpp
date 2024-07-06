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
#include "../main/simulator.hpp" // the struct moved here - cleaner - Leon

using namespace sc_core;
using namespace std;

extern "C" struct Result run_simulation(
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



/*Trang*/
SC_MODULE(L1){

    sc_in<char*> data_in_from_CPU;
    sc_in<char*> data_in_from_L2;

    sc_in<uint32_t> address;
    sc_out<uint32_t> address_out;

    sc_in<bool> write_enable;
    sc_out<bool> write_enable_out;

    sc_out<char*> data_out_to_CPU;
    sc_out<char*> data_out_to_L2;
    
    sc_out<bool> hit;
    sc_in<bool> done_from_L2;
    sc_in<bool> clk;

    sc_out<bool> done;
    

    vector<vector<char>> cache_blocks;
    
    vector<bool> valid;
    vector<uint32_t> tags;

    unsigned cacheLineSize;
    unsigned l1CacheLines;
    unsigned l1CacheLatency;
    
    SC_CTOR(L1);
    L1(sc_module_name name, unsigned cacheLineSize, unsigned l1CacheLines, unsigned l1CacheLatency): sc_module(name), cacheLineSize(cacheLineSize), l1CacheLines(l1CacheLines), l1CacheLatency(l1CacheLatency) {
        cache_blocks.resize(l1CacheLines, vector<char> (cacheLineSize));
        valid.resize(l1CacheLines);
        tags.resize(l1CacheLines);

        SC_CTHREAD(update, clk.pos());
    };



    void update(){
        while (true)
        {
            hit->write(false);
            done->write(false);
            
            /*converts address from binary to decimal*/
            unsigned address_int = address->read();

            /*extracts metadata bits from address*/
            /*using bit casting, because cache line size and number of cache lines are always power of 2*/
            size_t index = (address_int >> int(log2(l1CacheLines))) & (l1CacheLines-1);
            unsigned tag = address_int >> int(log2(cacheLineSize)-1) >> int(log2(l1CacheLines)-1);
            unsigned offset = address_int & (cacheLineSize-1);

            /*write operation*/
            if(write_enable->read()){
                if ((tags[index] == tag )&& (valid[index]))
                /*write hit, write through*/
                {
                    hit->write(true);
                    for (int i = 0; i < 4; i++){
                        /*write the input data to the matching cacheline */
                        cache_blocks[index][i + offset] = data_in_from_CPU->read()[i];
                        // std::cout << "|" << cache_blocks[index][i + offset] << std::endl;
                    }
                }

                /*no matter write miss or write hit, propagate to L2*/
                char* tmp = new char[4];

                for (int i = 0; i < 4; i++) {
                    tmp[i] = data_in_from_CPU->read()[i];

                }

                data_out_to_L2->write(tmp);

                address_out->write(address->read());
                write_enable_out->write(write_enable->read());

            /*read operation*/
            } else{
                /*cache hit*/
                if (valid[index] && tags[index]==tag)
                {
                    hit->write(true);
                    /// Bring the data to cpu
                    
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
                        cache_blocks[index][i] = data_in_from_L2->read()[i];
                    }
            
                }
                // std::cout << "read" << std::endl;
                char* tmp = new char[4];
                for (int i = 0; i < 4; i++) {
                    tmp[i] = cache_blocks[index][i];
                }
                    
                data_out_to_CPU->write(tmp);
            }

            /*waits for clock*/
            for (int i = 0; i < l1CacheLatency; i++) {
                wait();
            }
            done->write(true);
            
        }
        
        



    }
};

/*Trang*/
SC_MODULE(L2){
    sc_in<char*> data_in_from_L1;
    sc_in<char*> data_in_from_Mem;

    sc_in<uint32_t> address;
    sc_out<uint32_t> address_out;

    sc_in<bool> write_enable;
    sc_out<bool> write_enable_out;

    sc_out<char*> data_out_to_L1;
    sc_out<char*> data_out_to_Mem;

    sc_out<bool> hit;
    sc_in<bool> done_from_Mem;
    sc_in<bool> clk;

    sc_out<bool> done;



    vector<vector<char>> cache_blocks;

    vector<bool> valid;
    vector<uint32_t> tags;
    
    // char data_blocks[l2CacheLines];

    unsigned cacheLineSize;
    unsigned l2CacheLines;
    unsigned l2CacheLatency;



    SC_CTOR(L2);
    L2(sc_module_name name, unsigned cacheLineSize, unsigned l2CacheLines, unsigned l2CacheLatency): sc_module(name), cacheLineSize(cacheLineSize), l2CacheLines(l2CacheLines), l2CacheLatency(l2CacheLatency)    {

        cache_blocks.resize(l2CacheLines, vector<char> (cacheLineSize));
        valid.resize(l2CacheLines);
        tags.resize(l2CacheLines);

        SC_CTHREAD(update, clk.pos());
    }

    void update(){

        while (true)
        {   
            hit->write(false);
            done->write(false);

            /*converts address from binary to decimal*/
            unsigned address_int = address->read();

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
                    for (int i=0; i<4;i++){
                        /*write the input data to the matching cacheline */
                        cache_blocks[index][i+offset]= data_in_from_L1->read()[i];
                    }
                // propagate to Memory
                }
                /*no matter write miss or hit, continues to propagate to Memory*/
                char* tmp = new char[cacheLineSize];

                for (int i=0; i<4;i++){
                    tmp[i] = data_in_from_L1->read()[i];
                }

                data_out_to_Mem->write(tmp);

                
                // data_out_to_Mem->write(data_in_from_L1->read());
                address_out->write(address->read());
                write_enable_out->write(write_enable->read());
                


            /*read operation*/
            } else{
                /*cache hit*/
                if (valid[index] && tags[index]==tag)
                {
                    hit->write(true);

                }
                /*cache miss, propagate to mem*/
                else{
                    address_out->write(address->read());
                    write_enable_out->write(write_enable->read());
                    while (!done_from_Mem->read()) {
                        wait();
                    }

                    for (int i = 0; i < cacheLineSize; i++) {
                        cache_blocks[index][i] = data_in_from_Mem->read()[i];
                    }


                }
                //bring the read data back to L1
                char* tmp = new char[4];
                for (int i = 0; i < 4; i++) {
                    tmp[i] = cache_blocks[index][i];
                }
                    
                data_out_to_L1->write(tmp);
                
            }

            /*waits for clock*/
            for (int i = 0; i < l2CacheLatency; i++) {
                    wait();
            }
            done->write(true);
            
        }
        
        



    }
};

/* anthony
    MEMORY serves as the main memory of the computer
*/
SC_MODULE(MEMORY) {
    sc_in<char*> data_in_from_L2;
    sc_in<uint32_t> address;
    sc_in<bool> write_enable;
    sc_in<bool> clock; 
    sc_out<char*> data_out_to_L2;

    sc_out<bool> done;

    char memory_blocks[1000000];
    int latency;
    int count = 0;

    unsigned cacheLineSize;

    SC_CTOR(MEMORY);
    MEMORY(sc_module_name name,  unsigned cacheLineSize, int latency) : sc_module(name), latency(latency), cacheLineSize(cacheLineSize) {
        SC_CTHREAD(update, clock.pos());
    }

    /* anthony
        This update() method will wait until the cycle takes 8 ticks, before executing the write/read
    */
    
    void update() {
        while (true) {
            done->write(false);

            /* anthony
                Accessing memory: each cell has 4 bytes, which means that to convert address to index
                will be address/4 + address % 4 
            */

            unsigned address_u = (address->read());

            if (!write_enable->read()) {
                // Read data from memory
                char buffer_out;

                for (int i = 0; i < latency - 1; i++) {
                    wait();
                }
                char* tmp = new char[cacheLineSize];
                for (int i = 0; i < cacheLineSize; i++) {
                    tmp[i] = memory_blocks[address_u];

                    // Change address
                    address_u++;
                }

                // Write the result into data_outs
                data_out_to_L2->write(tmp);
                
            } else {
                // Write data to memory
                // char* buffer_vector = data_in->read();
                // char* temp = buffer_vector;

                for (int i = 0; i < 4; i++) {
                    // Write to memory
                    memory_blocks[address_u] = data_in_from_L2->read()[i];
                    // Change address
                    address_u++;

                }

                // Stall for the latency time
                for (int i = 0; i < latency - 1; i++) {
                    wait();
                }
            }

            wait();
            done->write(true);
        }
    }
};

struct CPU_L1_L2 {
    
    unsigned l1CacheLines;
    unsigned l2CacheLines;       
    unsigned cacheLineSize;
    unsigned l1CacheLatency;
    unsigned l2CacheLatency;
    unsigned memoryLatency;
    size_t numRequests;
    struct Request* requests;
    const char* tracefile;
 
    L1* l1;
    L2* l2;
    MEMORY* memory;

    // Bus between CPU and Cache
    sc_signal<char*> data_in;
    sc_signal<char*> data_out;

    // Signal between L1 and L2
    sc_signal<char*> data_from_L1_to_L2;
    sc_signal<char*> data_from_L2_to_L1;

    // Signal between L2 and Memory
    sc_signal<char*> data_from_L2_to_Memory;
    sc_signal<char*> data_from_Memory_to_L2;

    // Address
    sc_signal<uint32_t> address;
    sc_signal<uint32_t> address_from_L1_to_L2;
    sc_signal<uint32_t> address_from_L2_to_Memory;

    // Write enable flags
    sc_signal<bool> write_enable;
    sc_signal<bool> write_enable_from_L1_to_L2;
    sc_signal<bool> write_enable_from_L2_to_Memory;
    
    // Done flags
    sc_signal<bool> done_from_L1;
    sc_signal<bool> done_from_L2;
    sc_signal<bool> done_from_Memory;

    // Hit flags
    sc_signal<bool> hit_from_L1;
    sc_signal<bool> hit_from_L2;

    CPU_L1_L2( const unsigned l1CacheLines, const unsigned l2CacheLines,
        const unsigned cacheLineSize,
        unsigned l1CacheLatency, unsigned l2CacheLatency, unsigned memoryLatency,
        const char* tracefile) : 
        l1CacheLines(l1CacheLines), l2CacheLines(l2CacheLines), 
        cacheLineSize(cacheLineSize), 
        l1CacheLatency(l1CacheLatency), l2CacheLatency(l2CacheLatency), memoryLatency(memoryLatency),
        tracefile(tracefile) {
        
        // Initialize L1, L2, and Memory
        l1 = new L1("L1", cacheLineSize, l1CacheLines, l1CacheLatency);
        l2 = new L2("L2", cacheLineSize, l2CacheLines, l2CacheLatency);
        memory = new MEMORY("Memory", cacheLineSize, memoryLatency);

        data_in = new char[4];

        // Bind signals
        // 1. Bind CPU signals to L1
        l1->data_in_from_CPU(data_in);
        l1->data_out_to_CPU(data_out);
        
        // 2. Bind signals between L1-L2 and L2-Memory
        
        // L1-L2
        l1->data_out_to_L2(data_from_L1_to_L2);
        l2->data_in_from_L1(data_from_L1_to_L2);
        l1->data_in_from_L2(data_from_L2_to_L1);
        l2->data_out_to_L1(data_from_L2_to_L1);

        // L2-Memory
        l2->data_out_to_Mem(data_from_L2_to_Memory);
        memory->data_in_from_L2(data_from_L2_to_Memory);
        l2->data_in_from_Mem(data_from_Memory_to_L2);
        memory->data_out_to_L2(data_from_Memory_to_L2);
        
        // 3. Bind address
        l1->address(address);
        l1->address_out(address_from_L1_to_L2);
        l2->address(address_from_L1_to_L2);

        l2->address_out(address_from_L2_to_Memory);
        memory->address(address_from_L2_to_Memory);

        // 4. Bind write enable flags
        l1->write_enable(write_enable);
        
        l1->write_enable_out(write_enable_from_L1_to_L2);
        l2->write_enable(write_enable_from_L1_to_L2);

        l2->write_enable_out(write_enable_from_L2_to_Memory);
        memory->write_enable(write_enable_from_L2_to_Memory);

        // 5. Bind Done flags
        l1->done(done_from_L1);

        l1->done_from_L2(done_from_L2);
        l2->done(done_from_L2);

        l2->done_from_Mem(done_from_Memory);
        memory->done(done_from_Memory);

        // 6.Bind hit flags
        l1->hit(hit_from_L1);
        l2->hit(hit_from_L2);

    }
    
    // struct Result send_request(struct Request request) {
    //     sc_vector<sc_signal<char>> data;
    //     uint32_t data_req = request.data;
    //     size_t cycle_count = 0;
        
    //     for (int i = 0; i < 4; i++) {
    //         data[i] = data_req % 16;
    //         data_req /= 16;
    //     }

    //     data_in = data;
    //     write_enable = request.we;
    //     address = request.addr;
        
    //     while (!done_from_L1.read()) {
    //         sc_start(1, SC_SEC);
    //         std::cout << done_from_L1.read() << std::endl;
    //         cycle_count++;
    //     }

    //     struct Result res = {cycle_count, 0, 0, 0};
    //     delete[] data;
    //     return res;
    // }

    // size_t get_gate_count() {
    //     return 10;
    // }
   

    int test_L1(unsigned cacheLineSize, unsigned l1CacheLines, unsigned l1CacheLatency) {
        L1 l1("l1", cacheLineSize, l1CacheLines, l1CacheLatency);
        char data[64] = {'c', 'b', 'a', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd',
                        'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd',
                        'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd',
                        'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd'};
        
        sc_signal<char*> data_in;
         

        sc_signal<char*> data_in_l2;

        sc_signal<char*> data_out_to_CPU;
        sc_signal<char*> data_out_to_L2;

        sc_signal<bool> hit;
        sc_signal<bool> done;

        sc_signal<uint32_t> address;
        sc_signal<uint32_t> address_out;
        
        sc_clock clk("clk", 1, SC_SEC);

        sc_signal<bool> done_from_L2;
        done_from_L2 = true;
        
        sc_signal<bool> we;
        we = true;
        sc_signal<bool> we_out;
        char* tmp = new char[5];
        for (int i = 0; i < 4; i++) {
            tmp[i] = data[i];
        }
        tmp[4] = '\0';
        data_in = tmp;
        l1.data_in_from_CPU(data_in);
        l1.data_out_to_CPU(data_out_to_CPU);
    
        l1.data_in_from_L2(data_in_l2);
        l1.data_out_to_L2(data_out_to_L2);
        
        sc_trace_file * trace_file = sc_create_vcd_trace_file("trace");
        sc_trace(trace_file, data_in.read(), "Data In");

        l1.address(address);
        l1.address_out(address_out);
        l1.clk(clk);
        l1.write_enable(we);
        l1.write_enable_out(we_out);
        l1.hit(hit);
        l1.done(done);

        l1.done_from_L2(done_from_L2);

    
        sc_start(1, SC_SEC);
        data_in_l2 = data_out_to_L2.read();
        // std::cout << "Pointer " << reinterpret_cast<void *>(data_in_l2.read()) << std::endl;
        we = false;
        sc_start(1, SC_SEC);

        char* vec = data_out_to_CPU.read();
        // std::cout << "Pointer " << reinterpret_cast<void *>(vec) << std::endl;

        for (int i = 0; i < 4; i++) {
            std::cout << vec[i];
        }
        
        std::cout << std::endl;
        // std::cout << data_in << std::endl;
        return 0;
    }



    

    int test_L2() {
        L2 l2("l2", 64, 4, 1);
        char data[64] = {'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd',
                        'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd',
                        'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd',
                        'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd'};
        
        sc_signal<char*> data_in;
        

        sc_signal<char*> data_in_from_Mem;

        sc_signal<char*> data_out_to_L1;
        sc_signal<char*> data_out_to_Mem;

        sc_signal<bool> hit;
        sc_signal<uint32_t> address;
        sc_signal<uint32_t> address_out;

        
        sc_clock clk("clk", 1, SC_SEC);

        sc_signal<bool> done_from_Mem;
        done_from_Mem = true;
        
        sc_signal<bool> we;
        we = true;
        sc_signal<bool> we_out;

        sc_signal<bool> done;
        

        char* tmp = new char[4];
        for (int i=0; i<4; i++){
            tmp[i] = data[i];
        }
        data_in =tmp;

        l2.data_in_from_L1(data_in);
        l2.data_out_to_Mem(data_out_to_Mem);
        l2.data_in_from_Mem(data_in_from_Mem);
        l2.data_out_to_L1(data_out_to_L1);

        l2.address(address);
        l2.address_out(address_out);
        l2.clk(clk);
        l2.write_enable(we);
        l2.write_enable_out(we_out);
        l2.hit(hit);
        l2.done(done);

        l2.done_from_Mem(done_from_Mem);

        sc_start(1, SC_SEC);
        data_in_from_Mem = data_out_to_Mem.read();
        we = false;

        sc_start(1, SC_SEC);


        char* vec = data_out_to_Mem.read();


        for (int i = 0; i < 4; i++) {
           std::cout << vec[i];
        }
      
        std::cout << std::endl;


        
        return 0;
        
    }


    

    int test_memory() {
        MEMORY memory("memory", 64, 1);
        char data[64] = {'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd',
        'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd',
        'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd',
        'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd'};
        
        // data_in signal
        sc_signal<char*> mem;
        
        // data_out signal
        sc_signal<char*> out;
        
        // Address
        sc_signal<uint32_t> address;
        
        // Clock
        sc_clock clk("clk", 1, SC_SEC);
        
        // Write enable signal
        sc_signal<bool> we;
        we = true;

        mem = data;
        memory.data_in_from_L2(mem);
        // Write to each mem signal and then bind each data_in signal to each mem signal
        // for (int i = 0; i < 64; i++) {
        //     mem[i] = data[i];
        //     memory.data_in[i](mem[i]);
        // }

        // Bind each data_out signal to out signal
        memory.data_out_to_L2(out);


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
            std::cout << out.read()[i];
        }
        std::cout << std::endl;

        return 0;
    }

    
};
#endif
#endif