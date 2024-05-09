#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <limits>
#include <cmath>

#include "vec.hpp"
#include "sphere.hpp"
#include "light.hpp"

bool scene_intersect(const Vec &orig, const Vec &dir, const std::vector<Sphere> &spheres, Material &material, Vec &point, Vec &N)
{
    float sphereDist = std::numeric_limits<float>::max();
    for (int i = 0; i < spheres.size(); i++)
    {
        float newSphereDist = std::numeric_limits<float>::max();
        if (spheres[i].ray_intersect(orig, dir, newSphereDist))
        {
            if (newSphereDist < sphereDist)
            {
                material = spheres[i].getMaterial();
                sphereDist = newSphereDist;
                point = orig + dir * sphereDist;
                N = (point - spheres[i].getCenter()).normalize();
            }
        }
    }
    return sphereDist < 1000;
}

Vec reflect(const Vec &I, const Vec &N)
{
    return N * 2.f * Vec::dot(I, N) - I;
}

Vec refract(const Vec &I, const Vec &N, const float &refractive_index) {
    float cosi = std::max(-1.f, std::min(1.f, Vec::dot(I,N * -1.0)));
    float etai = 1, etat = refractive_index;
    Vec n = N;
    if (cosi < 0) { 
        cosi = -cosi;
        std::swap(etai, etat); n = N * -1.0;
    }
    float eta = etai / etat;
    float k = 1 - eta*eta*(1 - cosi*cosi);
    return k < 0 ? Vec(0,0,0) : I*eta + n*(eta * cosi - sqrtf(k));
}

Vec cast_ray(const Vec &orig, const Vec &dir, const std::vector<Sphere> &spheres, const std::vector<Light> &lights, int depth = 0)
{
    Material material;
    Vec point, N;

    if (depth < 4 && scene_intersect(orig, dir, spheres, material, point, N))
    {
        Vec res(0.0f, 0.0f, 0.0f);
        Vec reflect_dir = reflect(dir * -1.0, N).normalize();
        Vec refract_dir = refract(dir, N, material.getRefractiveIndex()).normalize();
        Vec reflect_orig = Vec::dot(reflect_dir, N) < 0 ? point - N * 1e-3 : point + N * 1e-3;
        Vec refract_orig = Vec::dot(refract_dir, N) < 0 ? point - N*1e-3 : point + N*1e-3;
        Vec reflect_color = cast_ray(reflect_orig, reflect_dir, spheres, lights, depth + 1);
        Vec refract_color = cast_ray(refract_orig, refract_dir, spheres, lights, depth + 1);
        for (int i = 0; i < lights.size(); i++)
        {
            Vec lightDir = (lights[i].getPosition() - point).normalize();
            float lightDist = (lights[i].getPosition() - point).distance();
            Vec shadowOrig = Vec::dot(lightDir, N) < 0 ? point - N * 1e-3 : point + N * 1e-3;
            Vec shadowPoint, shadowN;
            Material tmpmaterial;
            if (scene_intersect(shadowOrig, lightDir, spheres, tmpmaterial, shadowPoint, shadowN))
                continue;
            Vec ambient_light_intensity = material.getAmbientColor() * lights[i].getAmbientColor();
            Vec diffuse_light_intensity = material.getDiffuseColor() * std::max(0.0f, Vec::dot(lightDir, N)) * lights[i].getDiffuseColor();
            Vec specular_light_intensity = material.getSpecularColor() * std::pow(std::max(0.0f, Vec::dot(reflect(lightDir * -1.0, N), dir)), material.getShininess()) * lights[i].getSpecularColor();
            res = res + (ambient_light_intensity + diffuse_light_intensity + specular_light_intensity);
        }

        return res + reflect_color * material.getReflection() + refract_color * material.getRefraction();
    }
    else
    {
        return Vec(0.2f, 0.7f, 0.8f);
    }
}

