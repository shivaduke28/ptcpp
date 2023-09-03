#ifndef AGGREGATE_H
#define AGGREGATE_H
#include <memory>
#include <vector>
#include "sphere.h"
namespace ptcpp
{
    class aggregate
    {
    public:
        std::vector<std::shared_ptr<sphere>> spheres;

        aggregate(){};

        aggregate(const std::vector<std::shared_ptr<sphere>> &_spheres) : spheres(_spheres)
        {
        }

        void add(const std::shared_ptr<sphere> &s)
        {
            spheres.push_back(s);
        };

        bool intersect(const ray &ray, hit &res) const
        {
            bool hit = false;
            for (auto s : spheres)
            {
                ptcpp::hit res_temp;
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
}
#endif