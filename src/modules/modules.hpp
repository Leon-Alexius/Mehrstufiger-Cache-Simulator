#ifndef MODULES_HPP
#define MODULES_HPP

/* anthony
added #ifdef __cplusplus so that it works as a c header too
*/
#ifdef __cplusplus
#include <vector>
#include <iostream>
#include <systemc>
#include <string>
#include <cmath>

#include "../main/simulator.hpp" // the struct moved here - Leon

using namespace sc_core;
using namespace std;

/**
* @brief custom sc_trace method for pointer values
*
* @param f Trace file.
* @param arr Array to be traced.
* @param amount Number of elements to be traced.
* @param name Name of traced value
*
* @author Alexander Anthony Tang
*/

void trace(sc_trace_file*& f, char* arr, size_t amount, std::string name) {
    int i = 0;
    //check if there is any value in array to trace
    while (amount > 0) {
        // int temp = 0;
        // for (int j = 0; j < 4; j++) {
        //     std::cout << j << std::endl;
        //     if (i + j >= amount) break;
        //     temp += (arr[j] << (8*j));
        // }
        if (amount == 2) {
            sc_trace(f, (short *) &arr[i], name + "_" + to_string(i));
            break;
        } else { 
            sc_trace(f, (int *) &arr[i], name + "_" + to_string(i));
        }
        amount -= 4;
        i += 4;
    }

    // for (int i = 0; i < amount; i++) {
    //     sc_trace(f, arr[i], name + "_" + to_string(i));
    // }
}


/**
* @brief L1 represents a L1 cache in a memory hierarchy system
* @details This L1 module handles read and write operations to and from CPU and L2 cache
*
* @author Van Trang Nguyen
*/

