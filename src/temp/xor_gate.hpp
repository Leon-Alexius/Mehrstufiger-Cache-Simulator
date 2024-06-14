#ifndef XOR_GATE_HPP
#define XOR_GATE_HPP

#include <systemc>
using namespace sc_core;

SC_MODULE(XOR_GATE) {
    sc_signal<bool> a;
    sc_signal<bool> b;
    sc_signal<bool> out;

    SC_CTOR(XOR_GATE) {
        SC_THREAD(behavior);
        sensitive << a << b;
    }

    void behavior() {
        while(true) {
            out.write(a.read() ^ b.read());
            wait();
        }
    }
};

SC_MODULE(XOR_GADE) {
    sc_in<bool> a;
    sc_in<bool> b;
    sc_out<bool> out;

    SC_CTOR(XOR_GADE) {
        SC_THREAD(behavior);
        sensitive << a << b;
    }

    void behavior() {
        out->write((a->read()) ^ (b->read()));
        wait();
    }
};

#endif