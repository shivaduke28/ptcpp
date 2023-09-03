#include "aggregate.h"
#include "sphere.h"
#include "vec3.h"
#include "hit.h"
#include "image.h"
#include "camera.h"
#include "gtest/gtest.h"

namespace ptcpp::test
{
    TEST(aggregate, intersect)
    {
        // aggregate aggregate;
        // aggregate.add(std::make_shared<sphere>(vec3(0, 0, -1), 1));
        // aggregate.add(std::make_shared<sphere>(vec3(0, 0, 4), 1));

        // ray ray1(vec3(0, 0, -10), vec3(0, 0, 1));
        // hit hit;
        // ASSERT_TRUE(aggregate.intersect(ray1, hit));
        // ASSERT_EQ(hit.hit_sphere->center.x, 0);
        // ASSERT_EQ(hit.hit_sphere->center.y, 0);
        // ASSERT_EQ(hit.hit_sphere->center.z, -1);

        // ray ray2(vec3(0, 0, 10), vec3(0, 0, -1));
        // ASSERT_TRUE(aggregate.intersect(ray2, hit));
        // ASSERT_EQ(hit.hit_sphere->center.x, 0);
        // ASSERT_EQ(hit.hit_sphere->center.y, 0);
        // ASSERT_EQ(hit.hit_sphere->center.z, 4);
    };
}
