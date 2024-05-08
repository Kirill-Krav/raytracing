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

Vec cast_ray(const Vec &orig, const Vec &dir, const std::vector<Sphere> &spheres, const std::vector<Light> &lights)
{
    Material material;
    Vec point, N;

    if (scene_intersect(orig, dir, spheres, material, point, N))
    {
        Vec res(0.0f, 0.0f, 0.0f);
        for(int i = 0; i < lights.size(); i++){
            Vec lightDir = (lights[i].getPosition() - point).normalize();
            Vec ambient_light_intensity = material.getAmbientColor() * lights[i].getAmbientColor();
            Vec diffuse_light_intensity = material.getDiffuseColor() * std::max(0.0f, Vec::dot(lightDir, N)) * lights[i].getDiffuseColor();
            Vec specular_light_intensity = material.getSpecularColor() * std::pow(std::max(0.0f, Vec::dot(lightDir, N)), material.getShininess()) * lights[i].getSpecularColor();
            res = res + (ambient_light_intensity + diffuse_light_intensity + specular_light_intensity);
        }
        
        return res;
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
                  0.4f * 128);
    Material ruby(Vec(0.1745f, 0.01175f, 0.01175f),
                  Vec(0.61424f, 0.04136f, 0.04136f),
                  Vec(0.727811f, 0.626959f, 0.626959f),
                  0.6f * 128);
    std::vector<Sphere> spheres;
    // spheres.push_back(Sphere(Vec(-3.0f, 0.0f, -16.0f), 2.0f, gold));
    // spheres.push_back(Sphere(Vec(-1.0f, -1.5f, -12.0f), 2.0f, ruby));
    // spheres.push_back(Sphere(Vec(1.5f, -0.5f, -18.0f), 3.0f, ruby));
    // spheres.push_back(Sphere(Vec(7.0f, 5.0f, -18.0f), 4.0f, gold));
    spheres.push_back(Sphere(Vec(-10, 5, -18), 4, gold));
    spheres.push_back(Sphere(Vec(0, 2, -40), 14, ruby));
    spheres.push_back(Sphere(Vec(16, 5, -41), 9, gold));
    spheres.push_back(Sphere(Vec(4, -5, -10), 4, ruby));
    std::vector<Light> lights;
    lights.push_back(Light( Vec(-20.0f, 20.0f, 20.0f),
                            Vec(0.2f, 0.2f, 0.2f),
                            Vec(0.9f, 0.9f, 0.9f),
                            Vec(1.0f, 1.0f, 1.0f)));
    lights.push_back(Light( Vec(30.0f, 50.0f, -25.0f),
                            Vec(0.2f, 0.2f, 0.2f),
                            Vec(0.9f, 0.9f, 0.9f),
                            Vec(1.0f, 1.0f, 1.0f)));
    lights.push_back(Light( Vec(30.0f, 20.0f, 30.0f),
                            Vec(0.2f, 0.2f, 0.2f),
                            Vec(0.9f, 0.9f, 0.9f),
                            Vec(1.0f, 1.0f, 1.0f)));
    
    render(spheres, lights);
    return 0;
}