void render(const std::vector<Sphere> &spheres, const std::vector<Light> &lights)
{
    const int width = 1280;
    const int height = 720;
    const float fov = acos(-1) / 2.0f;
    std::vector<Vec> framebuffer(height * width);
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float x = (2 * (j + 0.5) / static_cast<float>(width) - 1) * tan(fov / 2.0f) * width / (float)height;
            float y = -(2 * (i + 0.5) / static_cast<float>(height) - 1) * tan(fov / 2.0f);
            Vec dir = Vec(x, y, -1.0).normalize();
            framebuffer[i * width + j] = cast_ray(Vec(0.0f, 0.0f, 0.0f), dir, spheres, lights);
        }
    }

    std::ofstream fout;
    fout.open("./out.ppm", std::ofstream::out | std::ofstream::binary);
    fout << "P6\n"
         << width << " " << height << "\n255\n";
    for (int i = 0; i < framebuffer.size(); i++)
    {
        for (int j = 0; j < 3; j++)
        {
            fout << static_cast<char>(255 * std::max(0.0f, std::min(1.0f, framebuffer[i][j])));
        }
    }
    fout.close();
}

int main()
{
    Material gold(Vec(0.24725f, 0.1995f, 0.0745f),
                  Vec(0.75164f, 0.60648f, 0.22648f),
                  Vec(0.628281f, 0.555802f, 0.366065f),
                  0.4f * 128, 0.1, 0.0, 1.0);
    Material ruby(Vec(0.1745f, 0.01175f, 0.01175f),
                  Vec(0.61424f, 0.04136f, 0.04136f),
                  Vec(0.727811f, 0.626959f, 0.626959f),
                  0.6f * 128, 0.1, 0.0, 1.0);
    Material mirror(Vec(0.1, 0.1, 0.1),
                    Vec(0.1, 0.1, 0.1),
                    Vec(10.0, 10.0, 10.0),
                    1428.0, 0.8, 0.0, 1.0);
    Material glass(Vec(0.0, 0.0, 0.0),
                    Vec(0.0, 0.0, 0.0),
                    Vec(0.5, 0.5, 0.5),
                    125.0, 0.1, 0.8, 1.5);
    std::vector<Sphere> spheres;
    // spheres.push_back(Sphere(Vec(-3.0f, 0.0f, -16.0f), 2.0f, gold));
    // spheres.push_back(Sphere(Vec(-1.0f, -1.5f, -12.0f), 2.0f, glass));
    // spheres.push_back(Sphere(Vec(1.5f, -0.5f, -18.0f), 3.0f, ruby));
    // spheres.push_back(Sphere(Vec(7.0f, 5.0f, -18.0f), 4.0f, mirror));
    spheres.push_back(Sphere(Vec(-10, 5, -18), 4, mirror));
    spheres.push_back(Sphere(Vec(0, 2, -40), 14, ruby));
    spheres.push_back(Sphere(Vec(16, 5, -41), 9, gold));
    spheres.push_back(Sphere(Vec(4, -3, -10), 4, glass));
    std::vector<Light> lights;
    lights.push_back(Light(Vec(-20.0f, 20.0f, 20.0f),
                           Vec(0.2f, 0.2f, 0.2f),
                           Vec(0.9f, 0.9f, 0.9f),
                           Vec(1.0f, 1.0f, 1.0f)));
    lights.push_back(Light(Vec(30.0f, 50.0f, -25.0f),
                           Vec(0.2f, 0.2f, 0.2f),
                           Vec(0.9f, 0.9f, 0.9f),
                           Vec(1.0f, 1.0f, 1.0f)));
    lights.push_back(Light(Vec(30.0f, 20.0f, 30.0f),
                           Vec(0.2f, 0.2f, 0.2f),
                           Vec(0.9f, 0.9f, 0.9f),
                           Vec(1.0f, 1.0f, 1.0f)));

    render(spheres, lights);
    return 0;
}