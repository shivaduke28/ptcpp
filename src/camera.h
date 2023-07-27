#ifndef CAMERA_H
#define CAMERA_H

#include "vec3.h"
#include "ray.h"

class Camera
{
public:
    Vec3 camPos;
    Vec3 camForward;
    Vec3 camRight;
    Vec3 camUp;
    Camera(const Vec3 &_camPos, const Vec3 &_camForward) : camPos(_camPos), camForward(_camForward)
    {
        orthonormalBasis(camForward, camRight, camUp);
    };

    virtual Ray getRay(double u, double v) const = 0;
};

class PinholeCamera : public Camera
{
public:
    double pinholeDist;
    PinholeCamera(const Vec3 &_camPos, const Vec3 &_camForward, double _pinholeDist)
        : Camera(_camPos, _camForward), pinholeDist(_pinholeDist){};

    Ray getRay(double u, double v) const
    {
        Vec3 pinholePos = camPos + pinholeDist * camForward;
        Vec3 sensorPos = camPos + u * camRight + v * camUp;

        return Ray(sensorPos, normalize(pinholePos - sensorPos));
    }
};

#endif