#ifndef SPHERE_H
#define SPHERE_H

#include <cmath>
#include "vec3.h"
#include "ray.h"
#include "hit.h"
#include "material.h"
#include "light.h"
#include <memory>
#include "shape.h"
#include "random.h"

namespace ptcpp
{
    class sphere : public shape
    {
        double area_inv;

    public:
        vec3 center;
        double radius;

        sphere(double _radius,
               const vec3 &_center,
               const std::shared_ptr<ptcpp::material> &_material,
               const std::shared_ptr<ptcpp::light> &_light)
            : center(_center), radius(_radius), shape(_material, _light)
        {
            area = 4.0 * M_PI * radius * radius;
            area_inv = 1.0 / area;
        };

        bool intersect(const ray &ray, hit &res) const
        {
            double b = dot(ray.direction, ray.origin - center);
            double c = (ray.origin - center).length2() - radius * radius;
            double D = b * b - c;

            if (D <= 0)
                return false;

            double d = std::sqrt(D);

            double t1 = -b - d;
            double t2 = -b + d;

            if (t1 > 10000 | t2 < 0)
                return false;

            double t = t1;
            if (t < 0)
            {
                t = t2;
                if (t2 > 10000)
                    return false;
            }

            res.t = t;
            res.position = ray(t);
            res.normal = normalize(res.position - center);
            res.material = material.get();
            res.light = light.get();

            return true;
        };

        vec3 sample(double &pdf) const
        {
            // double phi = 2 * M_PI * rnd();
            double cos_phi = cos(2 * M_PI * rnd());
            double cos_theta = 1 - 2 * rnd();
            double sin_theta = std::sqrt(1 - cos_theta * cos_theta);
            // double theta = acos(1 - 2 * rnd());
            double x = sin_theta * cos_phi;
            double z = sin_theta * sqrt(1 - cos_phi * cos_phi);
            double y = cos_theta;
            pdf = area_inv;
            return vec3(x * radius + center.x, y * radius + center.y, z * radius + center.z);
        }

        vec3 sample(vec3 &normal, double &pdf) const
        {
            // double phi = 2 * M_PI * rnd();
            double cos_phi = cos(2 * M_PI * rnd());
            double cos_theta = 1 - 2 * rnd();
            double sin_theta = std::sqrt(1 - cos_theta * cos_theta);
            // double theta = acos(1 - 2 * rnd());
            double x = sin_theta * cos_phi;
            double z = sin_theta * sqrt(1 - cos_phi * cos_phi);
            double y = cos_theta;
            pdf = area_inv;
            normal = vec3(x, y, z);
            return vec3(x * radius + center.x, y * radius + center.y, z * radius + center.z);
        }
    };
}

#endif