#ifndef RANDOM_H
#define RANDOM_H

#include <random>

namespace ptcpp
{
    std::random_device rnd_dev;
    std::mt19937 mt(rnd_dev());
    std::uniform_real_distribution<> dist(0, 1);

    inline double rnd()
    {
        return dist(mt);
    }
}

#endif