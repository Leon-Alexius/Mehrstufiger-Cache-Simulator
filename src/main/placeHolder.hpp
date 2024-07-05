// Lie Leon Alexius
// Don't delete this, I need this for testing

#ifndef PLACEHOLDER_HPP
#define PLACEHOLDER_HPP

struct Request {
    __uint32_t addr;
    __uint32_t data;
    int we; // write enabled
};

struct Result {
    size_t cycles;
    size_t misses;
    size_t hits;
    size_t primitiveGateCount;
};

#endif