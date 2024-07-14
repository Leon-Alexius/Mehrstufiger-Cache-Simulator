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
* @brief The storeback buffer is used so that L2 does not need for writes to end in memory.
* @details This module implements an sc_fifo for both the data and the address. The read() is called by the memory,
* while the write() by the L2 cache. An in_buffer() method denotes if a certain tag is currently in the buffer.
* is_empty() denotes if the buffer is now empty.
*
* @note Using a Write Through with Conditional Flush Buffer may not be faster than a Write Through with Unconditional Flush Buffer.
* As a read from L2 without any dependency will abort any ongoing write, which means the aborted write needs to start from the start
* and cost more cycles. But if a read hit is generated from the block, then the lost cycles will be recovered.
* 
* @author Alexander Anthony Tang
*/
SC_MODULE(STOREBACK){

    sc_fifo<char*> storeback;
    sc_fifo<uint32_t> address_storeback;
    vector<uint32_t> tags_storeback;
    unsigned head;
    unsigned tail;
    unsigned capacity;

    bool empty = true;
    bool conditional = false;

    /**
     * @brief Constructor for Store Back Buffer (Write Through w/ Conditional Flush Buffer) module.
     *
     * @param name The name of the module.
     * @param capacity The capacity of the buffer.
     *
     * @author
     * Alexander Anthony Tang
     */
    SC_CTOR(STOREBACK);
    STOREBACK(sc_module_name name, unsigned capacity, bool conditional) : sc_module(name), capacity(capacity), storeback(capacity), address_storeback(capacity), conditional(conditional) {
        tags_storeback.resize(capacity);
    };

    /**
    * @brief A write method accessing the buffer.
    * 
    * @param data A pointer which consists of an array of length 4.
    * @param address The address which corresponds to the data
    * @param tag The tag associated with the address and data 
    * 
    * @return Returns true if a write is successful, false if the buffer is full.
    * If the buffer is full then it does not change the state of the FIFO.
    */
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

    /**
    * @brief A read method from the buffer.
    * 
    * @param data A variable of a pointer which consists of an array of length 4 to be written to.
    * @param address A variable of the address to be written to
    * 
    * @return Returns true if a read is successful, false if the buffer is empty.
    * If the buffer is empty then it does not change the state of the FIFO.
    */
    bool read(char*& data, uint32_t& address) {
        wait(SC_ZERO_TIME);
        bool data_avail = storeback.nb_read(data);
        bool address_avail = address_storeback.nb_read(address);
        
        if (data_avail && address_avail) {
            wait(SC_ZERO_TIME);
            head = (head + 1) % capacity;
            if (head == tail) empty = true;
            return true;
        } else {
            return false;
        }
    }

    /**
    * @brief This method checks if a certain data with the tag exists in the buffer
    *
    * @param tag The tag to be searched for in the buffer.
    * 
    * @return Returns true if it exists in the buffer, false otherwise.
    */

    bool in_buffer(uint32_t tag) {
        if (conditional) {
            for (unsigned i = 0; i < (tail - head + capacity) % capacity; i++) {
                // std::cout << tag << " + " << tags_storeback[(i+head) % capacity] << std::endl;
                if (tag == tags_storeback[(i+head) % capacity]) return true;
            }
            return false;
        } else {
            if (empty) return false;
            else return true;
        }
    }

    /**
    * @brief This method checks if the buffer is empty
    * 
    * @return Returns true if it is empty, false if not.
    */
    bool is_empty() {
        return empty;
    }
};

#endif
#endif
