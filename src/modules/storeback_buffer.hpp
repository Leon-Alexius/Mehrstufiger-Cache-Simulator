// Leon - L1 moved here

#ifndef STOREBACK_BUFFER_HPP
#define STOREBACK_BUFFER_HPP

#ifdef __cplusplus // added #ifdef __cplusplus so that it works as a c header - anthony
#include <systemc>
#include <vector>

#include "../main/simulator.hpp" // the struct moved here - Leon

// using namespace directives won't get carried over. 
using namespace sc_core;
using namespace std;

/**
* @brief L1 represents a L1 cache in a memory hierarchy system
* @details This L1 module handles read and write operations to and from CPU and L2 cache
* its write miss policy is no-write-allocate, no-fetch-on-write, and no-write-before-hit
* its write hit policy is write-through
* 
* @author Van Trang Nguyen
*/
SC_MODULE(STOREBACK){

    sc_fifo<char*> storeback;
    sc_fifo<uint32_t> address_storeback;
    vector<uint32_t> tags_storeback;
    unsigned head;
    unsigned tail;
    unsigned capacity;

    bool empty = true;

    /**
     * @brief Constructor for L1 cache module.
     *
     * @param name The name of the module.
     * @param cacheLineSize The size of each cache line.
     * @param l1CacheLines The number of cache lines in the L1 cache.
     * @param l1CacheLatency The latency of the L1 cache in clock cycles.
     *
     * @authors 
     * Van Trang Nguyen
     * Lie Leon Alexius
     */
    SC_CTOR(STOREBACK);
    STOREBACK(sc_module_name name, unsigned capacity) : sc_module(name), capacity(capacity), storeback(capacity), address_storeback(capacity) {
        tags_storeback.resize(capacity);
    };

    // Will be used by L2
    bool write(char* data, uint32_t address, uint32_t tag) {
        wait(SC_ZERO_TIME);
        wait(SC_ZERO_TIME);
        
        if (storeback.nb_write(data) && address_storeback.nb_write(address)) {
            
            tags_storeback[tail] = tag;
            tail = (tail + 1) % capacity;
            empty = false;
            return true;
        } else {
            return false;
        }
    }

    // Will be used by MEMORY
    bool read(char*& data, uint32_t& address) {
        wait(SC_ZERO_TIME);
        bool data_avail = storeback.nb_read(data);
        bool address_avail = address_storeback.nb_read(address);
        
        if (data_avail && address_avail) {
            wait(SC_ZERO_TIME);
            head = (head + 1) % capacity;
            if (head == tail) empty = true;
            // std::cout << data_avail << " " << address_avail << std::endl;
            return true;
        } else {
            return false;
        }
    }

    bool in_buffer(uint32_t tag) {
        for (unsigned i = 0; i < (tail - head + capacity) % capacity; i++) {
            // std::cout << tag << " + " << tags_storeback[(i+head) % capacity] << std::endl;
            if (tag == tags_storeback[(i+head) % capacity]) return true;
        }
        return false;
    }

    bool is_empty() {
        return empty;
    }
};

#endif
#endif
