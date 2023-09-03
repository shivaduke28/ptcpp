#ifndef VEC3_H
#define VEC3_H

#include <iostream>
#include <cmath>

namespace ptcpp
{
    class vec3
    {
    public:
        double x;
        double y;
        double z;

        vec3() { x = y = z = 0; };
        vec3(double _x) { x = y = z = _x; };
        vec3(double _x, double _y, double _z) : x(_x), y(_y), z(_z){};

        double length() const
        {
            return std::sqrt(x * x + y * y + z * z);
        };

        double length2() const
        {
            return x * x + y * y + z * z;
        };

        vec3 operator-() const
        {
            return vec3(-x, -y, -z);
        };

        vec3 &operator+=(const vec3 &v)
        {
            x += v.x;
            y += v.y;
            z += v.z;
            return *this;
        }

        vec3 &operator-=(const vec3 &v)
        {
            x -= v.x;
            y -= v.y;
            z -= v.z;
            return *this;
        }

        vec3 &operator*=(const vec3 &v)
        {
            x *= v.x;
            y *= v.y;
            z *= v.z;
            return *this;
        }

        vec3 &operator/=(const vec3 &v)
        {
            x /= v.x;
            y /= v.y;
            z /= v.z;
            return *this;
        }
    };

    inline vec3 operator+(const vec3 &v1, const vec3 &v2)
    {
        return vec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
    };

    inline vec3 operator-(const vec3 &v1, const vec3 &v2)
    {
        return vec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
    };

    inline vec3 operator*(const vec3 &v1, const vec3 &v2)
    {
        return vec3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
    };

    inline vec3 operator/(const vec3 &v1, const vec3 &v2)
    {
        return vec3(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z);
    };

    inline vec3 operator+(const vec3 &v, double k)
    {
        return vec3(v.x + k, v.y + k, v.z + k);
    };

    inline vec3 operator+(double k, const vec3 &v)
    {
        return v + k;
    };

    inline vec3 operator-(const vec3 &v, double k)
    {
        return vec3(v.x - k, v.y - k, v.z - k);
    };

    inline vec3 operator-(double k, const vec3 &v)
    {
        return vec3(k - v.x, k - v.y, k - v.z);
    };

    inline vec3 operator*(const vec3 &v, double k)
    {
        return vec3(v.x * k, v.y * k, v.z * k);
    };

    inline vec3 operator*(double k, const vec3 &v)
    {
        return v * k;
    };

    inline vec3 operator/(const vec3 &v, double k)
    {
        return vec3(v.x / k, v.y / k, v.z / k);
    };

    inline vec3 operator/(double k, const vec3 &v)
    {
        return vec3(k / v.x, k / v.y / k, k / v.z);
    };

    inline double dot(const vec3 &v1, const vec3 &v2)
    {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    }

    inline vec3 cross(const vec3 &v1, const vec3 &v2)
    {
        return vec3(
            v1.y * v2.z - v1.z * v2.y,
            v1.z * v2.x - v1.x * v2.z,
            v1.x * v2.y - v1.y * v2.x);
    }

    inline vec3 normalize(const vec3 &v)
    {
        return v / v.length();
    }

    inline void orthonormal_basis(const vec3 &v1, vec3 &v2, vec3 &v3)
    {
        if (std::abs(v1.x) > 0.9)
            v2 = vec3(0, 1, 0);
        else
            v2 = vec3(1, 0, 0);

        v2 = normalize(v2 - dot(v1, v2) * v1);
        v3 = cross(v1, v2);
    }

    inline std::ostream &operator<<(std::ostream &stream, const vec3 &v)
    {
        stream << "(" << v.x << "," << v.y << "," << v.z << ")";
        return stream;
    }

    inline vec3 world_to_local(const vec3 &v, const vec3 &s, const vec3 &t, const vec3 &n)
    {
        return vec3(dot(v, s), dot(v, t), dot(v, n));
    }

    inline vec3 local_to_world(const vec3 &v, const vec3 &s, const vec3 &t, const vec3 &n)
    {
        vec3 a = vec3(s.x, t.x, n.x);
        vec3 b = vec3(s.y, t.y, n.y);
        vec3 c = vec3(s.z, t.z, n.z);
        return vec3(dot(v, a), dot(v, b), dot(v, c));
    }

}
#endif