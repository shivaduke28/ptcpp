#pragma once
#include "vec3.h"
#include "quaternion.h"
#include "ray.h"

namespace ptcpp
{
    class transform
    {

    public:
        vec3 position;
        quaternion rotation;

        transform(vec3 _position) : position(_position)
        {
            rotation = quaternion::unit();
        };

        transform(vec3 _position, quaternion _rotation) : position(_position), rotation(_rotation){};

        vec3 world_to_local(const vec3 &p) const
        {
            return rotation.inverse() * (p - position);
        };

        vec3 world_to_local_dir(const vec3 &d) const
        {
            return rotation.inverse() * d;
        };

        ray world_to_local(const ray &r) const
        {
            return ray(world_to_local(r.origin), rotation.inverse() * r.direction);
        };

        vec3 local_to_world(const vec3 &p) const
        {
            return (rotation * p) + position;
        };

        vec3 local_to_world_dir(const vec3 &d) const
        {
            return rotation * d;
        };

        ray local_to_world(const ray &r) const
        {
            return ray(local_to_world(r.origin), rotation * r.direction);
        };
    };

}