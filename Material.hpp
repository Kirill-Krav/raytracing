#pragma once

#include <string>
#include "vec.hpp"

class Material
{
public:
    Material(const Vec &ac, const Vec &dc, const Vec &sc, const float s) : ambientColor(ac), diffuseColor(dc), specularColor(sc), shininess(s) {}
    Material() : ambientColor(), diffuseColor(), specularColor(), shininess() {}
    Vec getAmbientColor() { return ambientColor; }
    Vec getDiffuseColor() { return diffuseColor; }
    Vec getSpecularColor() { return specularColor; }
    float getShininess() { return shininess; }

private:
    Vec ambientColor;
    Vec diffuseColor;
    Vec specularColor;
    float shininess;
};