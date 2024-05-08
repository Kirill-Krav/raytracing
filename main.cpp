#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <limits>
#include <cmath>

#include "vec.hpp"
#include "sphere.hpp"

Vec cast_ray(const Vec &orig, const Vec &dir, const Sphere &sphere)
{
    float sphereDist = std::numeric_limits<float>::max();
    if (sphere.ray_intersect(orig, dir, sphereDist))
    {
        return Vec(0.4f, 0.4f, 0.3f);
    }
    else
    {
        return Vec(0.2f, 0.7f, 0.8f);
    }
}

void render(const Sphere &sphere)
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
            framebuffer[i * width + j] = cast_ray(Vec(0.0f, 0.0f, 0.0f), dir, sphere);
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
    render(Sphere(Vec(-10, 5, -18), 4));
    return 0;
}