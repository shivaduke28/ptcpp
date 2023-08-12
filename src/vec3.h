#ifndef VEC3_H
#define VEC3_H

#include <iostream>
#include <cmath>

class Vec3
{
public:
    double x;
    double y;
    double z;

    Vec3() { x = y = z = 0; };
    Vec3(double _x) { x = y = z = _x; };
    Vec3(double _x, double _y, double _z) : x(_x), y(_y), z(_z){};

    double length() const
    {
        return std::sqrt(x * x + y * y + z * z);
    };

    double length2() const
    {
        return x * x + y * y + z * z;
    };

    Vec3 operator-() const
    {
        return Vec3(-x, -y, -z);
    };

    Vec3 &operator+=(const Vec3 &v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    Vec3 &operator-=(const Vec3 &v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    Vec3 &operator*=(const Vec3 &v)
    {
        x *= v.x;
        y *= v.y;
        z *= v.z;
        return *this;
    }

    Vec3 &operator/=(const Vec3 &v)
    {
        x /= v.x;
        y /= v.y;
        z /= v.z;
        return *this;
    }
};

inline Vec3 operator+(const Vec3 &v1, const Vec3 &v2)
{
    return Vec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
};

inline Vec3 operator-(const Vec3 &v1, const Vec3 &v2)
{
    return Vec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
};

inline Vec3 operator*(const Vec3 &v1, const Vec3 &v2)
{
    return Vec3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
};

inline Vec3 operator/(const Vec3 &v1, const Vec3 &v2)
{
    return Vec3(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z);
};

inline Vec3 operator+(const Vec3 &v, double k)
{
    return Vec3(v.x + k, v.y + k, v.z + k);
};

inline Vec3 operator+(double k, const Vec3 &v)
{
    return v + k;
};

inline Vec3 operator-(const Vec3 &v, double k)
{
    return Vec3(v.x - k, v.y - k, v.z - k);
};

inline Vec3 operator-(double k, const Vec3 &v)
{
    return Vec3(k - v.x, k - v.y, k - v.z);
};

inline Vec3 operator*(const Vec3 &v, double k)
{
    return Vec3(v.x * k, v.y * k, v.z * k);
};

inline Vec3 operator*(double k, const Vec3 &v)
{
    return v * k;
};

inline Vec3 operator/(const Vec3 &v, double k)
{
    return Vec3(v.x / k, v.y / k, v.z / k);
};

inline Vec3 operator/(double k, const Vec3 &v)
{
    return Vec3(k / v.x, k / v.y / k, k / v.z);
};

inline double dot(const Vec3 &v1, const Vec3 &v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

inline Vec3 cross(const Vec3 &v1, const Vec3 &v2)
{
    return Vec3(
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x);
}

inline Vec3 normalize(const Vec3 &v)
{
    return v / v.length();
}

inline void orthonormalBasis(const Vec3 &v1, Vec3 &v2, Vec3 &v3)
{
    if (std::abs(v1.x) > 0.9)
        v2 = Vec3(0, 1, 0);
    else
        v2 = Vec3(1, 0, 0);

    v2 = normalize(v2 - dot(v1, v2) * v1);
    v3 = cross(v1, v2);
}

inline std::ostream &operator<<(std::ostream &stream, const Vec3 &v)
{
    stream << "(" << v.x << "," << v.y << "," << v.z << ")";
    return stream;
}

inline Vec3 world_to_local(const Vec3 &v, const Vec3 &s, const Vec3 &t, const Vec3 &n)
{
    return Vec3(dot(v, s), dot(v, t), dot(v, n));
}

inline Vec3 local_to_world(const Vec3 &v, const Vec3 &s, const Vec3 &t, const Vec3 &n)
{
    Vec3 a = Vec3(s.x, t.x, n.x);
    Vec3 b = Vec3(s.y, t.y, n.y);
    Vec3 c = Vec3(s.z, t.z, n.z);
    return Vec3(dot(v, a), dot(v, b), dot(v, c));
}

#endif