#include "image.h"
#include "camera.h"
#include "vec3.h"
#include "sphere.h"
#include "quaternion.h"
#include "diffuse.h"
#include "light.h"
#include "cube.h"
#include "aggregate.h"
#include <memory>
#include <omp.h>
#include <algorithm>
using namespace ptcpp;
const int SUPER_SAMPLING = 1000;
const int MAX_DEPTH = 500;
const double ROULETTE = 0.9;
const double SCATTERING = 0.15;
const double ABSORPTION = 0.0;
const double EXTINCTION = SCATTERING + ABSORPTION;
const double HG_G = 0.8;

double sample_distance()
{
    double xi = rnd();
    double s = -std::log(1 - xi) / EXTINCTION;
    return s;
}

double transmittance(double distance)
{
    return std::exp(-EXTINCTION * distance);
}

vec3 sample_Henyey_Greenstein(vec3 wi, double g)
{
    double phi = 2 * M_PI * rnd();
    double sqrTerm = (1 - g * g) / (1 + g - 2 * g * rnd());
    double cos_theta = -(1 + g * g - sqrTerm * sqrTerm) / (2 * g);
    double sin_theta = std::sqrt(1 - cos_theta * cos_theta);

    double cos_phi = cos(phi);
    double x = cos_phi * sin_theta;
    double y = cos_theta;
    double z = sin_theta * sqrt(1 - cos_phi * cos_phi);

    vec3 local(x, y, z);
    vec3 s, t;
    orthonormal_basis(wi, s, t);

    // pdf = (1.0 - g * g) / (4.0 * M_PI * std::pow(1.0 + g * g + 2 * g * cos_theta, 1.5));
    return local_to_world(local, s, wi, t);
}

vec3 sample_in_scattering(double &pdf)
{
    double phi = 2 * M_PI * rnd();
    double theta = acos(1 - 2 * rnd());
    double x = sin(theta) * cos(phi);
    double z = sin(theta) * sin(phi);
    double y = cos(theta);
    pdf = 1 / (4.0 * M_PI);
    return vec3(x, y, z);
}

vec3 trace(const ray &init_ray, const aggregate &aggregate)
{
    vec3 col;
    vec3 throughput(1);
    ray ra = init_ray;

    for (int depth = 0; depth < MAX_DEPTH; depth++)
    {
        hit hit;
        if (aggregate.intersect(ra, hit))
        {
            double s = sample_distance();

            if (s < hit.t)
            {
                ra = ray(ra(s), sample_Henyey_Greenstein(ra.direction, HG_G));
                // ray = Ray(ray(s), sample_in_scattering(pdf));
                throughput *= SCATTERING / (EXTINCTION);
            }
            else
            {
                vec3 n = hit.normal;
                vec3 s, t;
                orthonormal_basis(n, s, t);
                vec3 wo_local = world_to_local(-ra.direction, s, n, t);

                auto hitMaterial = hit.material;
                auto hitLight = hit.light;
                if (hitLight->enable)
                {
                    col += throughput * hitLight->Le();
                    break;
                }

                vec3 brdf;
                double pdf;
                vec3 wi_local;
                brdf = hitMaterial->sample(wo_local, wi_local, pdf);
                double cos = wi_local.y;
                vec3 wi = local_to_world(wi_local, s, n, t);

                throughput *= brdf * cos / pdf;

                ra = ray(hit.position + 0.001 * hit.normal, wi);
            }
        }
        else
        {
            col += throughput * vec3(0.1, 0.1, 0.1);
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

                ray ray = cam.getRay(-u, -v);

                vec3 col = trace(ray, aggregate);
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
    img.ppm_output("volume.ppm");
    return 0;
};