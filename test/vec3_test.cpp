#include "vec3.hpp"
#include "gtest/gtest.h"

TEST(Vec3, ctor)
{
    Vec3 v1;
    EXPECT_EQ(v1.x, 0);
    EXPECT_EQ(v1.y, 0);
    EXPECT_EQ(v1.z, 0);

    Vec3 v2(2);
    EXPECT_EQ(v2.x, 2);
    EXPECT_EQ(v2.y, 2);
    EXPECT_EQ(v2.z, 2);

    Vec3 v3(1, 2, 3);
    EXPECT_EQ(v3.x, 1);
    EXPECT_EQ(v3.y, 2);
    EXPECT_EQ(v3.z, 3);
}

TEST(Vec3, length)
{
    EXPECT_EQ(Vec3(1).length(), std::sqrt(3));
    EXPECT_EQ(Vec3(1).length2(), 3);
}
