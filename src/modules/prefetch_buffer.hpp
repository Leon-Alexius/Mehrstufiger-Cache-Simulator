#ifndef PREFETCH_BUFFER_HPP
#define PREFETCH_BUFFER_HPP

#ifdef __cplusplus // added #ifdef __cplusplus so that it works as a c header - anthony
#include <systemc>
#include <vector>

#include "../main/simulator.hpp" // the struct moved here - Leon

// using namespace directives won't get carried over. 
using namespace sc_core;
using namespace std;


SC_MODULE(PREFETCH){

    sc_fifo<char*> prefetch;
    sc_fifo<uint32_t> address_prefetch;
    unsigned head = 0;
    unsigned tail = 0;
    unsigned capacity;

    bool empty = true;

   
    SC_CTOR(PREFETCH);
    PREFETCH(sc_module_name name, unsigned capacity) : sc_module(name), capacity(capacity), prefetch(capacity), address_prefetch(capacity) {

    };

   
    bool write(char* data, uint32_t address) {
        wait(SC_ZERO_TIME);
        wait(SC_ZERO_TIME);
        
        if (prefetch.nb_write(data) && address_prefetch.nb_write(address)) {
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
        bool data_avail = prefetch.nb_read(data);
        bool address_avail = address_prefetch.nb_read(address);
        
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
