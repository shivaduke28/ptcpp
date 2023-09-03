#ifndef RAY_H
#define RAY_H

#include <iostream>
#include "vec3.h"

namespace ptcpp
{
    class ray
    {
    public:
        vec3 origin;
        vec3 direction;
        ray(const vec3 &_origin, const vec3 &_direction) : origin(_origin), direction(_direction){};

        vec3 operator()(double t) const
        {
            return origin + t * direction;
        };
    };

    inline std::ostream &operator<<(std::ostream &stream, const ray &ray)
    {
        stream << "origin:" << ray.origin << "direciton:" << ray.direction;
        return stream;
    }
}

#endif