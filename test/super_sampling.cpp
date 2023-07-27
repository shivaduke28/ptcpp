#include <memory>
#include <random>
#include <omp.h>
#include "vec3.h"
#include "ray.h"
#include "hit.h"
#include "sphere.h"
#include "camera.h"
#include "image.h"
#include "aggregate.h"

std::random_device rnd_dev;
std::mt19937 mt(rnd_dev());
std::uniform_real_distribution<> dist(0, 1);
inline double rnd()
{
    return dist(mt);
}

int main()
{
    const int N = 100;
    Image img(512, 512);
    PinholeCamera cam(Vec3(0), Vec3(0, 0, -1), 1);

    Aggregate aggregate;
    aggregate.add(std::make_shared<Sphere>(Vec3(0, 0, -3), 1));
    aggregate.add(std::make_shared<Sphere>(Vec3(0, -10001, 0), 10000));

    Vec3 sunDir = normalize(Vec3(1, 1, 1));

    printf("使用可能な最大スレッド数：%d\n", omp_get_max_threads());

#pragma omp parallel for schedule(dynamic, 1)
    for (int k = 0; k < N; k++)
    {
        for (int i = 0; i < img.width; i++)
        {
            for (int j = 0; j < img.height; j++)
            {

                double u = (2.0 * (i + rnd()) - img.width) / img.width;
                double v = (2.0 * (j + rnd()) - img.height) / img.height;

                Ray ray = cam.getRay(-u, -v);

                Vec3 col;
                Hit res;
                if (aggregate.intersect(ray, res))
                {
                    Ray shadowRay(res.hitPos + 0.001 * res.hitNormal, sunDir);
                    Hit res_temp;
                    if (!aggregate.intersect(shadowRay, res_temp))
                    {
                        col = std::max(dot(res.hitNormal, sunDir), 0.0) * Vec3(1);
                    }
                }

                img.addtPixel(i, j, col);
            }
        }
        printf("thread = %d, k = %2d\n", omp_get_thread_num(), k);
    }

    img.divide(N);

    img.ppmOutput("super_sampling.ppm");

    return 0;
}