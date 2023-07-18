#ifndef AGGREGATE_H
#define AGGREGATE_H
#include <memory>
#include <vector>
#include "sphere.hpp"

class Aggregate
{
public:
    std::vector<std::shared_ptr<Sphere>> spheres;

    Aggregate(){};

    Aggregate(const std::vector<std::shared_ptr<Sphere>> &_spheres) : spheres(_spheres)
    {
    }

    void add(const std::shared_ptr<Sphere> &s)
    {
        spheres.push_back(s);
    };

    bool intersect(const Ray &ray, Hit &res) const
    {
        bool hit = false;
        for (auto s : spheres)
        {
            Hit res_temp;
            if ((*s).intersect(ray, res_temp))
            {
                hit = true;
                if (res_temp.t < res.t)
                {
                    res = res_temp;
                }
            }
        }
        return hit;
    }
};

#endif