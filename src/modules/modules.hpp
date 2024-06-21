// temporary struct
#include<systemc>
#include <cmath>
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
    private char[][] data_blocks;
    private char[][] cache_blocks;
  
    SC_MODULE(L1){
        sc_in<sc_bv<8*cache_line_size>> data_in;
        sc_in<sc_bv<32>> address_input;
        sc_in<bool> write_enable;
        sc_out<sc_bv<8*cache_line_size>> data_out;
        sc_bv <bool> hit;
        sc_in<bool> clk;

        L1(sc_module_name name): sc_module(name){
            SC_THREAD(behaviour, clk.pos())
        }

        void behaviour(){
            while (true)
            {
                unsigned offset_bits_amount = (int)log2((float)cache_line_size);
                unsigned index_bits_amount = (int) ceil((float)log2(64000/cache_line_size));
                unsigned tag_bits_amount = 32 - offset_bits_amount - index_bits_amount;
                unsigned address = (address_input->read()).to_uint();
                unsigned address = address<<tag_bits_amount >>offset_bits_amount;

                if (write_enable->read()) {
                    data_block[][] = 
                    data_out.write();
                }else{
                    
                    data_out->write()
                }

                

                wait();
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
        
        char[][] memory_blocks;
        int latency;
        int count = 0;
        SC_CTOR(MEMORY);

        MEMORY(sc_module_name name, int latency) : sc_module(name), latency(latency) {
            SC_CTHREAD(update, clk.pos());
        }

        /*anthony
        This update() method will wait until the cycle takes 8 ticks, before executing the write/read
        */
        void update() {
            while (true) {
                /* 
                    Accessing memory: each cell has 4 bytes, which means that to convert address to index
                    will be address/4 + address % 4 
                */
                unsigned address_u = (address->read()).to_uint();
                unsigned index1 = address_u / 4;
                unsigned index2 = address_u % 4;

                if (write_enable->read()) {
                    bool buffer_out[cacheLineSize*8] = {};
                    for (int i = 0; i < cacheLineSize*8; i += 8) {

                        char result = memory_blocks[index1][index2];
                        // Convert the result into an array of bits
                        for (int j=0; j<8; j++) buffer_out[j + i] = ((theByte & (1<<(j))) != 0)      

                        address_u++;
                        index1 = address_u / 4;
                        index2 = address_u % 4;
                    }
                    
                }
                wait();
            }
        }
    }
}
