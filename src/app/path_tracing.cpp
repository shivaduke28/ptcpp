#include "image.h"
#include "camera.h"
#include "vec3.h"
#include "diffuse.h"
#include "light.h"
#include <memory>
#include "aggregate.h"
#include <omp.h>
#include <algorithm>
#include "cube.h"
#include "sphere.h"

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
            vec3 n = res.normal;
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

            ra = ray(res.position + 0.001 * res.normal, wi);
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
    ptcpp::pinhole_camera cam(vec3(0, 2, 4), vec3(0, 0, -1), 1);

    auto white = std::make_shared<ptcpp::diffuse>(vec3(0.9, 0.9, 0.9));
    auto red = std::make_shared<ptcpp::diffuse>(vec3(0.9, 0.1, 0.1));
    auto green = std::make_shared<ptcpp::diffuse>(vec3(0.1, 0.9, 0.1));

    auto light_off = std::make_shared<ptcpp::light>(vec3(0));
    auto light = std::make_shared<ptcpp::light>(vec3(5));

    aggregate aggregate;
    // floor
    aggregate.add(std::make_shared<cube>(vec3(0, -0.5, 0), vec3(10, 1, 10), white, light_off));
    // ceil
    aggregate.add(std::make_shared<cube>(vec3(0, 4.5, 0), vec3(10, 1, 10), white, light_off));
    // wall
    aggregate.add(std::make_shared<cube>(vec3(0, 2, -2.5), vec3(10, 10, 1), white, light_off));
    // right green
    aggregate.add(std::make_shared<cube>(vec3(2.5, 0, 0), vec3(1, 10, 10), green, light_off));
    // left red
    aggregate.add(std::make_shared<cube>(vec3(-2.5, 0, 0), vec3(1, 10, 10), red, light_off));
    // light
    aggregate.add(std::make_shared<cube>(vec3(0, 4, 0), vec3(1, 0.2, 1), white, light));

    aggregate.add(std::make_shared<cube>(vec3(0.5, 1, -1), vec3(0.8, 2, 0.8), white, light_off));
    aggregate.add(std::make_shared<sphere>(vec3(-0.5, 0.5, 0), 0.5, white, light_off));

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