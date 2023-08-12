#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>
#include <fstream>
#include "vec3.h"
#include "util.h"

class Image
{
public:
    int width;
    int height;
    Vec3 *data;

    Image(int _width, int _height) : width(_width), height(_height)
    {
        data = new Vec3[width * height];
    };

    ~Image()
    {
        delete[] data;
    };

    Vec3 getPixel(int i, int j) const
    {
        return data[i + width * j];
    }

    void setPixel(int i, int j, const Vec3 &c)
    {
        data[i + width * j] = c;
    };

    void addPixel(int i, int j, const Vec3 &c)
    {
        Vec3 c2 = data[i + width * j];
        data[i + width * j] = c + c2;
    };

    void divide(double k)
    {
        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < height; j++)
            {
                this->setPixel(i, j, this->getPixel(i, j) / k);
            }
        }
    };

    void gammaCorrection()
    {
        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < height; j++)
            {
                Vec3 c = this->getPixel(i, j);
                this->setPixel(i, j, Vec3(std::pow(c.x, 0.45454545454), std::pow(c.y, 0.45454545454), std::pow(c.z, 0.45454545454)));
            }
        }
    };

    void ppmOutput(const std::string &fileName) const
    {
        std::ofstream file(fileName);
        file << "P3" << std::endl;
        file << width << " " << height << std::endl;
        file << 255 << std::endl;
        for (int j = 0; j < height; j++)
        {
            for (int i = 0; i < width; i++)
            {
                Vec3 c = this->getPixel(i, j);
                int r = clamp(int(255 * c.x), 0, 255);
                int g = clamp(int(255 * c.y), 0, 255);
                int b = clamp(int(255 * c.z), 0, 255);
                file << r << " " << g << " " << b << std::endl;
            }
        }

        file.close();
    };
};

#endif