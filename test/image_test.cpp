#include "image.h"
#include "gtest/gtest.h"

TEST(Image, CreateImage)
{
    Image img(512, 512);
    for (int i = 0; i < img.width; i++)
    {
        for (int j = 0; j < img.height; j++)
        {
            img.setPixel(i, j, Vec3(i / double(img.width), j / double(img.height), 1.0));
        }
    }

    img.ppmOutput("image_test.ppm");
};