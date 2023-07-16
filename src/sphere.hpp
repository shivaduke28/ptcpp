#ifndef SPHERE_H
#define SPHERE_H

#include <cmath>
#include "vec3.hpp"
#include "ray.hpp"
#include "hit.hpp"

class Sphere
{
public:
    Vec3 center;
    double radius;

    Sphere(const Vec3 &_center, double _radius) : center(_center), radius(_radius){};

    bool intersect(const Ray &ray, Hit &res) const
    {
        double b = dot(ray.direction, ray.origin - center);
        double c = (ray.origin - center).length2() - radius * radius;
        double D = b * b - c;

        if (D < 0)
            return false;

        double d = std::sqrt(D);

        double t1 = -b - d;
        double t2 = -b + d;

        double t = t1;
        if (d < 0)
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