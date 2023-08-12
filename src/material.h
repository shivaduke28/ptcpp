#ifndef MATERIAL_H
#define MATERIAL_H
#include "vec3.h"

class Material
{
public:
    virtual Vec3 sample(const Vec3 &wo, Vec3 &wi, double &pdf) const = 0;
};

#endif