#include "ray.hpp"
#include "gtest/gtest.h"

TEST(Ray, RayTest)
{
    Ray r(Vec3(1, 2, 3), Vec3(0.2, 0.3, 0.1));
    Vec3 r0 = r(0);
    EXPECT_EQ(r0.x, 1);
    EXPECT_EQ(r0.y, 2);
    EXPECT_EQ(r0.z, 3);

    Vec3 r2 = r(2);
    EXPECT_EQ(r2.x, 1.4);
    EXPECT_EQ(r2.y, 2.6);
    EXPECT_EQ(r2.z, 3.2);
}
