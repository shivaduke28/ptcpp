#ifndef MATERIAL_H
#define MATERIAL_H
#include "vec3.h"
namespace ptcpp
{
    class material
    {
    public:
        virtual vec3 sample(const vec3 &wo, vec3 &wi, double &pdf) const = 0;
        virtual vec3 sample(const vec3 &wi, double &pdf) const = 0;
    };
}
#endif