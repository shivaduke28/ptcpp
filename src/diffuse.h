#ifndef DIFFUSE_H
#define DIFFUSE_H

#include "material.h"
#include "random.h"
namespace ptcpp
{
    class diffuse : public material
    {

    public:
        vec3 rho;

        diffuse(const vec3 &_rho) : rho(_rho){};

        vec3 sample(const vec3 &wo, vec3 &wi, double &pdf) const
        {
            double u = rnd();
            double v = rnd();

            double theta = 0.5 * std::acos(1 - 2 * u);
            double phi = 2 * M_PI * v;

            double x = std::cos(phi) * std::sin(theta);
            double y = std::cos(theta);
            double z = std::sin(phi) * std::sin(theta);

            wi = vec3(x, y, z);
            pdf = y * M_1_PI;

            return rho * M_1_PI;
        };

        vec3 sample(const vec3 &wi, double &pdf) const
        {
            pdf = wi.y * M_1_PI;
            return rho * M_1_PI;
        }
    };
}
#endif