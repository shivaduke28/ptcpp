
#include "ray.h"
#include "image.h"
#include "gtest/gtest.h"
#include "camera.h"
#include "vec3.h"
#include "sphere.h"

TEST(SingleSphereTracing, Image)
{
    Image img(512, 512);
    PinholeCamera cam(Vec3(0, 0, 4), Vec3(0, 0, -1), 1);
    Sphere sphere(Vec3(0, 0, 0), 1);
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
            if (sphere.intersect(ray, res))
            {
                Vec3 n = res.hitNormal;
                double lambert = std::max(0.0, dot(res.hitNormal, light));
                img.setPixel(i, j, Vec3(1, 1, 1) * lambert);
            }
            else
            {
                img.setPixel(i, j, Vec3(0, 0, 0));
            }
        }
    }
    img.ppmOutput("single_sphere_tracing_test.ppm");
}
