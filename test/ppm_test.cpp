#include <iostream>
#include <fstream>
#include "gtest/gtest.h"
namespace ptcpp::test
{
    TEST(ppm, CreatePPM)
    {
        std::ofstream file("ppm_test.ppm");

        file << "P3" << std::endl;
        file << 512 << " " << 512 << std::endl;
        file << 255 << std::endl;
        for (int i = 0; i < 512; i++)
        {
            for (int j = 0; j < 512; j++)
            {
                int r = i / 512.0 * 255;
                int g = j / 512.0 * 255;
                int b = 255;
                file << r << " " << g << " " << b << std::endl;
            }
        }

        file.close();
    }
}