#pragma once
#include <memory>
#include "material.h"
#include "light.h"
#include "hit.h"
#include "ray.h"

namespace ptcpp
{
    class shape
    {
    public:
        std::shared_ptr<ptcpp::material> material;
        std::shared_ptr<ptcpp::light> light;

        shape(const std::shared_ptr<ptcpp::material> &_material, const std::shared_ptr<ptcpp::light> &_light) : material(_material), light(_light) {}

        virtual bool intersect(const ray &ray, hit &res) const = 0;
    };
}