#include "vec.hpp"
#include <cmath>

class Sphere
{
public:
    Sphere(const Vec &c, const float &r);
    bool ray_intersect(const Vec &orig, const Vec &dir, float &t0) const;

private:
    Vec center;
    float radius;
};