#pragma once

#include <cmath>

class Vec
{
public:
    Vec()
    {
        data[0] = 0.0f;
        data[1] = 0.0f;
        data[2] = 0.0f;
    }
    Vec(float x, float y, float z)
    {
        data[0] = x;
        data[1] = y;
        data[2] = z;
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
    float data[3];
};
