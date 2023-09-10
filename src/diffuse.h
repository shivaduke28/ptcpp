#ifndef DIFFUSE_H
#define DIFFUSE_H

#include "material.h"
#include "random.h"
#include "cmath"
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

            double cos_theta = std::sqrt(1.0 - u);
            double sin_theta = std::sqrt(u);
            double phi = 2 * M_PI * v;
            double cos_phi = std::cos(phi);
            double sin_phi = std::sqrt(1.0 - cos_phi * cos_phi);

            wi = vec3(cos_phi * sin_theta, cos_theta, sin_phi * sin_theta);
            pdf = cos_theta * M_1_PI;

            return rho * M_1_PI;
        };

        vec3 eval_brdf(const vec3 &wo, const vec3 &wi) const
        {
            return rho * M_1_PI;
        }

        vec3 eval_brdf(const vec3 &wo, const vec3 &wi, double &pdf) const
        {
            pdf = std::max(0.0, wi.y) * M_1_PI;
            return rho * M_1_PI;
        }
    };
}
#endif