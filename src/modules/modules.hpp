// temporary struct
#include<systemc>
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

    
}
