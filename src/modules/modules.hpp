#ifndef MODULES_HPP
#define MODULES_HPP

#ifdef __cplusplus // added #ifdef __cplusplus so that it works as a c header - anthony

// Split modules here + imported several libs - Leon
#include "memory.hpp"
#include "cache_l1.hpp"
#include "cache_l2.hpp"

using namespace sc_core;
using namespace std;

/**
 * @brief custom sc_trace() method for pointer values
 * 
 * @details - Leon
 * The function sc_trace is used to record the time-ordered sequence of value changes during simulation
 * 
 * void sc_trace(sc_trace_file* tf, const T& object, const std::string& name);
 * 
 * Parameters:
 * 1. tf: A pointer to an sc_trace_file object, which represents the VCD file.
 * 2. object: The object to be traced. This could be a signal or a variable.
 * 3. name: The name to be used for the object in the VCD file
 * 
 * Note:
 * This function will only trace the specific memory location to which it is pointed. 
 * In the case of `(short *) &arr[i]` or `(int *) &arr[i]`, it it will:
 * - trace the 2-byte value (assume `short` is 2 bytes) starting at `&arr[i]`. 
 * - trace the 4-byte value (assume `int` is 4 bytes) starting at `&arr[i]`.
 * 
 * sc_trace() does not automatically trace other areas of the array. 
 * - need to call sc_trace() for each indices.
 * 
 * @param file The trace file.
 * @param arr Array to be traced.
 * @param amount Number of elements to be traced.
 * @param name Name of traced value
 * 
 * @warning Assumption is that the array "arr" is of type "short" or "int"
 * 
 * @authors
 * Alexander Anthony Tang
 * Lie Leon Alexius
 */
void trace(sc_trace_file* file, char* arr, size_t amount, string name) {
    int i = 0;

    // check if there is any value in array to trace
    // trace the specific area (goal here is to split to 4 Bytes - int, otherwise short)
    while (amount > 0) {
        if (amount <= 2) {
            (amount == 2) ? 
            sc_trace(file, (short *) &arr[i], name + "_" + to_string(i)) : 
            sc_trace(file, (char *) &arr[i], name + "_" + to_string(i));
            break;
        }
        else { 
            sc_trace(file, (int *) &arr[i], name + "_" + to_string(i));
        }
        amount -= 4;
        i += 4;
    }
}

/**
 * @brief CPU_L1_L2 simulates a CPU with L1 and L2 caches and main memory.
 * 
 * @details
 * Simplified Architecture:
 * CPU <--> L1 <--> L2 <--> Memory
 *
 * @authors
 * Alexander Anthony Tang
 * Van Trang Nguyen
 */
struct CPU_L1_L2 {
    
    unsigned l1CacheLines;      // Number of cache lines in L1 cache
    unsigned l2CacheLines;      // Number of cache lines in L2 cache  
    unsigned cacheLineSize;     // Size of each cache line
    unsigned l1CacheLatency;    // Latency of L1 cache
    unsigned l2CacheLatency;    // Latency of L2 cache
    unsigned memoryLatency;     // Latency of main memory
    size_t numRequests;         // Number of requests
    struct Request* requests;   // Array of requests
    const char* tracefile;      // Tracefile name
 
    L1* l1;                     // Pointer to L1 cache
    L2* l2;                     // Pointer to L2 cache
    MEMORY* memory;             // Pointer to main memory

    // Bus between CPU and Cache (L1)
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
    /*
        "valid" marks if the commands from component "from" is a valid one
        
        Example: 
        Suppose Memory is done, L2 is now writing the data from Memory, 
        Memory must wait until L2 give new command, not the current old command (signal)
    */
    sc_signal<bool> valid;
    sc_signal<bool> valid_from_L1_to_L2;
    sc_signal<bool> valid_from_L2_to_Memory;

    // Clock and Trace File
    sc_clock* clk = new sc_clock("clk", 1, SC_SEC);
    sc_trace_file* trace_file;

   /**
    * @brief Constructor for memory hierarchy system CPU_L1_L2.
    * 
    * @details
    * In case of:
    * 1. Read Miss (L1, L2): Fetch from memory, write to L2, write to L1, send to CPU
    * 2. Write Miss: Write to Memory
    * 3. Write Hit: Update value (L1 or L2), Write to Memory
    * 4. Read Hit: If in L1, L2 won't do anything (wait), etc.
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
    CPU_L1_L2 (const unsigned l1CacheLines, const unsigned l2CacheLines, const unsigned cacheLineSize,
        unsigned l1CacheLatency, unsigned l2CacheLatency, unsigned memoryLatency,
        const char* tracefile) : 
        l1CacheLines(l1CacheLines), l2CacheLines(l2CacheLines), cacheLineSize(cacheLineSize), 
        l1CacheLatency(l1CacheLatency), l2CacheLatency(l2CacheLatency), memoryLatency(memoryLatency),
        tracefile(tracefile) {
        
        // Initialize L1, L2, and Memory
        l1 = new L1("L1", cacheLineSize, l1CacheLines, l1CacheLatency);
        l2 = new L2("L2", cacheLineSize, l2CacheLines, l2CacheLatency);
        memory = new MEMORY("Memory", cacheLineSize, memoryLatency);

        // Initialize data_in, etc. and set value to '\0'
        // Bus from Memory -> L2 -> L1 is as big as a cacheLine, while the other is only 4 Bytes
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

        // start simulation for 1 delta cycle, without advancing the time (Simulation Second)
        sc_start(SC_ZERO_TIME);

        /*
            Must run simulation first otherwise segmentation fault
            This is due to data_in, etc. is an array with NULLs and thus must be filled (1 delta cycle)
            https://www.learnsystemc.com/basic/simu_stage
        */

