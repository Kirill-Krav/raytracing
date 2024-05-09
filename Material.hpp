#pragma once

#include <string>
#include "vec.hpp"

class Material
{
public:
    Material(const Vec &ac, const Vec &dc, const Vec &sc, const float s, const float refl, const float refr, const float ri) : ambientColor(ac), diffuseColor(dc), specularColor(sc), shininess(s), reflection(refl), refraction(refr), refractiveIndex(ri) {}
    Material() : ambientColor(), diffuseColor(), specularColor(), shininess(), reflection(), refraction(), refractiveIndex() {}
    Vec getAmbientColor() { return ambientColor; }
    Vec getDiffuseColor() { return diffuseColor; }
    Vec getSpecularColor() { return specularColor; }
    float getShininess() { return shininess; }
    float getReflection() { return reflection; }
    float getRefraction() { return refraction; }
    float getRefractiveIndex() { return refractiveIndex; }

private:
    Vec ambientColor;
    Vec diffuseColor;
    Vec specularColor;
    float shininess;
    float reflection;
    float refraction;
    float refractiveIndex;
};