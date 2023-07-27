#include "vec3.h"
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

TEST(Vec3, ONB)
{
    Vec3 v1(1, 0, 0);
    Vec3 v2;
    Vec3 v3;
    orthonormalBasis(v1, v2, v3);
    EXPECT_DOUBLE_EQ(v2.x, 0);
    EXPECT_DOUBLE_EQ(v2.y, 1);
    EXPECT_DOUBLE_EQ(v2.z, 0);
    EXPECT_DOUBLE_EQ(v3.x, 0);
    EXPECT_DOUBLE_EQ(v3.y, 0);
    EXPECT_DOUBLE_EQ(v3.z, 1);

    orthonormalBasis(v2, v1, v3);

    EXPECT_DOUBLE_EQ(v1.x, 1);
    EXPECT_DOUBLE_EQ(v1.y, 0);
    EXPECT_DOUBLE_EQ(v1.z, 0);
    EXPECT_DOUBLE_EQ(v3.x, 0);
    EXPECT_DOUBLE_EQ(v3.y, 0);
    EXPECT_DOUBLE_EQ(v3.z, -1);

    v1 = normalize(Vec3(1, 1, 1));
    orthonormalBasis(v1, v2, v3);
    EXPECT_NEAR(dot(v1, v2), 0.0, 1e-10);
    EXPECT_NEAR(dot(v2, v3), 0.0, 1e-10);
    EXPECT_NEAR(dot(v3, v1), 0.0, 1e-10);
    EXPECT_NEAR(dot(v2, v2), 1.0, 1e-10);
    EXPECT_NEAR(dot(v3, v3), 1.0, 1e-10);
}
