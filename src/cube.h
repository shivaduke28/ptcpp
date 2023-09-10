#pragma once

#include "shape.h"
#include "vec3.h"
#include "quaternion.h"
#include "ray.h"
#include "hit.h"
#include "random.h"

namespace ptcpp
{
    class cube : public shape
    {
        vec3 center;
        vec3 side_length;
        quaternion rotation;
        quaternion rotation_inv;

        vec3 world_to_local(const vec3 &p) const
        {
            return rotation_inv * (p - center);
        };

        ray world_to_local(const ray &r) const
        {
            return ray(world_to_local(r.origin), rotation_inv * r.direction);
        };

        vec3 local_to_world(const vec3 &p) const
        {
            return (rotation * p) + center;
        };

        ray local_to_world(const ray &r) const
        {
            return ray(local_to_world(r.origin), rotation * r.direction);
        };

    public:
        cube(const vec3 &_center, vec3 _side_length,
             const std::shared_ptr<ptcpp::material> &_material,
             const std::shared_ptr<ptcpp::light> &_light)
            : center(_center), side_length(_side_length),
              shape(_material, _light)
        {
            rotation = quaternion::unit();
            rotation_inv = rotation.inverse();
            area = (side_length.x * side_length.y + side_length.y * side_length.z + side_length.z * side_length.x) * 2.0;
        };

        cube(const vec3 &_center, vec3 _side_length, quaternion _rotation,
             const std::shared_ptr<ptcpp::material> &_material,
             const std::shared_ptr<ptcpp::light> &_light)
            : center(_center), side_length(_side_length),
              rotation(_rotation),
              shape(_material, _light)
        {
            rotation_inv = rotation.inverse();
            area = (side_length.x * side_length.y + side_length.y * side_length.z + side_length.z * side_length.x) * 2.0;
        };

        bool intersect(const ray &ray, hit &hit) const
        {
            ptcpp::ray ray_local = world_to_local(ray);
            float tmin = 0.0, tmax = INFINITY;
            vec3 min = -side_length * 0.5;
            vec3 max = side_length * 0.5;
            vec3 dir_inv = 1.0 / ray_local.direction;

            vec3 t1 = (min - ray_local.origin) * dir_inv;
            vec3 t2 = (max - ray_local.origin) * dir_inv;

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

            vec3 position_local = ray_local(tmin);
            vec3 normal_local;

            if (tmin == t1x)
            {
                normal_local = position_local.x >= 0 ? vec3(1, 0, 0) : vec3(-1, 0, 0);
            }
            else if (tmin == t1y)
            {
                normal_local = position_local.y >= 0 ? vec3(0, 1, 0) : vec3(0, -1, 0);
            }
            else
            {
                normal_local = position_local.z >= 0 ? vec3(0, 0, 1) : vec3(0, 0, -1);
            }
            hit.normal = rotation * normal_local;
            return true;
        };

        vec3 sample(double &pdf) const
        {
            double r = rnd() * area;
            double area_x = side_length.y * side_length.z;
            double area_y = side_length.z * side_length.x;
            double area_z = side_length.x * side_length.y;

            double a = 0.0;

            double u = rnd() - 0.5;
            double v = rnd() - 0.5;

            double weight(area_x);

            pdf = 1.0 / area;

            if (r < weight)
            {
                return local_to_world(vec3(side_length.x * 0.5, side_length.y * u, side_length.z * v));
            }

            weight += area_x;
            if (r < weight)
            {
                return local_to_world(vec3(-side_length.x * 0.5, side_length.y * u, side_length.z * v));
            }

            weight += area_y;
            if (r < weight)
            {
                return local_to_world(vec3(side_length.x * u, side_length.y * 0.5, side_length.z * v));
            }

            weight += area_y;
            if (r < weight)
            {
                return local_to_world(vec3(side_length.x * u, -side_length.y * 0.5, side_length.z * v));
            }

            weight += area_z;
            if (r < weight)
            {
                return local_to_world(vec3(side_length.x * u, side_length.y * v, side_length.z * 0.5));
            }

            return local_to_world(vec3(side_length.x * u, side_length.y * v, -side_length.z * 0.5));
        };
    };
}