SC_MODULE(L1){

    sc_in<char*> data_in_from_CPU;      ///< Data input from CPU for write
    sc_in<char*> data_in_from_L2;       ///< Data input from L2 cache to L1

    sc_in<uint32_t> address;            ///< Address input for read/write 
    sc_out<uint32_t> address_out;       ///< Address propagated to L2

    sc_in<bool> write_enable;           ///< Write-enable signal for a write operation
    sc_out<bool> write_enable_out;      ///< Write-enable signal propagated to L2 cache

    sc_in<char*> data_out_to_CPU;       ///< Read data to the CPU
    sc_in<char*> data_out_to_L2;        ///< Data propagated from CPU to the L2 cache
    
    sc_out<bool> hit;                   ///< Cache hit signal
    sc_in<bool> done_from_L2;           ///< Signal indicating the completion operation in L2
    sc_in<bool> clk;                    ///< Clock signal for synchronization

    sc_out<bool> done;                  ///< Signal indicating the completion of an operation in the current L1
    sc_in<bool> valid_in;               
    sc_out<bool> valid_out;
    

    vector<vector<char>> cache_blocks;  ///< Cache blocks represented in a 2D vector
    
    vector<bool> valid;                 ///< Vector indicating the validity of cache lines
    vector<uint32_t> tags;              ///< Vector storing the tags for each cache line

    unsigned cacheLineSize;             ///< Size of each cache line
    unsigned l1CacheLines;              ///< Number of cache lines in the L1 cache
    unsigned l1CacheLatency;            ///< Latency of L1 cache in clock cycles
    
    /**
     * @brief Constructor for L1 cache module.
     *
     * @param name The name of the module.
     * @param cacheLineSize The size of each cache line.
     * @param l1CacheLines The number of cache lines in the L1 cache.
     * @param l1CacheLatency The latency of the L1 cache in clock cycles.
     *
     * @author Van Trang Nguyen
     */

    SC_CTOR(L1);
    L1(sc_module_name name, unsigned cacheLineSize, unsigned l1CacheLines, unsigned l1CacheLatency): sc_module(name), cacheLineSize(cacheLineSize), l1CacheLines(l1CacheLines), l1CacheLatency(l1CacheLatency) {
        cache_blocks.resize(l1CacheLines, vector<char> (cacheLineSize));
        valid.resize(l1CacheLines);
        tags.resize(l1CacheLines);

        SC_THREAD(update);
        sensitive << clk.pos();

    };


    /**
     * @brief Main update method for the L1 cache.
     * @details This method implements the main functionality of the cache, which is handling read and write operations while maintaining cache coherence.
     * The operations are synchronized with the clock signal.
     */
    void update(){
        wait();
        while (true)
        {
            
            wait(SC_ZERO_TIME);
            // std::cout << "Done write false" << std::endl;
            
            hit->write(false);
            done->write(false);
            
            //converts address from binary to decimal
            unsigned address_int = address->read();

            //extracts metadata bits from address
            //using bit casting, because cache line size and number of cache lines are always power of 2
            size_t index = (address_int >> int(log2(l1CacheLines))) & (l1CacheLines-1);
            unsigned tag = address_int >> int(log2(cacheLineSize)-1) >> int(log2(l1CacheLines)-1);
            unsigned offset = address_int & (cacheLineSize-1);
            std::cout << "Write enable L1: " << write_enable->read() << std::endl;

            //write operation
            if(write_enable->read()){
                // std::cout << "write" << std::endl;
                if ((tags[index] == tag )&& (valid[index]))
                //write hit, write through
                {
                    hit->write(true);
                    for (int i = 0; i < 4; i++){
                        //write the input data to the matching cacheline
                        cache_blocks[index][i + offset] = data_in_from_CPU->read()[i];
                        // std::cout << "|" << cache_blocks[index][i + offset] << std::endl;
                    }
                }

                //no matter write miss or write hit, propagate to L2
                // char* tmp = new char[4];

                for (int i = 0; i < 4; i++) {
                    data_out_to_L2->read()[i] = data_in_from_CPU->read()[i];
                }

                // data_out_to_L2->write(tmp);

                address_out->write(address->read());
                write_enable_out->write(write_enable->read());
                // wait(SC_ZERO_TIME);
                valid_out->write(true);
                while (!done_from_L2->read()) {
                    wait();
                }
                valid_out->write(false);
            //read operation
            } else{
                // std::cout << "read" << std::endl;
                /*cache hit*/
                if (valid[index] && tags[index]==tag)
                {
                    hit->write(true);
                    // Bring the data to cpu
                    
                }
                //Read miss, propagate to L2, load cacheline from L2 to L1, and write to data_out_to_CPU
                else{
                    uint32_t temp_address = ((address->read())/cacheLineSize) * cacheLineSize;
                    address_out->write(temp_address);
                    write_enable_out->write(write_enable->read());
                    valid_out->write(true);
                    wait(SC_ZERO_TIME);
                    while (!done_from_L2->read()) {
                        wait();
                    }
                    valid_out->write(false);
                    for (unsigned i = 0; i < cacheLineSize; i++) {
                        cache_blocks[index][i] = data_in_from_L2->read()[i];
                    }
                    valid[index] = true;
            
                }
                // std::cout << "read" << std::endl;
                // char* tmp = new char[4];
                for (unsigned i = 0; i < 4; i++) {
                    data_out_to_CPU->read()[i] = cache_blocks[index][i + offset];
                    std::cout << "read from L1 cache: " << data_out_to_CPU->read()[i] << std::endl;
                }
                    
                // data_out_to_CPU->write(tmp);
            }

            //waits for clock
            for (unsigned i = 0; i < l1CacheLatency - 1; i++) {
                wait();
            }
            done->write(true);
            wait();
            
            
        }
    

    }
};

/**
 * @brief L2 represents an L2 cache in a memory hierarchy system.
 * @details L2 handles read and write operations to and from L1 cache and main memory.
 *
 * @author Van Trang Nguyen
 */

