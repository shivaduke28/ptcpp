#include "vec3.h"
#include "gtest/gtest.h"
namespace ptcpp::test
{
    TEST(vec3, ctor)
    {
        vec3 v1;
        EXPECT_EQ(v1.x, 0);
        EXPECT_EQ(v1.y, 0);
        EXPECT_EQ(v1.z, 0);

        vec3 v2(2);
        EXPECT_EQ(v2.x, 2);
        EXPECT_EQ(v2.y, 2);
        EXPECT_EQ(v2.z, 2);

        vec3 v3(1, 2, 3);
        EXPECT_EQ(v3.x, 1);
        EXPECT_EQ(v3.y, 2);
        EXPECT_EQ(v3.z, 3);
    }

    TEST(vec3, length)
    {
        EXPECT_EQ(vec3(1).length(), std::sqrt(3));
        EXPECT_EQ(vec3(1).length2(), 3);
    }

    TEST(vec3, ONB)
    {
        vec3 v1(1, 0, 0);
        vec3 v2;
        vec3 v3;
        orthonormal_basis(v1, v2, v3);
        EXPECT_DOUBLE_EQ(v2.x, 0);
        EXPECT_DOUBLE_EQ(v2.y, 1);
        EXPECT_DOUBLE_EQ(v2.z, 0);
        EXPECT_DOUBLE_EQ(v3.x, 0);
        EXPECT_DOUBLE_EQ(v3.y, 0);
        EXPECT_DOUBLE_EQ(v3.z, 1);

        orthonormal_basis(v2, v1, v3);

        EXPECT_DOUBLE_EQ(v1.x, 1);
        EXPECT_DOUBLE_EQ(v1.y, 0);
        EXPECT_DOUBLE_EQ(v1.z, 0);
        EXPECT_DOUBLE_EQ(v3.x, 0);
        EXPECT_DOUBLE_EQ(v3.y, 0);
        EXPECT_DOUBLE_EQ(v3.z, -1);

        v1 = normalize(vec3(1, 1, 1));
        orthonormal_basis(v1, v2, v3);
        EXPECT_NEAR(dot(v1, v2), 0.0, 1e-10);
        EXPECT_NEAR(dot(v2, v3), 0.0, 1e-10);
        EXPECT_NEAR(dot(v3, v1), 0.0, 1e-10);
        EXPECT_NEAR(dot(v2, v2), 1.0, 1e-10);
        EXPECT_NEAR(dot(v3, v3), 1.0, 1e-10);

        vec3 w1 = local_to_world(vec3(1, 0, 0), v1, v2, v3);
        EXPECT_NEAR(v1.x, w1.x, 1e-10);
        EXPECT_NEAR(v1.y, w1.y, 1e-10);
        EXPECT_NEAR(v1.z, w1.z, 1e-10);

        vec3 w2 = local_to_world(vec3(0, 1, 0), v1, v2, v3);
        EXPECT_NEAR(v2.x, w2.x, 1e-10);
        EXPECT_NEAR(v2.y, w2.y, 1e-10);
        EXPECT_NEAR(v2.z, w2.z, 1e-10);

        vec3 w3 = local_to_world(vec3(0, 0, 1), v1, v2, v3);
        EXPECT_NEAR(v3.x, w3.x, 1e-10);
        EXPECT_NEAR(v3.y, w3.y, 1e-10);
        EXPECT_NEAR(v3.z, w3.z, 1e-10);

        vec3 a1 = world_to_local(v1, v1, v2, v3);

        EXPECT_NEAR(a1.x, 1, 1e-10);
        EXPECT_NEAR(a1.y, 0, 1e-10);
        EXPECT_NEAR(a1.z, 0, 1e-10);

        vec3 a2 = world_to_local(v2, v1, v2, v3);

        EXPECT_NEAR(a2.y, 1, 1e-10);
        EXPECT_NEAR(a2.x, 0, 1e-10);
        EXPECT_NEAR(a2.z, 0, 1e-10);

        vec3 a3 = world_to_local(v3, v1, v2, v3);

        EXPECT_NEAR(a3.z, 1, 1e-10);
        EXPECT_NEAR(a3.x, 0, 1e-10);
        EXPECT_NEAR(a3.y, 0, 1e-10);
    }
}