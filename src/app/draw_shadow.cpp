#include <memory>
#include "vec3.h"
#include "ray.h"
#include "hit.h"
#include "sphere.h"
#include "image.h"
#include "aggregate.h"
#include "camera.h"

int main()
{
    Image img(512, 512);
    PinholeCamera cam(Vec3(0, 2, 3), Vec3(0, 0, -1), 1);
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
                Vec3 p = res.hitPos;
                Vec3 n = res.hitNormal;
                Ray r(p + n * 0.001, light);
                Hit shadowRes;
                if (aggregate.intersect(r, shadowRes))
                {
                    double lambert = std::max(0.0, dot(res.hitNormal, light));
                    img.setPixel(i, j, Vec3(lambert * 0.1));
                }
                else
                {
                    double lambert = std::max(0.0, dot(res.hitNormal, light));
                    img.setPixel(i, j, Vec3(lambert));
                }
            }
            else
            {
                img.setPixel(i, j, Vec3(0, 0, 0));
            }
        }
    }
    img.ppmOutput("draw_shadow.ppm");
    return 0;
}
