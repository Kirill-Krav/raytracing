#pragma once

#include <cmath>

class Vec
{
public:
    Vec()
    {
        for (int i = 0; i < 4; i++)
        {
            data[i] = 0.0;
        }
    }
    Vec(float x, float y, float z)
    {
        data[0] = x;
        data[1] = y;
        data[2] = z;
        data[3] = 1.0;
    }
    Vec(float x, float y, float z, float w)
    {
        data[0] = x;
        data[1] = y;
        data[2] = z;
        data[3] = w;
    }
    float &operator[](int i)
    {
        return data[i];
    }
    float x() const { return data[0]; }
    float r() const { return data[0]; }
    float y() const { return data[1]; }
    float g() const { return data[1]; }
    float z() const { return data[2]; }
    float b() const { return data[2]; }
    float w() const { return data[3]; }
    float a() const { return data[3]; }

    float distance() const
    {
        return sqrtf(x() * x() + y() * y() + z() * z());
    }

    Vec &normalize()
    {
        *this = *this / distance();
        return *this;
    }

    float operator*(const Vec &other) const
    {
        return x() * other.x() + y() * other.y() + z() * other.z();
    }

    Vec operator/(float val) const
    {
        return Vec(x() / val, y() / val, z() / val);
    }

    Vec operator-(const Vec &other) const
    {
        return Vec(x() - other.x(), y() - other.y(), z() - other.z());
    }

private:
    float data[4];
};
