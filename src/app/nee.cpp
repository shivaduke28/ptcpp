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

using namespace ptcpp;

const int MAX_DEPTH = 500;
const double ROULETTE = 0.9;

class scene_lights
{
    std::vector<std::shared_ptr<shape>> shapes;
    std::vector<double> areas;

public:
    double area;
    scene_lights(){};

    void add(const std::shared_ptr<shape> &s)
    {
        shapes.push_back(s);
        double a = (*s).area;
        areas.push_back(a);
        area += a;
    };

    vec3 sample(double &pdf) const
    {
        double r = rnd() * area;
        double sum(0);
        int length = shapes.size();
        for (int i = 0; i < length; ++i)
        {
            double w = areas[i];
            sum += w;
            if (r < sum || i == length - 1)
            {
                pdf = w / area;
                auto shape = shapes[i];
                double pdf2;
                vec3 pos = (*shape).sample(pdf2);
                pdf *= pdf2;
                return pos;
            }
        }
    }
};

vec3 radiance(const ray &init_ray, const aggregate &aggregate, const scene_lights &scene_lights)
{
    vec3 col;
    vec3 throughput(1);
    ray ra = init_ray;

    for (int depth = 0; depth < MAX_DEPTH; depth++)
    {
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
            if (depth > 0 && light->enable)
                break;

            // depth == 0のときしか意味がないはず
            col += throughput * light->Le();

            double light_pdf;
            vec3 light_pos = scene_lights.sample(light_pdf);

            double light_distance2 = (light_pos - position).length2();
            vec3 light_ray_dir = normalize(light_pos - position);
            ray light_ray(position + 0.001 * normal, light_ray_dir);
            hit light_hit;
            if (aggregate.intersect(light_ray, light_hit))
            {
                // visible
                if (light_hit.t * light_hit.t >= light_distance2 - 0.001)
                {
                    double G = std::abs(dot(light_ray.direction, normal)) *
                               std::abs(dot(-light_ray.direction, light_hit.normal)) / light_distance2;
                    vec3 light_ray_dir_local = world_to_local(light_ray_dir, s, normal, t);
                    double x;
                    vec3 light_brdf = material->sample(light_ray_dir_local, x);
                    auto l = light_hit.light;
                    col += throughput * light_brdf * G / light_pdf * l->Le(); // * scene_lights.area / aggregate.area;
                }
            }

            vec3 brdf;
            double pdf;
            vec3 wi_local;
            brdf = material->sample(wo_local, wi_local, pdf);
            double cos = wi_local.y;
            vec3 wi = local_to_world(wi_local, s, normal, t);

            throughput *= brdf * cos / pdf;

            ra = ray(position + 0.001 * normal, wi);
        }
        else
        {
            col += throughput * vec3(0.00);
            break;
        }

        if (rnd() >= ROULETTE)
            break;
        throughput /= ROULETTE;
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
    aggregate.add(std::make_shared<cube>(vec3(0, -0.5, 0), vec3(5, 1, 5), white, light_off));
    // ceil
    aggregate.add(std::make_shared<cube>(vec3(0, 4.5, 0), vec3(5, 1, 5), white, light_off));
    // wall
    aggregate.add(std::make_shared<cube>(vec3(0, 2, -2.5), vec3(5, 5, 1), white, light_off));
    // right green
    aggregate.add(std::make_shared<cube>(vec3(2.5, 2.5, 0), vec3(1, 5, 5), green, light_off));
    // left red
    aggregate.add(std::make_shared<cube>(vec3(-2.5, 2.5, 0), vec3(1, 5, 5), red, light_off));
    // light
    aggregate.add(std::make_shared<cube>(vec3(0, 4, 0), vec3(1, 0.2, 1), white, light));

    aggregate.add(std::make_shared<cube>(vec3(0.5, 1, -1), vec3(0.8, 2, 0.8), white, light_off));
    aggregate.add(std::make_shared<sphere>(vec3(-0.5, 0.5, 0), 0.5, white, light_off));

    // todo: aggregateと統一する
    scene_lights scene_lights;
    scene_lights.add(std::make_shared<cube>(vec3(0, 4, 0), vec3(1, 0.2, 1), white, light));

    std::cout << scene_lights.area << " / " << aggregate.area << std::endl;

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

                vec3 col = radiance(ray, aggregate, scene_lights);
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