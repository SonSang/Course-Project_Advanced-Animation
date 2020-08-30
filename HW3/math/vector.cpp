#include "vector.hpp"
#include <iostream>
#include <math.h>

// vec2d
vec2d::vec2d(double x, double y) : vector({x, y}) {}
void vec2d::set(double x, double y)
{
    vector::set({x, y});
}
double vec2d::operator[](int i) const
{
    return get(i);
}

// vec3d
vec3d::vec3d(double x, double y, double z) : vector({x, y, z}) {}
void vec3d::set(double x, double y, double z)
{
    vector::set({x, y, z});
}
vec3d vec3d::operator+(const vec3d &v) const noexcept
{
    return vec3d(get(0) + v.get(0), get(1) + v.get(1), get(2) + v.get(2));
}
vec3d vec3d::operator-(const vec3d &v) const noexcept
{
    return vec3d(get(0) - v.get(0), get(1) - v.get(1), get(2) - v.get(2));
}
vec3d vec3d::operator*(double c) const noexcept
{
    return vec3d(get(0) * c, get(1) * c, get(2) * c);
}
vec3d vec3d::operator/(double c) const
{
    return vec3d(get(0) / c, get(1) / c, get(2) / c);
}

void vec3d::operator+=(const vec3d &v) noexcept
{
    set(get(0) + v.get(0), get(1) + v.get(1), get(2) + v.get(2));
}
void vec3d::operator-=(const vec3d &v) noexcept
{
    set(get(0) - v.get(0), get(1) - v.get(1), get(2) - v.get(2));
}
void vec3d::operator*=(double c) noexcept
{
    set(get(0) * c, get(1) * c, get(2) * c);
}
void vec3d::operator/=(double c) noexcept
{
    set(get(0) / c, get(1) / c, get(2) / c);
}

double vec3d::operator[](int i) const
{
    return get(i);
}
bool vec3d::operator!=(const vec3d &v) const noexcept
{
    return !(*this == v);
}
bool vec3d::operator==(const vec3d &v) const noexcept
{
    return (*this - v).is_zero();
}
std::ostream &operator<<(std::ostream &os, const vec3d &v)
{
    os << v.get(0) << ", " << v.get(1) << ", " << v.get(2);
    return os;
}

double vec3d::length() const noexcept
{
    return sqrt(get(0) * get(0) + get(1) * get(1) + get(2) * get(2));
}
void vec3d::normalize()
{
    if (is_zero())
        std::cerr << "Cannot normalize zero vector." << std::endl;
    double
        len = length();
    *this /= len;
}
double vec3d::dot(const vec3d &v) const noexcept
{
    return get(0) * v[0] + get(1) * v[1] + get(2) * v[2];
}
vec3d vec3d::cross(const vec3d &v) const noexcept
{
    return vec3d(get(1) * v[2] - get(2) * v[1], get(2) * v[0] - get(0) * v[2], get(0) * v[1] - get(1) * v[0]);
}
double vec3d::cross(const vec3d &v, const vec3d &v2) const noexcept
{
    return get(0) * (v[1] * v2[2] - v[2] * v2[1]) - get(1) * (v[0] * v2[2] - v[2] * v2[0]) + get(2) * (v[0] * v2[1] - v[1] * v2[0]);
}
bool vec3d::is_zero() const noexcept
{
    return (get(0) == 0 && get(1) == 0 && get(2) == 0);
}
double vec3d::factorize(const vec3d &v, bool is_unit_length) const noexcept
{
    if (is_unit_length)
        return dot(v);
    else
    {
        double
            denom = v.dot(v);
        if (denom == 0.0)
            std::cerr << "[v] should not be zero vector." << std::endl;
        return dot(v) / denom;
    }
}

void vec3d::to_float_array(float arr[3]) const noexcept
{
    arr[0] = (float)get(0);
    arr[1] = (float)get(1);
    arr[2] = (float)get(2);
}

// vec4d
vec4d::vec4d(double x, double y, double z, double w) : vector({x, y, z, w}) {}

vec4d vec4d::operator+(const vec4d &v) const noexcept
{
    return vec4d(get(0) + v.get(0), get(1) + v.get(1), get(2) + v.get(2), get(3) + v.get(3));
}
vec4d vec4d::operator-(const vec4d &v) const noexcept
{
    return vec4d(get(0) - v.get(0), get(1) - v.get(1), get(2) - v.get(2), get(3) - v.get(3));
}
vec4d vec4d::operator*(double c) const noexcept
{
    return vec4d(get(0) * c, get(1) * c, get(2) * c, get(3) * c);
}
vec4d vec4d::operator/(double c) const
{
    return vec4d(get(0) / c, get(1) / c, get(2) / c, get(3) / c);
}
void vec4d::operator+=(const vec4d &v) noexcept
{
    set({get(0) + v.get(0), get(1) + v.get(1), get(2) + v.get(2), get(3) + v.get(3)});
}
void vec4d::operator-=(const vec4d &v) noexcept
{
    set({get(0) - v.get(0), get(1) - v.get(1), get(2) - v.get(2), get(3) - v.get(3)});
}
void vec4d::operator*=(double c) noexcept
{
    set({get(0) * c, get(1) * c, get(2) * c, get(3) * c});
}
void vec4d::operator/=(double c) noexcept
{
    set({get(0) / c, get(1) / c, get(2) / c, get(3) / c});
}
double vec4d::operator[](int i) const
{
    return get(i);
}
bool vec4d::operator==(const vec4d &v) const noexcept
{
    return (*this - v).is_zero();
}
std::ostream &operator<<(std::ostream &os, const vec4d &v)
{
    os << v.get(0) << ", " << v.get(1) << ", " << v.get(2);
    return os;
}

double vec4d::length() const noexcept
{
    return sqrt(get(0) * get(0) + get(1) * get(1) + get(2) * get(2) + get(3) * get(3));
}
bool vec4d::is_zero() const noexcept
{
    return (get(0) == 0 && get(1) == 0 && get(2) == 0 && get(3) == 0);
}