SC_MODULE(L2){
    sc_in<char*> data_in_from_L1;           ///< Data input from L1 cache for write
    sc_in<char*> data_in_from_Mem;          ///< Data input from memory to L2

    sc_in<uint32_t> address;                ///< Address input for read/write operations
    sc_out<uint32_t> address_out;           ///< Address propagated to memory

    sc_in<bool> write_enable;               ///< Write-enable signal for a write operation
    sc_out<bool> write_enable_out;          ///< Write-enable signal propagated to memory

    sc_in<char*> data_out_to_L1;            ///< Data output to L1 cache
    sc_in<char*> data_out_to_Mem;           ///< Data output to memory

    sc_out<bool> hit;                       ///< Cache hit signal
    sc_in<bool> done_from_Mem;              ///< Signal indicating the completion of an operation in memory
    sc_in<bool> clk;                        ///< Clock signal for synchronization

    sc_out<bool> done;                      ///< Signal indicating the completion of an operation in L2 cache
    sc_in<bool> valid_in;
    sc_out<bool> valid_out;



    vector<vector<char>> cache_blocks;      ///< A 2D vector representing the cache blocks

    vector<bool> valid;                     ///< Vector indicating the validity of cache lines
    vector<uint32_t> tags;                  ///< Vector storing the tags for each cache line
    
    //char data_blocks[l2CacheLines];

    unsigned cacheLineSize;                 ///< Size of each cache line
    unsigned l2CacheLines;                  ///< Number of cache lines in the L2 cache
    unsigned l2CacheLatency;                ///< Latency of L2 cache in clock cycles


   /**
    * @brief Constructor for L2 cache module.
    *
    * @param name The name of the module.
    * @param cacheLineSize The size of each cache line.
    * @param l2CacheLines The number of cache lines in the L2 cache.
    * @param l2CacheLatency The latency of the L2 cache in clock cycles.
    *
    * @author Van Trang Nguyen
    */
    SC_CTOR(L2);
    L2(sc_module_name name, unsigned cacheLineSize, unsigned l2CacheLines, unsigned l2CacheLatency): sc_module(name), cacheLineSize(cacheLineSize), l2CacheLines(l2CacheLines), l2CacheLatency(l2CacheLatency)    {

        cache_blocks.resize(l2CacheLines, vector<char> (cacheLineSize));
        valid.resize(l2CacheLines);
        tags.resize(l2CacheLines);

        SC_THREAD(update);
        sensitive << clk.pos();
    }

   /**
    * @brief Main update method for the L2 cache.
    * @details This method implements the main functionality of the cache, handling read and write operations while maintaining cache coherence.
    * The operations are synchronized with the clock signal.
    */

    void update(){
        wait();
        while (true)
        {   
            
            wait(SC_ZERO_TIME);
            wait(SC_ZERO_TIME);
           
            
            hit->write(false);
            done->write(false);
            

            while (!valid_in->read()) {
                wait();
            }

            std::cout << "Write Enable L2: " << write_enable->read() << std::endl;

            

            //converts address from binary to decimal
            unsigned address_int = address->read();

            //extracts metadata bits from address
            //using bit casting, because cache line size and number of cache lines are always power of 2
            unsigned index = (address_int >> int(log2(l2CacheLines))) & (l2CacheLines-1);
            unsigned tag = address_int >> int(log2(cacheLineSize)-1) >> int(log2(l2CacheLines)-1);
            unsigned offset = address_int & (cacheLineSize-1);

            //write operation
            if(write_enable->read()){
                if (tags[index] == tag && valid[index])
                // write hit, write through
                {
                    hit->write(true);
                    for (unsigned i=0; i<4;i++){
                        //write the input data to the matching cacheline 
                        cache_blocks[index][i+offset]= data_in_from_L1->read()[i];
                    }
                // propagate to Memory
                }
                //no matter write miss or hit, continues to propagate to Memory
                // char* tmp = new char[cacheLineSize];

                
                for (unsigned i=0; i<4;i++){
                    
                    data_out_to_Mem->read()[i] = data_in_from_L1->read()[i];
                }

                // data_out_to_Mem->write(tmp);

                
                // data_out_to_Mem->write(data_in_from_L1->read());
                address_out->write(address->read());
                write_enable_out->write(write_enable->read());
                valid_out->write(true);
                while (!done_from_Mem->read()) {
                    wait();
                }
                valid_out->write(false);


            //read operation
            } else{
                
                //cache hit
                if (valid[index] && tags[index]==tag)
                {
                    hit->write(true);

                }
                //cache miss, propagate to mem
                else{
                    uint32_t temp_address = ((address->read())/cacheLineSize) * cacheLineSize;
                    address_out->write(temp_address); 
                    
                    write_enable_out->write(write_enable->read());
                    
                    valid_out->write(true);
                    while (!done_from_Mem->read()) {
                        wait();
                    }
                    valid_out->write(false);

                    for (unsigned i = 0; i < cacheLineSize; i++) {
                        cache_blocks[index][i] = data_in_from_Mem->read()[i];
                    }
                    valid[index] = true;


                }
                //bring the read data back to L1
                // char* tmp = new char[4];
                for (unsigned i = 0; i < 4; i++) {
                    data_out_to_L1->read()[i] = cache_blocks[index][i];
                }
                    
                // data_out_to_L1->write(tmp);
                
            }

            //waits for clock
            for (unsigned i = 0; i < l2CacheLatency - 1; i++) {
                wait();
            }
            done->write(true);
            wait();
            
        }

    }
};

