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

namespace ptcpp
{
    class sphere : public shape
    {
    public:
        vec3 center;
        double radius;

        sphere(const vec3 &_center, double _radius, const std::shared_ptr<ptcpp::material> &_material,
               const std::shared_ptr<ptcpp::light> &_light) : center(_center), radius(_radius), shape(_material, _light){};

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
            res.hit_pos = ray(t);
            res.hit_normal = normalize(res.hit_pos - center);
            res.material = material.get();
            res.light = light.get();

            return true;
        };
    };
}

#endif