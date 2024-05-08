#include <cmath>

#include "vec.hpp"
#include "Material.hpp"

class Sphere
{
public:
    Sphere(const Vec &c, const float &r, const Material &m);
    bool ray_intersect(const Vec &orig, const Vec &dir, float &t0) const;
    Material getMaterial() const { return material; }

private:
    Vec center;
    float radius;
    Material material;
};