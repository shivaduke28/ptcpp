#include "vec3.hpp"
#include "ray.hpp"
#include "sphere.hpp"
#include <cassert>

int main()
{
    Sphere s(Vec3(0, 0, 0), 1);
    Ray ray(Vec3(0, 0, -3), Vec3(0, 0, 1));
    Hit res = Hit();
    bool result = s.intersect(ray, res);

    assert(result == true);
    assert(res.hitPos.x == 0);
    assert(res.hitPos.y == 0);
    assert(res.hitPos.z == -1);
    assert(res.hitNormal.x == 0);
    assert(res.hitNormal.y == 0);
    assert(res.hitNormal.z == -1);

    Ray ray2(Vec3(0, 0, -3), Vec3(0, 1, 0));
    assert(s.intersect(ray2, res) == false);

    return 0;
};