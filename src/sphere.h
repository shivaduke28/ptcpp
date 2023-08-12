#ifndef SPHERE_H
#define SPHERE_H

#include <cmath>
#include "vec3.h"
#include "ray.h"
#include "hit.h"
#include "material.h"
#include "light.h"
#include <memory>

class Sphere
{
public:
    Vec3 center;
    double radius;
    std::shared_ptr<Material> material; // BRDF
    std::shared_ptr<Light> light;       // Le

    Sphere(const Vec3 &_center, double _radius) : center(_center), radius(_radius){};

    Sphere(const Vec3 &_center, double _radius, const std::shared_ptr<Material> &_material,
           const std::shared_ptr<Light> &_light) : center(_center), radius(_radius), material(_material), light(_light){};

    bool intersect(const Ray &ray, Hit &res) const
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
        res.hitPos = ray(t);
        res.hitNormal = normalize(res.hitPos - center);
        res.hitSphere = this;

        return true;
    };
};

#endif