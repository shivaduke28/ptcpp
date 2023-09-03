#include "image.h"
#include "gtest/gtest.h"
namespace ptcpp::test
{
    TEST(image, CreateImage)
    {
        image img(512, 512);
        for (int i = 0; i < img.width; i++)
        {
            for (int j = 0; j < img.height; j++)
            {
                img.set_pixel(i, j, vec3(i / double(img.width), j / double(img.height), 1.0));
            }
        }

        img.ppm_output("image_test.ppm");
    };
}