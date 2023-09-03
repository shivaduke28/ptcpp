#ifndef HIT_H
#define HIT_H

#include "vec3.h"
#include "material.h"
#include "light.h"

namespace ptcpp
{
    class hit
    {
    public:
        double t;
        vec3 hit_pos;
        vec3 hit_normal;
        const ptcpp::material *material;
        const ptcpp::light *light;

        hit()
        {
            t = 100000;
        };
    };
}
#endif