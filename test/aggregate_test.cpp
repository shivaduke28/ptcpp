#include "aggregate.h"
#include "sphere.h"
#include "vec3.h"
#include "hit.h"
#include "image.h"
#include "camera.h"
#include "gtest/gtest.h"

TEST(Aggregate, intersect)
{
    Aggregate aggregate;
    aggregate.add(std::make_shared<Sphere>(Vec3(0, 0, -1), 1));
    aggregate.add(std::make_shared<Sphere>(Vec3(0, 0, 4), 1));

    Ray ray1(Vec3(0, 0, -10), Vec3(0, 0, 1));
    Hit hit;
    ASSERT_TRUE(aggregate.intersect(ray1, hit));
    ASSERT_EQ(hit.hitSphere->center.x, 0);
    ASSERT_EQ(hit.hitSphere->center.y, 0);
    ASSERT_EQ(hit.hitSphere->center.z, -1);

    Ray ray2(Vec3(0, 0, 10), Vec3(0, 0, -1));
    ASSERT_TRUE(aggregate.intersect(ray2, hit));
    ASSERT_EQ(hit.hitSphere->center.x, 0);
    ASSERT_EQ(hit.hitSphere->center.y, 0);
    ASSERT_EQ(hit.hitSphere->center.z, 4);
};