#pragma once

#include <string>
#include "vec.hpp"

class Material{
public:
    Material(const Vec &color) : diffuseColor(color) {}
    Material() : diffuseColor() {}
    Vec getColor(){ return diffuseColor; }
private:
    Vec diffuseColor;
};