#include <iostream>
#include "vec3.hpp"
#include <cassert>

int main()
{
    Vec3 v1;
    assert(v1.x == 0);
    assert(v1.y == 0);
    assert(v1.z == 0);

    Vec3 v2(2);
    assert(v2.x == 2);
    assert(v2.y == 2);
    assert(v2.z == 2);

    Vec3 v3(1, 2, 3);
    assert(v3.x == 1);
    assert(v3.y == 2);
    assert(v3.z == 3);

    assert(Vec3(1).length() == std::sqrt(3));
    assert(Vec3(1).length2() == 3);

    Vec3 v4 = v2 + v3;
    assert(v4.x == 3);
    assert(v4.y == 4);
    assert(v4.z == 5);

    assert(dot(v3, v3) == 1 + 4 + 9);
}