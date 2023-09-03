#ifndef LIGHT_H
#define LIGHT_H

#include "vec3.h"
namespace ptcpp
{
    class light
    {
    public:
        vec3 color;

        light(const vec3 &_color) : color(_color){};

        vec3 Le() const
        {
            return color;
        };
    };
}
#endif