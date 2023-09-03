#include "ray.h"
#include "gtest/gtest.h"

namespace ptcpp::test
{
    TEST(ray, RayTest)
    {
        ray r(vec3(1, 2, 3), vec3(0.2, 0.3, 0.1));
        vec3 r0 = r(0);
        EXPECT_EQ(r0.x, 1);
        EXPECT_EQ(r0.y, 2);
        EXPECT_EQ(r0.z, 3);

        vec3 r2 = r(2);
        EXPECT_EQ(r2.x, 1.4);
        EXPECT_EQ(r2.y, 2.6);
        EXPECT_EQ(r2.z, 3.2);
    }
}