/**
 * @brief MEMORY represents the main memory in a memory hierarchy system.
 * @details MEMORY handles read and write operations directly from L2 cache.
 *
 * @author Alexander Anthony Tang
 */

SC_MODULE(MEMORY) {
    sc_in<char*> data_in_from_L2;       ///< Data input from L2 cache for write
    sc_in<char*> data_out_to_L2;        ///< Data output to L2 cache for read  
    sc_in<uint32_t> address;            ///< Address input for read/write operations
    sc_in<bool> write_enable;           ///< Write-enable signal for a write operation
    sc_in<bool> clock;                  ///< Clock signal for synchronization


    sc_out<bool> done;                  ///< Signal indicating the completion of an operation in the current memory
    sc_in<bool> valid_in;

    char memory_blocks[1000000];        ///< Memory blocks represented by an array of char
    unsigned latency;                   ///< Latency of memory in clock cycles

    unsigned cacheLineSize;             ///< Size of each cache line

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
    MEMORY(sc_module_name name,  unsigned cacheLineSize, unsigned latency) : sc_module(name), latency(latency), cacheLineSize(cacheLineSize) {
        SC_THREAD(update);
        sensitive << clock.pos();
    }

   /**
    * @brief Main update method for the MEMORY.
    * @details This method implements the main functionality of the memory, handling read and write operations while maintaining memory coherence.
    * The operations are synchronized with the clock signal.
    * This update() method will wait until the cycle takes 8 ticks, before executing the write/read
    *
    * @author Alexander Anthony Tang
    */
    void update() {
        while (true) {
            wait(SC_ZERO_TIME);
            wait(SC_ZERO_TIME);
            done->write(false);
            while(!valid_in->read()) {
                wait();
            }

            // Accessing memory: each cell has 4 bytes, which means that to convert address to index
            // will be address/4 + address % 4
            unsigned address_u = (address->read());

            if (!write_enable->read()) {
                // Read data from memory

                for (unsigned i = 0; i < latency - 1; i++) {
                    wait();
                }

                for (unsigned i = 0; i < cacheLineSize; i++) {
                    data_out_to_L2->read()[i] = memory_blocks[address_u];
                    // std::cout << "read from memory: " << memory_blocks[address_u] << std::endl;
                    // Change address
                    address_u++;
                }

                // Write the result into data_outs
                // data_out_to_L2->write(tmp);
                
            } else {
                // Write data to memory
                // char* buffer_vector = data_in->read();
                // char* temp = buffer_vector;
                

                for (unsigned i = 0; i < 4; i++) {
                    // Write to memory
                    memory_blocks[address_u] = data_in_from_L2->read()[i];
                    // std::cout << "in memory: " << memory_blocks[address_u] << std::endl;
                    // Change address
                    address_u++;

                }

                // Stall for the latency time
                for (unsigned i = 0; i < latency - 1; i++) {
                    wait();
                }
            }
            done->write(true);
            wait();
            
        }
    }
};

/**
 * @brief CPU_L1_L2 simulates a CPU with L1 and L2 caches and main memory.
 *
 * @authors
 * Alexander Anthony Tang
 * Van Trang Nguyen
 */

struct CPU_L1_L2 {
    
