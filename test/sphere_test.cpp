#include "vec3.h"
#include "ray.h"
#include "sphere.h"
#include "gtest/gtest.h"
namespace ptcpp::test
{
    TEST(sphere, SphereTest)
    {
        // sphere s(vec3(0, 0, 0), 1);
        // ray ray1(vec3(0, 0, -3), vec3(0, 0, 1));
        // hit res = hit();
        // bool result = s.intersect(ray1, res);

        // EXPECT_EQ(result, true);
        // EXPECT_EQ(res.hit_pos.x, 0);
        // EXPECT_EQ(res.hit_pos.y, 0);
        // EXPECT_EQ(res.hit_pos.z, -1);
        // EXPECT_EQ(res.hit_normal.x, 0);
        // EXPECT_EQ(res.hit_normal.y, 0);
        // EXPECT_EQ(res.hit_normal.z, -1);

        // ray ray2(vec3(0, 0, -3), vec3(0, 1, 0));
        // EXPECT_EQ(s.intersect(ray2, res), false);
    };
}