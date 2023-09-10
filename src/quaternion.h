#pragma once
#include <cmath>
#include "vec3.h"
namespace ptcpp
{
    class quaternion
    {
    public:
        double x;
        double y;
        double z;
        double w;

        quaternion() { x = y = z = w = 0; };
        quaternion(double _x, double _y, double _z, double _w) : x(_x), y(_y), z(_z), w(_w){};

        static quaternion axis_x(double theta)
        {
            double half = theta * 0.5;
            double c = cos(half);
            double s = sqrt(1 - c * c);
            return quaternion(s, 0, 0, c);
        }

        static quaternion axis_y(double theta)
        {
            double half = theta * 0.5;
            double c = cos(half);
            double s = sqrt(1 - c * c);
            return quaternion(0, s, 0, c);
        }

        static quaternion axis_z(double theta)
        {
            double half = theta * 0.5;
            double c = cos(half);
            double s = sqrt(1 - c * c);
            return quaternion(0, 0, s, c);
        }

        static quaternion unit()
        {
            return quaternion(0, 0, 0, 1);
        }

        quaternion operator*(const quaternion v) const
        {
            return quaternion(
                x * v.w + y * v.z - z * v.y + w * v.x,
                -x * v.z + y * v.w + z * v.x + w * v.y,
                x * v.y - y * v.x + z * v.w + w * v.z,
                -x * v.x - y * v.y - z * v.z + w * v.w);
        }

        quaternion operator*(const double k) const
        {
            return quaternion(x * k, y * k, z * k, w * k);
        }

        vec3 operator*(const vec3 v) const
        {
            quaternion q = (*this) * quaternion(v.x, v.y, v.z, 0) * ((*this).inverse());
            return vec3(q.x, q.y, q.z);
        }

        quaternion operator/(const double k) const
        {
            return *this * (1.0 / k);
        }

        quaternion adjoint() const
        {
            return quaternion(-x, -y, -z, w);
        }

        double length() const
        {
            return sqrt(x * x + y * y + z * z + w * w);
        }

        double length2() const
        {
            return x * x + y * y + z * z + w * w;
        }

        quaternion inverse() const
        {
            return adjoint() / length2();
        }

        quaternion normalize() const
        {
            return *this / length();
        }
    };
}