#include <memory>
#include "vec3.h"
#include "ray.h"
#include "hit.h"
#include "sphere.h"
#include "image.h"
#include "aggregate.h"
#include "camera.h"
namespace ptcpp::app
{
    int main()
    {
        image img(512, 512);
        pinhole_camera cam(vec3(0, 2, 3), vec3(0, 0, -1), 1);
        aggregate aggregate;
        aggregate.add(std::make_shared<sphere>(vec3(0, 1, -3), 1));
        aggregate.add(std::make_shared<sphere>(vec3(0, -1000, -1), 1000));
        aggregate.add(std::make_shared<sphere>(vec3(2, 3, -7), 3));

        vec3 light = normalize(vec3(1, 1, 1));
        double widthInv = 1.0 / img.width;
        double heigthtInv = 1.0 / img.height;

        for (int i = 0; i < img.width; i++)
        {
            double u = 2.0 * i * widthInv - 1.0;
            for (int j = 0; j < img.height; j++)
            {
                double v = 2.0 * j * heigthtInv - 1.0;
                ray ray1 = cam.getRay(-u, -v);
                hit res;
                if (aggregate.intersect(ray1, res))
                {
                    vec3 p = res.hit_pos;
                    vec3 n = res.hit_normal;
                    ray r(p + n * 0.001, light);
                    hit shadowRes;
                    if (aggregate.intersect(r, shadowRes))
                    {
                        double lambert = std::max(0.0, dot(res.hit_normal, light));
                        img.set_pixel(i, j, vec3(lambert * 0.1));
                    }
                    else
                    {
                        double lambert = std::max(0.0, dot(res.hit_normal, light));
                        img.set_pixel(i, j, vec3(lambert));
                    }
                }
                else
                {
                    img.set_pixel(i, j, vec3(0, 0, 0));
                }
            }
        }
        img.ppm_output("draw_shadow.ppm");
        return 0;
    }
}