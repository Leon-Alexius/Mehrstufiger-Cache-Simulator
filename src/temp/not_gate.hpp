#ifndef NOT_GATE_HPP
#define NOT_GATE_HPP

#include <systemc>
using namespace sc_core;

SC_MODULE(NOT_GATE) {
    sc_signal<bool> a;
    sc_signal<bool> out;

    SC_CTOR(NOT_GATE) {
        SC_THREAD(behavior);
        sensitive << a;
    }

    void behavior() {
        while(true) {
            out.write(!(a.read()));
            wait();
        }
    }
};

SC_MODULE(NOT_GADE) {
    sc_in<bool> a;
    sc_out<bool> out;

    SC_CTOR(NOT_GADE) {
        SC_THREAD(behavior);
        sensitive << a;
    }

    void behavior() {
        out->write(!(a->read()));
        wait();
    }
};

#endif