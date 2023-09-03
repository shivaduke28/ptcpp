#include "image.h"
#include "camera.h"
#include "vec3.h"
#include "diffuse.h"
#include "light.h"
#include <memory>
#include "aggregate.h"
#include <omp.h>
#include <algorithm>

using namespace ptcpp;

const int MAX_DEPTH = 500;
const double ROULETTE = 0.9;

vec3 radiance(const ray &init_ray, const aggregate &aggregate)
{
    vec3 col;
    vec3 throughput(1);
    ray ra = init_ray;

    for (int depth = 0; depth < MAX_DEPTH; depth++)
    {
        hit res;
        if (aggregate.intersect(ra, res))
        {
            vec3 n = res.hit_normal;
            vec3 s, t;
            orthonormal_basis(n, s, t);
            vec3 wo_local = world_to_local(-ra.direction, s, n, t);

            auto hitMaterial = res.material;
            auto hitLight = res.light;
            col += throughput * hitLight->Le();

            vec3 brdf;
            double pdf;
            vec3 wi_local;
            brdf = hitMaterial->sample(wo_local, wi_local, pdf);
            double cos = wi_local.y;
            vec3 wi = local_to_world(wi_local, s, n, t);

            throughput *= brdf * cos / pdf;

            ra = ray(res.hit_pos + 0.001 * res.hit_normal, wi);
        }
        else
        {
            col += throughput * vec3(0.001);
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

    image img(512, 512);
    ptcpp::pinhole_camera cam(vec3(0, 0, 1), vec3(0, 0, -1), 1);

    auto mat1 = std::make_shared<ptcpp::diffuse>(vec3(0.9, 0.9, 0.9));
    auto mat2 = std::make_shared<ptcpp::diffuse>(vec3(0.2, 0.2, 0.8));

    auto light1 = std::make_shared<ptcpp::light>(vec3(0));
    auto light2 = std::make_shared<ptcpp::light>(vec3(0.2, 0.2, 0.8));

    aggregate aggregate;
    aggregate.add(std::make_shared<sphere>(vec3(0, -10001, 0), 10000, mat1, light1));
    aggregate.add(std::make_shared<sphere>(vec3(0, 0, -3), 1, mat2, light2));

#pragma omp parallel for schedule(dynamic, 1)
    for (int i = 0; i < img.width; i++)
    {
        for (int j = 0; j < img.height; j++)
        {
            for (int k = 0; k < N; k++)
            {
                double u = (2.0 * (i + rnd()) - img.width) / img.width;
                double v = (2.0 * (j + rnd()) - img.height) / img.height;

                ray ray = cam.getRay(-u, -v);

                vec3 col = radiance(ray, aggregate);
                img.add_pixel(i, j, col);
            }

            if (omp_get_thread_num() == 0)
            {
                std::cout << double(j + i * img.height) / (img.width * img.height) * 100 << "\r" << std::flush;
            }
        }
    }

    img.divide(N);
    img.gamma_correction();
    img.ppm_output("path_tracing.ppm");
    return 0;
}