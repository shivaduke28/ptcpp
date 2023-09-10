#ifndef AGGREGATE_H
#define AGGREGATE_H
#include <memory>
#include <vector>
#include "shape.h"
#include "random.h"
namespace ptcpp
{
    class aggregate
    {
    public:
        std::vector<std::shared_ptr<shape>> shapes;
        std::vector<std::shared_ptr<shape>> light_shapes;
        double light_area;
        double light_area_inv;

        aggregate(){};

        aggregate(const std::vector<std::shared_ptr<shape>> &_shapes) : shapes(_shapes)
        {
            for (auto s : shapes)
            {
                if (s->light->enable)
                {
                    light_shapes.push_back(s);
                    light_area += s->area;
                }
            }
            light_area_inv = 1.0 / light_area;
        }

        void add(const std::shared_ptr<shape> &s)
        {
            shapes.push_back(s);
            if (s->light->enable)
            {
                light_shapes.push_back(s);
                light_area += s->area;
                light_area_inv = 1.0 / light_area;
            }
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

        vec3 sample_light(double &pdf) const
        {
            double r = rnd() * light_area;
            int length = light_shapes.size();
            for (int i = 0; i < length; ++i)
            {
                auto shape = light_shapes[i];
                double a = shape->area;
                if (r < a || i == length - 1)
                {
                    double shape_pdf;
                    vec3 pos = (*shape).sample(shape_pdf);
                    pdf = a / light_area * shape_pdf;
                    return pos;
                }
                r -= a;
            }

            // not reached
            pdf = 1.0;
            return vec3(0, 1, 0);
        }

        vec3 sample_light(double &pdf, vec3 &normal, vec3 &le) const
        {
            double r = rnd() * light_area;
            int length = light_shapes.size();
            pdf = light_area_inv;
            for (int i = 0; i < length; ++i)
            {
                auto shape = light_shapes[i];
                double a = shape->area;
                if (r < a || i == length - 1)
                {
                    double shape_pdf;
                    vec3 pos = (*shape).sample(normal, shape_pdf);
                    le = shape->light->Le();
                    return pos;
                }
                r -= a;
            }

            // not reached
            pdf = 1.0;
            le = vec3(0);
            return vec3(0, 1, 0);
        }
    };
}
#endif