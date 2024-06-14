#ifndef NAND_GATE_HPP
#define NAND_GATE_HPP

#include <systemc>
using namespace sc_core;

SC_MODULE(NAND_GATE) {
    sc_signal<bool> a;
    sc_signal<bool> b;
    sc_signal<bool> out;

    SC_CTOR(NAND_GATE) {
        SC_THREAD(behavior);
        sensitive << a << b;
    }

    void behavior() {
        while(true) {
            out.write(!(a.read() & b.read()));
            wait();
        }
    }
};

SC_MODULE(NAND_GADE) {
    sc_in<bool> a;
    sc_in<bool> b;
    sc_out<bool> out;

    SC_CTOR(NAND_GADE) {
        SC_THREAD(behavior);
        sensitive << a << b;
    }

    void behavior() {
        out->write(!((a->read()) & (b->read())));
        wait();
    }
};

#endif