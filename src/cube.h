#pragma once

#include "shape.h"
#include "vec3.h"
#include "ray.h"
#include "hit.h"

namespace ptcpp
{
    class cube : public shape
    {
        vec3 center;
        vec3 side_length;
        // todo: rotation
    public:
        cube(const vec3 &_center, vec3 _side_length,
             const std::shared_ptr<ptcpp::material> &_material,
             const std::shared_ptr<ptcpp::light> &_light)
            : center(_center), side_length(_side_length),
              shape(_material, _light){};

        bool intersect(const ray &ray, hit &hit) const
        {
            float tmin = 0.0, tmax = INFINITY;
            vec3 min = center - side_length * 0.5;
            vec3 max = center + side_length * 0.5;
            vec3 dir_inv = 1.0 / ray.direction;

            vec3 t1 = (min - ray.origin) * dir_inv;
            vec3 t2 = (max - ray.origin) * dir_inv;

            float t1x = std::fmin(t1.x, t2.x);
            float t1y = std::fmin(t1.y, t2.y);
            float t1z = std::fmin(t1.z, t2.z);

            tmin = std::fmax(tmin, std::fmax(std::fmax(t1x, t1y), t1z));
            tmax = std::fmin(tmax, std::fmin(std::fmin(std::fmax(t1.x, t2.x), std::fmax(t1.y, t2.y)), std::fmax(t1.z, t2.z)));

            if (tmin > tmax)
                return false;

            hit.t = tmin;
            hit.position = ray(tmin);
            hit.material = material.get();
            hit.light = light.get();

            if (tmin == t1x)
            {
                hit.normal = hit.position.x >= center.x ? vec3(1, 0, 0) : vec3(-1, 0, 0);
            }
            else if (tmin == t1y)
            {
                hit.normal = hit.position.y >= center.y ? vec3(0, 1, 0) : vec3(0, -1, 0);
            }
            else
            {
                hit.normal = hit.position.z >= center.z ? vec3(0, 0, 1) : vec3(0, 0, -1);
            }
            return true;
        };
    };
}