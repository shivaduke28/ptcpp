#ifndef LIGHT_H
#define LIGHT_H

#include "vec3.h"
namespace ptcpp
{
    class light
    {
        vec3 color;

    public:
        bool enable;

        light(const vec3 &_color) : color(_color)
        {
            enable = color.length2() > 0.0;
        };

        vec3 Le() const
        {
            return color;
        };
    };
}
#endif