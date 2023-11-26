#include <iomanip>
#include <iostream>
#include <map>
#include <random>
#include <string>

#include "LDD.h"

int main()
{
    std::random_device rd;
    std::mt19937 gen(rd());

    std::geometric_distribution<> d(0.5);
    // same as
    // std::negative_binomial_distribution<> d(1, 0.5):

    std::map<int, int> hist;
    for (int n = 0; n != 10000; ++n)
        ++hist[d(gen)];

    for (auto [x, y] : hist)
    {
        const char c = x < 10 ? x + '0' : x - 10 + 'a';
        std::cout << c << ' ' << std::string(y / 100, '*') << '\n';
    }
}