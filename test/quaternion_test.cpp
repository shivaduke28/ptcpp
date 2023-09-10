#include "quaternion.h"
#include "gtest/gtest.h"
namespace ptcpp::test
{
    TEST(quaternion, ctor)
    {
        quaternion v(1, 2, 3, 4);
        EXPECT_EQ(v.x, 1);
        EXPECT_EQ(v.y, 2);
        EXPECT_EQ(v.z, 3);
        EXPECT_EQ(v.w, 4);
    }

    TEST(quaternion, length)
    {
        quaternion v(1, 1, 1, 1);
        EXPECT_DOUBLE_EQ(v.length(), std::sqrt(4));
        EXPECT_DOUBLE_EQ(v.length2(), 4);
    }

    TEST(quaternion, inverse)
    {
        quaternion u = quaternion::unit();
        quaternion u_inv = u.inverse();
        EXPECT_DOUBLE_EQ(u_inv.x, 0);
        EXPECT_DOUBLE_EQ(u_inv.y, 0);
        EXPECT_DOUBLE_EQ(u_inv.z, 0);
        EXPECT_DOUBLE_EQ(u_inv.w, 1);

        quaternion v(1, 1, 1, 1);
        quaternion v_inv = v.inverse();
        quaternion m = v * v_inv;
        EXPECT_DOUBLE_EQ(m.x, 0);
        EXPECT_DOUBLE_EQ(m.y, 0);
        EXPECT_DOUBLE_EQ(m.z, 0);
        EXPECT_DOUBLE_EQ(m.w, 1);
    }

    TEST(quaternion, rotation)
    {
        double angle = M_PI / 4.0; // 45 deg
        quaternion rot_x = quaternion::axis_x(angle);
        vec3 y(0, 1, 0);
        vec3 rotated = rot_x * y;
        EXPECT_DOUBLE_EQ(rotated.x, 0);
        EXPECT_DOUBLE_EQ(rotated.y, sqrt(2) / 2);
        EXPECT_DOUBLE_EQ(rotated.z, sqrt(2) / 2);
    }
}