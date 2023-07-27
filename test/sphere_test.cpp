#include "vec3.h"
#include "ray.h"
#include "sphere.h"
#include "gtest/gtest.h"

TEST(Sphere, SphereTest)
{
    Sphere s(Vec3(0, 0, 0), 1);
    Ray ray(Vec3(0, 0, -3), Vec3(0, 0, 1));
    Hit res = Hit();
    bool result = s.intersect(ray, res);

    EXPECT_EQ(result, true);
    EXPECT_EQ(res.hitPos.x, 0);
    EXPECT_EQ(res.hitPos.y, 0);
    EXPECT_EQ(res.hitPos.z, -1);
    EXPECT_EQ(res.hitNormal.x, 0);
    EXPECT_EQ(res.hitNormal.y, 0);
    EXPECT_EQ(res.hitNormal.z, -1);

    Ray ray2(Vec3(0, 0, -3), Vec3(0, 1, 0));
    EXPECT_EQ(s.intersect(ray2, res), false);
};