#include "image.h"
#include "camera.h"
#include "vec3.h"
#include "diffuse.h"
#include "light.h"
#include <memory>
#include "aggregate.h"
#include <omp.h>
#include <algorithm>

double sample_Henyey_Greenstein(double g)
{
    double sqrTerm = (1.0 - g * g) / (1.0 + g - 2.0 * g * rnd());
    double cos_theta = -(1 + g * g - sqrTerm * sqrTerm) / (2.0 * g);
    return cos_theta;
}

double HenyeyGreenstein(double theta, double g)
{
    double a = (1.0 + g * g + 2.0 * g * std::cos(theta));
    return (1.0 - g * g) / (4.0 * M_PI * a * std::sqrt(a));
}

int main()
{
    const int N = 10000;
    const double G = 0.8;
    Image img(512, 512);
    double result[img.width];

    for (int i = 0; i < N; i++)
    {
        double cos_theta = sample_Henyey_Greenstein(G);
        double theta = std::acos(cos_theta);
        double u = theta / (M_PI);
        int j = (int)(u * img.width);
        result[j]++;
    }

    for (int i = 0; i < img.width; i++)
    {
        double u = ((double)i) / (double)img.width * M_PI;
        double hg = HenyeyGreenstein(u, G) * std::sin(u);
        int count = result[i];
        for (int j = 0; j < img.height; j++)
        {
            if (j < hg * 300)
            {
                img.setPixel(i, j, Vec3(1, 0, 0));
            }
            if (j < count)
            {
                img.addPixel(i, j, Vec3(0, 1, 0));
            }
        }
    }

    img.ppmOutput("HG.ppm");
    return 1;
}
