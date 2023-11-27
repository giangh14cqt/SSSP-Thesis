#ifndef RANDD_H
#define RANDD_H

#include "Graph.h"

class Random
{
public:
private:
    // Make the default constructor private.
    Random() {}

public:
    // Delete the copy constructor function.
    Random(const Random &) = delete;

    // Delete the overloading of assignment operator
    Random &operator=(const Random &) = delete;

    static Random &Get()
    {
        static Random inst;
        return inst;
    }

    // Seed the random number generator.
    void Seed()
    {
        srand(time(NULL));
    }

    int GenInt()
    {
        return rand();
    }
};

#endif