// temporary struct
#ifndef MODULES_HPP
#define MODULES_HPP

#include <systemc>
#include <string>
struct Request {
    uint32_t addr;
    uint32_t data;
    int we;
};

struct Result {
    size_t cycles;
    size_t misses;
    size_t hits;
    size_t primitiveGateCount;
};

struct CPU_L1_L2 {
    unsigned l1CacheLines;
    unsigned l2CacheLines;
    unsigned cacheLineSize;
    unsigned l1CacheLatency;
    unsigned l2CacheLatency;
    unsigned memoryLatency;
    size_t numRequests;
    private unsigned cache_line_size;
    private byte[][] data_blocks;
    private byte[][] cache_blocks;
  
    SC_MODULE(L1){
        sc_in<sc_bv<8*cache_line_size>> data_in;
        sc_in<sc_bv<32>> address;
        sc_in<bool> write_enable;
        sc_out<sc_bv<8*cache_line_size>> data_out;
        sc_bv <bool> hit;

        L1(sc_module_name name): sc_module(name){
            
        }

        void behaviour(){
            if (write_enable) {
                
                data_out.write();
            }else{
                
            }
        }
    }

    /* anthony
        MEMORY serves as the main memory of the computer
    */
    SC_MODULE(MEMORY) {
        sc_in<sc_bv<8*cacheLineSize>> data_in;
        sc_in<sc_bv<32>> address;
        sc_in<bool> write_enable;
        sc_in<bool> clock; 
        sc_out<sc_bv<8*cacheLineSize>> data_out;
        
        private char[][] memory_blocks;
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
                unsigned index1 = address_u / 4;
                unsigned index2 = address_u % 4;

                if (!write_enable->read()) {
                    // Read data from memory
                    bool buffer_out[cacheLineSize*8] = {};

                    for (int i = 0; i < cacheLineSize*8; i += 8) {
                        char result = memory_blocks[index1][index2];

                        // Convert the result into an array of bits
                        for (int j=0; j<8; j++) buffer_out[j + i] = ((result & (1<<(j))) != 0);

                        // Change address
                        address_u++;
                        index1 = address_u / 4;
                        index2 = address_u % 4;
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
                        memory_blocks[index1][index2] = c;

                        // Change address
                        address_u++;
                        index1 = address_u / 4;
                        index2 = address_u % 4;

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
