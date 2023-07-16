#include "image.hpp"

int main()
{
    Image img(512, 512);
    for (int i = 0; i < img.width; i++)
    {
        for (int j = 0; j < img.height; j++)
        {
            img.setPixel(i, j, Vec3(i / double(img.width), j / double(img.height), 1.0));
        }
    }

    img.ppmOutput("out/image_test.ppm");
    return 0;
};