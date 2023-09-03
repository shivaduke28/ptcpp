#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>
#include <fstream>
#include "vec3.h"
#include "util.h"
namespace ptcpp
{
    class image
    {
    public:
        int width;
        int height;
        vec3 *data;

        image(int _width, int _height) : width(_width), height(_height)
        {
            data = new vec3[width * height];
        };

        ~image()
        {
            delete[] data;
        };

        vec3 get_pixel(int i, int j) const
        {
            return data[i + width * j];
        }

        void set_pixel(int i, int j, const vec3 &c)
        {
            data[i + width * j] = c;
        };

        void add_pixel(int i, int j, const vec3 &c)
        {
            vec3 c2 = data[i + width * j];
            data[i + width * j] = c + c2;
        };

        void divide(double k)
        {
            for (int i = 0; i < width; i++)
            {
                for (int j = 0; j < height; j++)
                {
                    this->set_pixel(i, j, this->get_pixel(i, j) / k);
                }
            }
        };

        void gamma_correction()
        {
            for (int i = 0; i < width; i++)
            {
                for (int j = 0; j < height; j++)
                {
                    vec3 c = this->get_pixel(i, j);
                    this->set_pixel(i, j, vec3(std::pow(c.x, 0.45454545454), std::pow(c.y, 0.45454545454), std::pow(c.z, 0.45454545454)));
                }
            }
        };

        void ppm_output(const std::string &fileName) const
        {
            std::ofstream file(fileName);
            file << "P3" << std::endl;
            file << width << " " << height << std::endl;
            file << 255 << std::endl;
            for (int j = 0; j < height; j++)
            {
                for (int i = 0; i < width; i++)
                {
                    vec3 c = this->get_pixel(i, j);
                    int r = clamp(int(255 * c.x), 0, 255);
                    int g = clamp(int(255 * c.y), 0, 255);
                    int b = clamp(int(255 * c.z), 0, 255);
                    file << r << " " << g << " " << b << std::endl;
                }
            }

            file.close();
        };
    };
}
#endif