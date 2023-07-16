#include "ray.hpp"
#include <cassert>

int main()
{
    Ray r(Vec3(1, 2, 3), Vec3(0.2, 0.3, 0.1));
    Vec3 r0 = r(0);
    assert(r0.x == 1);
    assert(r0.y == 2);
    assert(r0.z == 3);

    Vec3 r2 = r(2);
    assert(r2.x == 1.4);
    assert(r2.y == 2.6);
    assert(r2.z == 3.2);

    return 0;
}
