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

TEST(Aggregate, image)
{
    Image img(512, 512);
    PinholeCamera cam(Vec3(0, 2, 5), Vec3(0, 0, -1), 1);
    Aggregate aggregate;
    aggregate.add(std::make_shared<Sphere>(Vec3(0, 1, -3), 1));
    aggregate.add(std::make_shared<Sphere>(Vec3(0, -1000, -1), 1000));
    aggregate.add(std::make_shared<Sphere>(Vec3(2, 3, -7), 3));
    Vec3 light = normalize(Vec3(1, 1, 1));

    double widthInv = 1.0 / img.width;
    double heigthtInv = 1.0 / img.height;

    for (int i = 0; i < img.width; i++)
    {
        double u = 2.0 * i * widthInv - 1.0;
        for (int j = 0; j < img.height; j++)
        {
            double v = 2.0 * j * heigthtInv - 1.0;
            Ray ray = cam.getRay(-u, -v);
            Hit res;
            if (aggregate.intersect(ray, res))
            {
                Vec3 n = res.hitNormal;
                double lambert = std::max(0.0, dot(res.hitNormal, light));
                img.setPixel(i, j, Vec3(lambert));
            }
            else
            {
                img.setPixel(i, j, Vec3(0, 0, 0));
            }
        }
    }
    img.ppmOutput("aggregate_test.ppm");
}