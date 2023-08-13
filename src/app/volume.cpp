#include "image.h"
#include "camera.h"
#include "vec3.h"
#include "diffuse.h"
#include "light.h"
#include <memory>
#include "aggregate.h"
#include <omp.h>
#include <algorithm>

const int SUPER_SAMPLING = 100;
const int MAX_DEPTH = 500;
const double ROULETTE = 0.9;
const double SCATTERING = 0.32;
const double ABSORPTION = 0.0;
const double EXTINCTION = SCATTERING + ABSORPTION;
const double HG_G = 0.5;

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

Vec3 sample_Henyey_Greenstein(Vec3 wi, double g, double &pdf)
{
    double phi = 2 * M_PI * rnd();
    double sqrTerm = (1 - g * g) / (1 - g + 2 * g * rnd());
    double cos_theta = (1 + g * g - sqrTerm * sqrTerm) / (2 * g);
    double sin_theta = std::sqrt(1 - cos_theta * cos_theta);

    double x = cos(phi) * sin_theta;
    double y = cos_theta;
    double z = sin_theta * sin(phi);

    Vec3 local(x, y, z);
    Vec3 s, t;
    orthonormalBasis(wi, s, t);

    // pdf = (1.0 - g * g) / (4.0 * M_PI * std::pow(1.0 + g * g + 2 * g * cos_theta, 1.5));
    return local_to_world(local, s, wi, t);
}

Vec3 sample_in_scattering(double &pdf)
{
    double phi = 2 * M_PI * rnd();
    double theta = acos(1 - 2 * rnd());
    double x = sin(theta) * cos(phi);
    double z = sin(theta) * sin(phi);
    double y = cos(theta);
    pdf = 1 / (4.0 * M_PI);
    return Vec3(x, y, z);
}

Vec3 trace(const Ray &init_ray, const Aggregate &aggregate)
{
    Vec3 col;
    Vec3 throughput(1);
    Ray ray = init_ray;

    for (int depth = 0; depth < MAX_DEPTH; depth++)
    {
        Hit hit;
        if (aggregate.intersect(ray, hit))
        {
            double s = sample_distance();

            if (s < hit.t)
            {
                double pdf = 1;
                ray = Ray(ray(s), sample_Henyey_Greenstein(ray.direction, HG_G, pdf));
                // ray = Ray(ray(s), sample_in_scattering(pdf));
                throughput *= SCATTERING / (EXTINCTION);
            }
            else
            {
                Vec3 n = hit.hitNormal;
                Vec3 s, t;
                orthonormalBasis(n, s, t);
                Vec3 wo_local = world_to_local(-ray.direction, s, n, t);

                auto hitMaterial = hit.hitSphere->material;
                auto hitLight = hit.hitSphere->light;
                col += throughput * hitLight->Le();

                Vec3 brdf;
                double pdf;
                Vec3 wi_local;
                brdf = hitMaterial->sample(wo_local, wi_local, pdf);
                double cos = wi_local.y;
                Vec3 wi = local_to_world(wi_local, s, n, t);

                throughput *= brdf * cos / pdf;

                ray = Ray(hit.hitPos + 0.001 * hit.hitNormal, wi);
            }
        }
        else
        {
            col += throughput * Vec3(0.1, 0.1, 0.1);
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
    const int N = SUPER_SAMPLING;

    Image img(512, 512);
    PinholeCamera cam(Vec3(0, 1, 2), Vec3(0, 0, -1), 1);

    auto mat_white = std::make_shared<Diffuse>(Vec3(0.9, 0.9, 0.9));
    auto mat_green = std::make_shared<Diffuse>(Vec3(0.4, 0.9, 0.4));
    auto mat_red = std::make_shared<Diffuse>(Vec3(0.9, 0.4, 0.4));

    auto light_black = std::make_shared<Light>(Vec3(0));
    auto light_white = std::make_shared<Light>(Vec3(1, 1, 1) * 2);
    auto light_blue = std::make_shared<Light>(Vec3(0.1, 0.1, 0.9) * 2);

    Aggregate aggregate;
    aggregate.add(std::make_shared<Sphere>(Vec3(0, 0, 10004), 10000, mat_white, light_black));
    aggregate.add(std::make_shared<Sphere>(Vec3(0, 0, -10004), 10000, mat_white, light_black));
    aggregate.add(std::make_shared<Sphere>(Vec3(0, -10001, 0), 10000, mat_white, light_black));
    aggregate.add(std::make_shared<Sphere>(Vec3(0, 10004, 0), 10000, mat_white, light_black));
    aggregate.add(std::make_shared<Sphere>(Vec3(10003, 0, 0), 10000, mat_green, light_black));
    aggregate.add(std::make_shared<Sphere>(Vec3(-10003, 0, 0), 10000, mat_red, light_black));
    aggregate.add(std::make_shared<Sphere>(Vec3(0, 0.5, -1), 0.5, mat_white, light_black));
    aggregate.add(std::make_shared<Sphere>(Vec3(0, 6.5, -3), 3, mat_white, light_white));

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

                Vec3 col = trace(ray, aggregate);
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
    img.ppmOutput("volume.ppm");
    return 0;
}