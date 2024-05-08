#pragma once

#include "vec.hpp"

class Light
{
public:
    Light(const Vec &p, const float &i) : position(p), intensity(i){};
    Vec getPosition() { return position; }
    float getIntensity() { return intensity; }
private:
    Vec position;
    float intensity;
};