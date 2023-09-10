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
#include "random.h"
#include <float.h>

#define NEE 1

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
        if (rnd() >= ROULETTE)
            break;
        throughput /= ROULETTE;

        hit ray_hit;
        if (aggregate.intersect(ra, ray_hit))
        {
            vec3 normal = ray_hit.normal;
            vec3 position = ray_hit.position;
            vec3 s, t;
            orthonormal_basis(normal, s, t);
            vec3 wo_local = world_to_local(-ra.direction, s, normal, t);

            auto material = ray_hit.material;
            auto light = ray_hit.light;
            if (light->enable)
            {
#if NEE
                if (depth == 0)
                {
                    col += throughput * light->Le();
                }
#else
                col += throughput * light->Le();
#endif
                break;
            }

#if NEE
            double light_pdf;
            vec3 light_pos = aggregate.sample_light(light_pdf);

            vec3 diff = light_pos - position;
            double light_distance = diff.length();
            vec3 light_ray_dir = diff / light_distance;
            ray light_ray(position + 0.0001 * normal, light_ray_dir);
            hit light_hit;
            if (aggregate.intersect(light_ray, light_hit))
            {
                // visible
                if (light_hit.t >= light_distance - 0.001)
                {
                    double G = std::abs(dot(light_ray_dir, normal)) *
                               std::abs(dot(-light_ray_dir, light_hit.normal)) / (light_distance * light_distance);
                    vec3 light_ray_dir_local = world_to_local(light_ray_dir, s, normal, t);
                    vec3 light_brdf = material->eval_brdf(wo_local, light_ray_dir_local);
                    col += throughput * light_brdf * G * light_hit.light->Le() / light_pdf;
                }
            }
#endif

            double pdf;
            vec3 wi_local;
            vec3 brdf = material->sample(wo_local, wi_local, pdf);
            double cos = wi_local.y;
            vec3 wi = local_to_world(wi_local, s, normal, t);

            throughput *= brdf * cos / pdf;

            ra = ray(position + 0.0001 * normal, wi);
        }
        else
        {
            col += throughput * vec3(0.00);
            break;
        }
    }

    return col;
}

int main()
{
    // サンプリング数
    const int N = 10;

    image img(512, 512);
    ptcpp::pinhole_camera cam(vec3(0, 2, 4), vec3(0, 0, -1), 1);

    auto white = std::make_shared<ptcpp::diffuse>(vec3(0.9, 0.9, 0.9));
    auto red = std::make_shared<ptcpp::diffuse>(vec3(0.9, 0.1, 0.1));
    auto green = std::make_shared<ptcpp::diffuse>(vec3(0.1, 0.9, 0.1));

    auto light_off = std::make_shared<ptcpp::light>(vec3(0));
    auto light = std::make_shared<ptcpp::light>(vec3(5));

    aggregate aggregate;
    // floor
    aggregate.add(std::make_shared<cube>(vec3(5, 1, 5), vec3(0, -0.5, 0), white, light_off));
    // ceil
    aggregate.add(std::make_shared<cube>(vec3(5, 1, 5), vec3(0, 4.5, 0), white, light_off));
    // wall
    aggregate.add(std::make_shared<cube>(vec3(5, 5, 1), vec3(0, 2, -2.5), white, light_off));
    // right green
    aggregate.add(std::make_shared<cube>(vec3(1, 5, 5), vec3(2.5, 2.5, 0), green, light_off));
    // left red
    aggregate.add(std::make_shared<cube>(vec3(1, 5, 5), vec3(-2.5, 2.5, 0), red, light_off));
    // light
    auto cube_light = std::make_shared<cube>(vec3(1, 0.2, 1), vec3(0, 4, 0), white, light);
    aggregate.add(cube_light);

    aggregate.add(std::make_shared<cube>(vec3(0.8, 2, 0.8), vec3(0.5, 1, -1), quaternion::axis_y(M_PI / 4.0), white, light_off));
    aggregate.add(std::make_shared<cube>(vec3(1.5, 0.5, 0.5), vec3(-0.5, 2.5, 0.2), quaternion::axis_x(M_PI / 4.0), white, light_off));
    aggregate.add(std::make_shared<sphere>(0.5, vec3(-0.5, 0.5, 0), white, light_off));

#pragma omp parallel for schedule(dynamic, 1)
    for (int i = 0; i < img.width; i++)
    {
        for (int j = 0; j < img.height; j++)
        {
            for (int k = 0; k < N; k++)
            {
                double u = (2.0 * (i + rnd()) - img.width) / img.width;
                double v = (2.0 * (j + rnd()) - img.height) / img.height;

                ray ray = cam.get_ray(-u, -v);

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
    img.ppm_output("nee.ppm");
    return 0;
}