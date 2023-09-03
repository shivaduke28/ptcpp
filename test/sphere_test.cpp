#include "vec3.h"
#include "ray.h"
#include "sphere.h"
#include "gtest/gtest.h"
namespace ptcpp::test
{
    TEST(sphere, SphereTest)
    {
        sphere s(vec3(0, 0, 0), 1);
        ray ray1(vec3(0, 0, -3), vec3(0, 0, 1));
        hit res = hit();
        bool result = s.intersect(ray1, res);

        EXPECT_EQ(result, true);
        EXPECT_EQ(res.hitPos.x, 0);
        EXPECT_EQ(res.hitPos.y, 0);
        EXPECT_EQ(res.hitPos.z, -1);
        EXPECT_EQ(res.hitNormal.x, 0);
        EXPECT_EQ(res.hitNormal.y, 0);
        EXPECT_EQ(res.hitNormal.z, -1);

        ray ray2(vec3(0, 0, -3), vec3(0, 1, 0));
        EXPECT_EQ(s.intersect(ray2, res), false);
    };
}