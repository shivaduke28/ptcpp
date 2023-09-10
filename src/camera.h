#ifndef CAMERA_H
#define CAMERA_H

#include "vec3.h"
#include "ray.h"
namespace ptcpp
{
    class camera
    {
    public:
        vec3 cam_pos;
        vec3 cam_forward;
        vec3 cam_right;
        vec3 cam_up;
        camera(const vec3 &_camPos, const vec3 &_camForward) : cam_pos(_camPos), cam_forward(_camForward)
        {
            orthonormal_basis(cam_forward, cam_right, cam_up);
        };

        virtual ray get_ray(double u, double v) const = 0;
    };

    class pinhole_camera : public camera
    {
    public:
        double pinhole_dist;
        pinhole_camera(const vec3 &_camPos, const vec3 &_camForward, double _pinholeDist)
            : camera(_camPos, _camForward), pinhole_dist(_pinholeDist){};

        ray get_ray(double u, double v) const
        {
            vec3 pinholePos = cam_pos + pinhole_dist * cam_forward;
            vec3 sensorPos = cam_pos + u * cam_right + v * cam_up;

            return ray(sensorPos, normalize(pinholePos - sensorPos));
        }
    };
}
#endif