        // Bind to trace
        trace_file = sc_create_vcd_trace_file(tracefile);

        // Custom Trace - Array
        trace(trace_file, data_in, 4, "Data_In");
        trace(trace_file, data_out, 4, "Data_Out");

        trace(trace_file, data_from_L1_to_L2, 4, "Data-from_L1_to_L2");
        trace(trace_file, data_from_L2_to_L1, cacheLineSize, "Data_from_L2_to_L1");

        trace(trace_file, data_from_L2_to_Memory, 4, "Data_from_L2_to_Memory");
        trace(trace_file, data_from_Memory_to_L2, cacheLineSize, "Data_from_Memory_to_L2");

        // Standard Trace
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
    }
    

    /**
     * @brief Send a request to the simulated memory hierarchy system.
     *
     * @param request The request to send.
     * @return Result structure containing cycle count and other data.
     * 
     * @todo Fine Tuning: hit_from_L2.read(); must return valid only if cycles = 12
     *
     * @authors 
     * Alexander Anthony Tang
     * Lie Leon Alexius
     */
    struct Result send_request(struct Request request) {

        uint32_t data_req = request.data;

        /*  Leon - Optimized from Base Anthony
            The data is unsigned 32 bits (4 Bytes), our Bus is also 4 Bytes (char array of size 4)
            Assign the data to the Bus (1 Byte = 0 - 255)
        */
        for (int i = 0; i < 4; i++) {
            data_in[i] = (char) (data_req & 0xFF); // 256 = (1.0000.0000)b
            data_req >> 8; // divide 256;
        }

        // signal the caches
        write_enable = request.we;
        address = request.addr;
        valid = true;

        // Variables to calculate Cycle Count and Hits
        size_t cycle_count = 0;
        bool cache_l2_executes = false; // whether L2 runs or not
        size_t hit_L2 = 0; // does L2 hit? 0 or 1

        // run the simulation (+1) to process the request
        do {
            sc_start(1, SC_SEC);

            // if L1 miss, then request propagated to L2, thus valid_from_L1_to_L2 = true
            if (valid_from_L1_to_L2) {
                cache_l2_executes = true;
                hit_L2 |= hit_from_L2.read(); // TODO: Make sure only return 1 not at 5 Sec
            }

            cycle_count++;

            /*
                Cases for cycles:
                L1.done = true in 4 Sec if L1 hit
                L1.done = true in 16 Sec (if L1 miss but L2 hit - L2.done = 12 Sec)
                L1.done = true in 116 Sec (if L2 miss - Memory.done = 100 Sec)
            */
        } while (!done_from_L1.read());

        /*
            Calculating the misses and hits
            cache_l2_executes is true IFF "valid_from_l1_to_l2" is true

            1. Misses:
               - If L1 Hit: Misses = 0
               - If L1 Miss: Miss = !(hit_L1) + (1 - hit_L2)

            2. Hits:
               - If L1 Hit: Hits = 1
               - If L1 Miss: Hits = hit_L2
        */
        size_t misses = !(hit_from_L1) + ((cache_l2_executes) ? (1 - hit_L2) : 0);
        size_t hits = hit_from_L1 + ((cache_l2_executes) ? hit_L2 : 0);
        
        // create Result and send back
        struct Result res = { 
            cycle_count, // cycles
            misses, // misses
            hits, // hits
            get_gate_count() // primitive gate count
        };

        valid = false; // set valid as false
        return res;
    }

    /**
     * @brief stop the simulation, close trace file, clean up
     * @link https://www.geeksforgeeks.org/delete-in-c/ 
     * @author 
     * Anthony Tang
     * Lie Leon Alexius
     */
    void close_trace_file() {
        // SystemC - stop then close
        sc_stop();
        sc_close_vcd_trace_file(trace_file);

        // Free all allocated memory
        delete l1;
        delete l2;
        delete memory;
        delete clk;

        delete[] data_in.read();
        delete[] data_out.read();
        delete[] data_from_L1_to_L2.read();
        delete[] data_from_L2_to_L1.read();
        delete[] data_from_L2_to_Memory.read();
        delete[] data_from_Memory_to_L2.read();
    }

    /**
     * @brief get the gate count
     * @todo Implement This
     */
    size_t get_gate_count() {
        return -1;
    }
};

#endif // #ifdef __cplusplus
#endif // #ifndef MODULES_HPP