    unsigned l1CacheLines;      ///< Number of cache lines in L1 cache
    unsigned l2CacheLines;      ///< Number of cache lines in L2 cache  
    unsigned cacheLineSize;     ///< Size of each cache line
    unsigned l1CacheLatency;    ///< Latency of L1 cache
    unsigned l2CacheLatency;    ///< Latency of L2 cache
    unsigned memoryLatency;     ///< Latency of main memory
    size_t numRequests;         ///< Number of requests
    struct Request* requests;   ///< Array of requests
    const char* tracefile;      ///< Tracefile name
 
    L1* l1;                     ///< Pointer to L1 cache
    L2* l2;                     ///< Pointer to L2 cache
    MEMORY* memory;             ///< Pointer to main memory

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

    // Valid flags
    sc_signal<bool> valid;
    sc_signal<bool> valid_from_L1_to_L2;
    sc_signal<bool> valid_from_L2_to_Memory;

    sc_clock* clk = new sc_clock("clk", 1, SC_SEC);
    sc_trace_file* trace_file;


   /**
    * @brief Constructor for memory hierarchy system CPU_L1_L2.
    *
    * @param l1CacheLines Number of cache lines in L1 cache.
    * @param l2CacheLines Number of cache lines in L2 cache.
    * @param cacheLineSize Size of each cache line in L1 and L2.
    * @param l1CacheLatency Latency of L1 cache.
    * @param l2CacheLatency Latency of L2 cache.
    * @param memoryLatency Latency of main memory.
    * @param tracefile Name of trace file.
    *
    * @authors
    * Alexander Anthony Tang
    * Van Trang Nguyen
    */

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

        // char* temp_data_in = new char[4] ();
        // data_in = temp_data_in;
        // char* temp_data_out = new char[4] ();
        // data_out = temp_data_out;

        data_in = new char[4]();
        data_out = new char[4]();

        data_from_L1_to_L2 = new char[4]();
        data_from_L2_to_L1 = new char[cacheLineSize] ();

        data_from_L2_to_Memory = new char[4] ();
        data_from_Memory_to_L2 = new char[cacheLineSize] ();
        

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

        // 7. Bind clock
        l1->clk(*clk);
        l2->clk(*clk);
        memory->clock(*clk);

        // Bind valids;
        l1->valid_in(valid);
        
        l1->valid_out(valid_from_L1_to_L2);
        l2->valid_in(valid_from_L1_to_L2);

        l2->valid_out(valid_from_L2_to_Memory);
        memory->valid_in(valid_from_L2_to_Memory);

        sc_start(SC_ZERO_TIME);

        // Bind to trace
        trace_file = sc_create_vcd_trace_file(tracefile);
        std::cout << reinterpret_cast<void *> (trace_file) << std::endl;
        trace(trace_file, data_in, 4, "Data_In");
        trace(trace_file, data_out, 4, "Data_Out");

        trace(trace_file, data_from_L1_to_L2, 4, "Data-from_L1_to_L2");
        trace(trace_file, data_from_L2_to_L1, cacheLineSize, "Data_from_L2_to_L1");

        trace(trace_file, data_from_L2_to_Memory, 4, "Data_from_L2_to_Memory");
        trace(trace_file, data_from_Memory_to_L2, cacheLineSize, "Data_from_Memory_to_L2");

        sc_trace(trace_file, address, "Address");
        sc_trace(trace_file, address_from_L1_to_L2, "Address_from_L1_to_L2");
        sc_trace(trace_file, address_from_L2_to_Memory, "Address_from_L2_to_Memory");

        sc_trace(trace_file, write_enable, "WE");
        sc_trace(trace_file, write_enable_from_L1_to_L2, "WE_from_L1_to_L2");
        sc_trace(trace_file, write_enable_from_L2_to_Memory, "WE_from_L2_to_Memory");

        sc_trace(trace_file, done_from_L1, "Done_from_L1");
        sc_trace(trace_file, done_from_L2, "Done_from_L2");
        sc_trace(trace_file, done_from_Memory, "Done_from_memory");
         
        sc_trace(trace_file, hit_from_L1, "Hit_from_L1");
        sc_trace(trace_file, hit_from_L2, "Hit_from_L2");

