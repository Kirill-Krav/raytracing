#pragma once

#include "vec.hpp"

class Light
{
public:
    Light(const Vec &p, const Vec &ac, const Vec &dc, const Vec &sc) : position(p), ambientColor(ac), diffuseColor(dc), specularColor(sc){};
    Vec getPosition() const { return position; }
    Vec getAmbientColor() const { return ambientColor; }
    Vec getDiffuseColor() const { return diffuseColor; }
    Vec getSpecularColor() const { return specularColor; }

private:
    Vec position;
    Vec ambientColor;
    Vec diffuseColor;
    Vec specularColor;
};