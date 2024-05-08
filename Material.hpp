#pragma once

#include <string>
#include "vec.hpp"

class Material
{
public:
    Material(const Vec &ac, const Vec &dc, const Vec &sc, const float s, const float r) : ambientColor(ac), diffuseColor(dc), specularColor(sc), shininess(s), reflection(r) {}
    Material() : ambientColor(), diffuseColor(), specularColor(), shininess(), reflection() {}
    Vec getAmbientColor() { return ambientColor; }
    Vec getDiffuseColor() { return diffuseColor; }
    Vec getSpecularColor() { return specularColor; }
    float getShininess() { return shininess; }
    float getReflection() { return reflection; }

private:
    Vec ambientColor;
    Vec diffuseColor;
    Vec specularColor;
    float shininess;
    float reflection;
};