        // sc_start(10, SC_SEC);
        // sc_close_vcd_trace_file(trace_file);
    }
    

    /**
     * @brief Send a request to the simulated memory hierarchy system.
     *
     * @param request The request to send.
     * @return Result structure containing cycle count and other data.
     *
     * @author Alexander Anthony Tang
     */

    struct Result send_request(struct Request request) {
        // sc_signal<char*> data;
        uint32_t data_req = request.data;
        size_t cycle_count = 0;
        std::cout << "test" << std::endl;
        for (int i = 0; i < 4; i++) {
            data_in[i] = (char) data_req % 256;
            data_req /= 256;
        }

       
        write_enable = request.we;
        
        address = request.addr;
        
        
        do {
            sc_start(1, SC_SEC);
            cycle_count++;
        } while (!done_from_L1.read());
    
        for (int i = 0; i < 1; i++) std::cout << data_out[0] << data_out[1] << data_out[2] << data_out[3] << std::endl;
        
        
        struct Result res = {cycle_count, 0, 0, 0};
 
        return res;
    }

    void close_trace_file() {
        sc_stop();
        std::cout << reinterpret_cast<void *> (trace_file) << std::endl;
        sc_close_vcd_trace_file(trace_file);
        delete[] data_in.read();
        delete[] data_out.read();
        delete[] data_from_L1_to_L2.read();
        delete[] data_from_L2_to_L1.read();
        delete[] data_from_L2_to_Memory.read();
        delete[] data_from_Memory_to_L2.read();
        
    
        
    }

    size_t get_gate_count() {
        return 10;
    }
  
   

    // int test_L1(unsigned cacheLineSize, unsigned l1CacheLines, unsigned l1CacheLatency) {
    //     // L1 l1("l1", cacheLineSize, l1CacheLines, l1CacheLatency);
    //     char data[64] = {'c', 'b', 'a', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd',
    //                     'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd',
    //                     'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd',
    //                     'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd'};
        
    //     // sc_signal<char*> data_in;
    //     // data_in = new char[4];
         

    //     // sc_signal<char*> data_in_l2;

    //     // sc_signal<char*> data_out_to_CPU;
    //     // sc_signal<char*> data_out_to_L2;

    //     // sc_signal<bool> hit;
    //     // sc_signal<bool> done;

    //     // sc_signal<uint32_t> address;
    //     // sc_signal<uint32_t> address_out;
        
    //     // sc_clock clk("clk", 1, SC_SEC);

    //     // sc_signal<bool> done_from_L2;
    //     // done_from_L2 = true;
        
    //     // sc_signal<bool> we;
    //     // we = true;
    //     // sc_signal<bool> we_out;
    //     // char* tmp = new char[4];
    //     // for (int i = 0; i < 4; i++) {
    //     //     tmp[i] = data[i];
    //     // }
    //     // // tmp[4] = '\0';
    //     // data_in = tmp;
    //     // sc_start(0, SC_SEC);
    //     for (int i = 0; i < 4; i++) {
    //         data_in[i] = data[i];
    //         std::cout << data_in[i] << std::endl;
    //     }
    //     // l1.data_in_from_CPU(data_in);
    //     // l1.data_out_to_CPU(data_out_to_CPU);
    
    //     // l1.data_in_from_L2(data_in_l2);
    //     // l1.data_out_to_L2(data_out_to_L2);
        
       

    //     // l1.address(address);
    //     // l1.address_out(address_out);
    //     // l1.clk(clk);
    //     // l1.write_enable(we);
    //     // l1.write_enable_out(we_out);
    //     // l1.hit(hit);
    //     // l1.done(done);

    //     // l1.done_from_L2(done_from_L2);
        
    
    //     // sc_start(0, SC_SEC);
    //     // sc_trace_file * trace_file = sc_create_vcd_trace_file("trace1");
    //     // trace(trace_file, data_in, 4, "Data In");
        
    //     // // sc_trace(trace_file, data_in, "Data In");
    //     // sc_start(1, SC_SEC);
    //     // data[0] = 'K';
    //     // char* data_in_l2 = data_from_L1_to_L2.read();
    //     // // std::cout << "Pointer " << reinterpret_cast<void *>(data_in_l2.read()) << std::endl;
    //     // write_enable = false;
    //     // sc_start(1, SC_SEC);

    //     // char* vec = data_out.read();
    //     // // std::cout << "Pointer " << reinterpret_cast<void *>(vec) << std::endl;

    //     // for (int i = 0; i < 4; i++) {
    //     //     std::cout << vec[i];
    //     // }

    //     // sc_close_vcd_trace_file(trace_file);
        
    //     // std::cout << std::endl;
    //     // std::cout << data_in << std::endl;
    //     return 0;
    // }



    

    // int test_L2() {
    //     L2 l2("l2", 64, 4, 1);
    //     char data[64] = {'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd',
    //                     'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd',
    //                     'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd',
    //                     'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd'};
        
    //     sc_signal<char*> data_in;
        

    //     sc_signal<char*> data_in_from_Mem;

    //     sc_signal<char*> data_out_to_L1;
    //     sc_signal<char*> data_out_to_Mem;

    //     sc_signal<bool> hit;
    //     sc_signal<uint32_t> address;
    //     sc_signal<uint32_t> address_out;

        
    //     sc_clock clk("clk", 1, SC_SEC);

    //     sc_signal<bool> done_from_Mem;
    //     done_from_Mem = true;
        
    //     sc_signal<bool> we;
    //     we = true;
    //     sc_signal<bool> we_out;

    //     sc_signal<bool> done;
        

    //     char* tmp = new char[4];
    //     for (int i=0; i<4; i++){
    //         tmp[i] = data[i];
    //     }
    //     data_in =tmp;

    //     l2.data_in_from_L1(data_in);
    //     l2.data_out_to_Mem(data_out_to_Mem);
    //     l2.data_in_from_Mem(data_in_from_Mem);
    //     l2.data_out_to_L1(data_out_to_L1);

    //     l2.address(address);
    //     l2.address_out(address_out);
    //     l2.clk(clk);
    //     l2.write_enable(we);
    //     l2.write_enable_out(we_out);
    //     l2.hit(hit);
    //     l2.done(done);

    //     l2.done_from_Mem(done_from_Mem);

    //     sc_start(1, SC_SEC);
    //     data_in_from_Mem = data_out_to_Mem.read();
    //     we = false;

    //     sc_start(1, SC_SEC);


    //     char* vec = data_out_to_Mem.read();


    //     for (int i = 0; i < 4; i++) {
    //        std::cout << vec[i];
    //     }
      
    //     std::cout << std::endl;


        
    //     return 0;
        
    // }


    

    // int test_memory() {
    //     MEMORY memory("memory", 64, 1);
    //     char data[64] = {'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd',
    //     'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd',
    //     'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd',
    //     'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd'};
        
    //     // data_in signal
    //     sc_signal<char*> mem;
        
    //     // data_out signal
    //     sc_signal<char*> out;
        
    //     // Address
    //     sc_signal<uint32_t> address;
        
    //     // Clock
    //     sc_clock clk("clk", 1, SC_SEC);
        
    //     // Write enable signal
    //     sc_signal<bool> we;
    //     we = true;

    //     mem = data;
    //     memory.data_in_from_L2(mem);
    //     // Write to each mem signal and then bind each data_in signal to each mem signal
    //     // for (int i = 0; i < 64; i++) {
    //     //     mem[i] = data[i];
    //     //     memory.data_in[i](mem[i]);
    //     // }

    //     // Bind each data_out signal to out signal
    //     memory.data_out_to_L2(out);


    //     // Bind the address, the clock, and the write enable
    //     memory.address(address);
    //     memory.clock(clk);
    //     memory.write_enable(we);

    //     // Start for 1 cycle to let write take place
    //     sc_start(1, SC_SEC);

    //     // Begin read by setting write_enable to false
    //     we = false;

    //     // Start simulation for 1 cycle
    //     sc_start(1, SC_SEC);
        
    //     // Read from the out signals
    //     for (int i = 0; i < 64; i++) {
    //         std::cout << out.read()[i];
    //     }
    //     std::cout << std::endl;

    //     return 0;
    // }

    
};
#endif
#endif