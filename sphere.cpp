#include "sphere.hpp"

Sphere::Sphere(const Vec &c, const float &r, const Material &m) : center(c), radius(r), material(m){}

bool Sphere::ray_intersect(const Vec &orig, const Vec &dir, float &t0) const{
    Vec L = center - orig;
    float tca = Vec::dot(L, dir);
    float d2 = Vec::dot(L, L) - tca * tca;
    //std::cout << radius * radius << std::endl;
    if(d2 > radius * radius){
        return false;
    }
    float thc = sqrtf(radius * radius - d2);
    t0 = tca - thc;
    float t1 = tca + thc;
    if(t0 < 0){
        t0 = t1;
    }
    if(t0 < 0){
        return false;
    }
    return true;
}