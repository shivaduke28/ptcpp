#ifndef HIT_H
#define HIT_H

#include "vec3.h"
namespace ptcpp
{
    // prototype
    class sphere;

    class hit
    {
    public:
        double t;
        vec3 hitPos;
        vec3 hitNormal;
        const sphere *hitSphere;

        hit()
        {
            t = 100000;
        };
    };
}
#endif