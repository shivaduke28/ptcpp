#include "image.h"
#include "camera.h"
#include "vec3.h"
#include "diffuse.h"
#include "light.h"
#include <memory>
#include "aggregate.h"
#include <omp.h>
#include <algorithm>

const int MAX_DEPTH = 500;
const double ROULETTE = 0.9;

Vec3 radiance(const Ray &init_ray, const Aggregate &aggregate)
{
    Vec3 col;
    Vec3 throughput(1);
    Ray ray = init_ray;

    for (int depth = 0; depth < MAX_DEPTH; depth++)
    {
        Hit res;
        if (aggregate.intersect(ray, res))
        {
            Vec3 n = res.hitNormal;
            Vec3 s, t;
            orthonormalBasis(n, s, t);
            Vec3 wo_local = world_to_local(-ray.direction, s, n, t);

            auto hitMaterial = res.hitSphere->material;
            auto hitLight = res.hitSphere->light;
            col += throughput * hitLight->Le();

            Vec3 brdf;
            double pdf;
            Vec3 wi_local;
            brdf = hitMaterial->sample(wo_local, wi_local, pdf);
            double cos = wi_local.y;
            Vec3 wi = local_to_world(wi_local, s, n, t);

            throughput *= brdf * cos / pdf;

            ray = Ray(res.hitPos + 0.001 * res.hitNormal, wi);
        }
        else
        {
            col += throughput * Vec3(0.001);
            break;
        }

        if (rnd() >= ROULETTE)
        {
            break;
        }
        else
        {
            throughput /= ROULETTE;
        }
    }

    return col;
}

int main()
{
    // サンプリング数
    const int N = 100;

    Image img(512, 512);
    PinholeCamera cam(Vec3(0, 0, 1), Vec3(0, 0, -1), 1);

    auto mat1 = std::make_shared<Diffuse>(Vec3(0.9, 0.9, 0.9));
    auto mat2 = std::make_shared<Diffuse>(Vec3(0.2, 0.2, 0.8));

    auto light1 = std::make_shared<Light>(Vec3(0));
    auto light2 = std::make_shared<Light>(Vec3(0.2, 0.2, 0.8));

    Aggregate aggregate;
    aggregate.add(std::make_shared<Sphere>(Vec3(0, -10001, 0), 10000, mat1, light1));
    aggregate.add(std::make_shared<Sphere>(Vec3(0, 0, -3), 1, mat2, light2));

#pragma omp parallel for schedule(dynamic, 1)
    for (int i = 0; i < img.width; i++)
    {
        for (int j = 0; j < img.height; j++)
        {
            for (int k = 0; k < N; k++)
            {
                double u = (2.0 * (i + rnd()) - img.width) / img.width;
                double v = (2.0 * (j + rnd()) - img.height) / img.height;

                Ray ray = cam.getRay(-u, -v);

                Vec3 col = radiance(ray, aggregate);
                img.addPixel(i, j, col);
            }

            if (omp_get_thread_num() == 0)
            {
                std::cout << double(j + i * img.height) / (img.width * img.height) * 100 << "\r" << std::flush;
            }
        }
    }

    img.divide(N);
    img.gammaCorrection();
    img.ppmOutput("path_tracing.ppm");
    return 0;
}