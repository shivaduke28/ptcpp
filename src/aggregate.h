#ifndef AGGREGATE_H
#define AGGREGATE_H
#include <memory>
#include <vector>
#include "shape.h"
namespace ptcpp
{
    class aggregate
    {
    public:
        std::vector<std::shared_ptr<shape>> shapes;
        double area;

        aggregate(){};

        aggregate(const std::vector<std::shared_ptr<shape>> &_spheres) : shapes(_spheres)
        {
        }

        void add(const std::shared_ptr<shape> &s)
        {
            shapes.push_back(s);
            area += (*s).area;
        };

        bool intersect(const ray &ray, hit &res) const
        {
            bool hit = false;
            for (auto s : shapes)
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