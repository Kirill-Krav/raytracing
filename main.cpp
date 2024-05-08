#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <limits>
#include <cmath>

#include "vec.hpp"
#include "sphere.hpp"
#include "light.hpp"

bool scene_intersect(const Vec &orig, const Vec &dir, const std::vector<Sphere> &spheres, Material &material, Vec &point, Vec &N){
    float sphereDist = std::numeric_limits<float>::max();
    for(int i = 0; i < spheres.size(); i++){
        float newSphereDist = std::numeric_limits<float>::max();
        if(spheres[i].ray_intersect(orig, dir, newSphereDist)){
            if(newSphereDist < sphereDist){
                material = spheres[i].getMaterial();
                sphereDist = newSphereDist;
                point = orig + dir * sphereDist;
                N = (point - spheres[i].getCenter()).normalize();
            }
        }
    }
    return sphereDist < 1000;
}

Vec cast_ray(const Vec &orig, const Vec &dir, const std::vector<Sphere> &spheres)
{
    Material material;
    Vec point, N;

    if (scene_intersect(orig, dir, spheres, material, point, N))
    {
        Light light(Vec(-20.0f, 20.0f, 20.0f), 1.5f);
        Vec lightDir = (light.getPosition() - point).normalize();
        float diffuse_light_intensity = light.getIntensity() * std::max(0.0f, lightDir * N);
        return material.getColor() * diffuse_light_intensity;
    }
    else
    {
        return Vec(0.2f, 0.7f, 0.8f);
    }
}

void render(const std::vector<Sphere> &spheres)
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
            framebuffer[i * width + j] = cast_ray(Vec(0.0f, 0.0f, 0.0f), dir, spheres);
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
    Material ivory(Vec(0.4f, 0.4f, 0.3f));
    Material red_rubber(Vec(0.3, 0.1, 0.1));
    std::vector<Sphere> spheres;
    Sphere sphere1(Vec(-10, 5, -18), 4, ivory);
    Sphere sphere2(Vec(0, 2, -40), 14, red_rubber);
    Sphere sphere3(Vec(10, 5, -41), 5, ivory);
    Sphere sphere4(Vec(4, -5, -10), 4, red_rubber);
    spheres.push_back(sphere1);
    spheres.push_back(sphere2);
    spheres.push_back(sphere3);
    spheres.push_back(sphere4);
    render(spheres);
    return 0;
}