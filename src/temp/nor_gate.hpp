#ifndef NOR_GATE_HPP
#define NOR_GATE_HPP

#include <systemc>
using namespace sc_core;

SC_MODULE(NOR_GATE) {
    sc_signal<bool> a;
    sc_signal<bool> b;
    sc_signal<bool> out;

    SC_CTOR(NOR_GATE) {
        SC_THREAD(behavior);
        sensitive << a << b;
    }

    void behavior() {
        while(true) {
            out.write(!(a.read() | b.read()));
            wait();
        }
    }
};

SC_MODULE(NOR_GADE) {
    sc_in<bool> a;
    sc_in<bool> b;
    sc_out<bool> out;

    SC_CTOR(NOR_GADE) {
        SC_THREAD(behavior);
        sensitive << a << b;
    }

    void behavior() {
        out->write(!((a->read()) | (b->read())));
        wait();
    }
};

#endif