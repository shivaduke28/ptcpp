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
#include <string>
using namespace ptcpp;

const int MAX_DEPTH = 500;
const double ROULETTE = 0.9;
const double ROULETTE_INV = 1.0 / ROULETTE;
const double SCATTERING = 0.15;
const double ABSORPTION = 0.0;
const double EXTINCTION = SCATTERING + ABSORPTION;
const double EXTINCTION_INV = 1.0 / EXTINCTION;
const double SCATTERING_ALBEDO = SCATTERING * EXTINCTION_INV;
const double HG_G = 0.5;

double sample_distance()
{
    double xi = rnd();
    double s = -std::log(1 - xi) * EXTINCTION_INV;
    return s;
}

double transmittance(double distance)
{
    return std::exp(-EXTINCTION * distance);
}

vec3 sample_henyey_greenstein(vec3 wi, double g, double &pdf)
{
    double phi = 2.0 * M_PI * rnd();
    double sqr_term = (1.0 - g * g) / (1.0 + g - 2.0 * g * rnd());
    double cos_theta = -(1.0 + g * g - sqr_term * sqr_term) / (2.0 * g);
    double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

    double cos_phi = cos(phi);
    double x = sin_theta * cos_phi;
    double y = cos_theta;
    double z = sin_theta * sqrt(1 - cos_phi * cos_phi);

    vec3 local(x, y, z);
    vec3 s, t;
    orthonormal_basis(wi, s, t);

    pdf = (1.0 - g * g) / (4.0 * M_PI * std::pow(1.0 + g * g + 2 * g * cos_theta, 1.5));
    return local_to_world(local, s, wi, t);
}

double eval_henyey_greenstein(double cos_theta, double g)
{
    double g2 = g * g;
    return (1.0 - g2) / (4.0 * M_PI * pow(1.0 + g2 + 2.0 * g * cos_theta, 1.5));
}

// not used
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
    double last_pt_pdf;

    for (int depth = 0; depth < MAX_DEPTH; depth++)
    {
        if (rnd() >= ROULETTE)
            break;
        throughput *= ROULETTE_INV;

        hit ray_hit;
        if (aggregate.intersect(ra, ray_hit))
        {
            double s = sample_distance();
            if (s < ray_hit.t)
            {
                throughput *= SCATTERING_ALBEDO;

                // NEE
                {
                    double light_pdf;
                    vec3 light_le;
                    vec3 light_normal;
                    vec3 light_pos = aggregate.sample_light(light_pdf, light_normal, light_le);

                    vec3 position = ra(s);
                    vec3 diff = light_pos - position;
                    double light_distance = diff.length();
                    vec3 light_ray_dir = diff / light_distance;
                    ray light_ray(position, light_ray_dir);

                    hit light_hit;
                    if (aggregate.intersect(light_ray, light_hit))
                    {
                        // visible
                        if (light_hit.t >= light_distance - 0.001)
                        {
                            double G_over_cos_theta = std::abs(dot(-light_ray_dir, light_normal)) / (light_distance * light_distance);
                            double phase = eval_henyey_greenstein(dot(-ra.direction, light_ray_dir), HG_G);
                            double mis_weight = light_pdf / (light_pdf + phase * G_over_cos_theta);
                            col += mis_weight * transmittance(light_distance) * throughput * phase * G_over_cos_theta * light_le / light_pdf;
                        }
                    }
                }
                ra = ray(ra(s), sample_henyey_greenstein(-ra.direction, HG_G, last_pt_pdf));
            }
            else
            {

                vec3 normal = ray_hit.normal;
                vec3 position = ray_hit.position;
                auto material = ray_hit.material;
                auto light = ray_hit.light;

                vec3 s, t;
                orthonormal_basis(normal, s, t);
                vec3 wo_local = world_to_local(-ra.direction, s, normal, t);

                // path tracing
                if (light->enable)
                {
                    if (depth == 0)
                    {
                        col += throughput * light->Le();
                    }
                    else
                    {
                        double cos_theta_over_G = ray_hit.t * ray_hit.t / std::abs(wo_local.y);
                        double mis_weight = last_pt_pdf / (last_pt_pdf + aggregate.light_area_inv * cos_theta_over_G);
                        col += throughput * light->Le() * mis_weight;
                    }
                    break;
                }

                // NEE
                {
                    double light_pdf;
                    vec3 light_le;
                    vec3 light_normal;
                    vec3 light_pos = aggregate.sample_light(light_pdf, light_normal, light_le);

                    vec3 diff = light_pos - position;
                    double light_distance = diff.length();
                    vec3 light_ray_dir = diff / light_distance;
                    ray light_ray(position + 0.0001 * normal, light_ray_dir);

                    hit light_hit;
                    if (aggregate.intersect(light_ray, light_hit))
                    {
                        if (light_hit.t >= light_distance - 0.01)
                        {
                            vec3 light_ray_dir_local = world_to_local(light_ray_dir, s, normal, t);
                            double light_brdf_pdf;
                            double G_over_cos_theta = std::abs(dot(-light_ray_dir, light_normal)) / (light_distance * light_distance);
                            double G = G_over_cos_theta * std::abs(light_ray_dir_local.y);
                            vec3 light_brdf = material->eval_brdf(wo_local, light_ray_dir_local, light_brdf_pdf);
                            double mis_weight = light_pdf / (light_pdf + light_brdf_pdf * G_over_cos_theta);
                            col += mis_weight * transmittance(light_distance) * throughput * light_brdf * G * light_le / light_pdf;
                        }
                    }
                }

                vec3 brdf;
                vec3 wi_local;
                brdf = material->sample(wo_local, wi_local, last_pt_pdf);
                double cos = wi_local.y;
                vec3 wi = local_to_world(wi_local, s, normal, t);

                throughput *= brdf * cos / last_pt_pdf;

                ra = ray(ray_hit.position + 0.001 * normal, wi);
            }
        }
        else
        {
            col += throughput * vec3(0);
            break;
        }
    }

    return col;
}

int main()
{
    // サンプリング数
    const int N = 1000;

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
    img.ppm_output("volume_mis.ppm");
    return 0;
};