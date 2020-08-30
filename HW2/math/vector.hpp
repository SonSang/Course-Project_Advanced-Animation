#ifndef __CMATH_VECTOR_H__
#define __CMATH_VECTOR_H__

#ifdef _MSC_VER
#pragma once
#endif

#include <array>

template <typename T, int N>
class vector
{
private:
    std::array<T, N> elements;

public:
    vector() = default;
    vector(const std::array<T, N> &e) : elements(e) {}

    T get(int i) const { return elements.at(i); }
    void set(int i, const T &e) { elements.at(i) = e; }
    void set(const std::array<T, N> &e) { elements = e; }
};

class vec2d : public vector<double, 2>
{
public:
    vec2d() = default;
    vec2d(double x, double y);

    void set(double x, double y);

    double operator[](int i) const;
};

class vec3d : public vector<double, 3>
{
public:
    vec3d() = default;
    vec3d(double x, double y, double z);

    void set(double x, double y, double z);

    vec3d operator+(const vec3d &v) const noexcept;
    vec3d operator-(const vec3d &v) const noexcept;
    vec3d operator*(double c) const noexcept;
    vec3d operator/(double c) const;
    void operator+=(const vec3d &v) noexcept;
    void operator-=(const vec3d &v) noexcept;
    void operator*=(double c) noexcept;
    void operator/=(double c) noexcept;
    double operator[](int i) const;
    bool operator==(const vec3d &v) const noexcept;
    bool operator!=(const vec3d &v) const noexcept;
    friend std::ostream &operator<<(std::ostream &os, const vec3d &v);

    double length() const noexcept;
    void normalize();
    double dot(const vec3d &v) const noexcept;
    vec3d cross(const vec3d &v) const noexcept;
    double cross(const vec3d &v, const vec3d &v2) const noexcept; // triple vector product
    bool is_zero() const noexcept;
    // Return factor of this vector regarding to given vector [v].
    // @ is_unit_length : If [v] is unit vector, there is no need to divide by its length.
    double factorize(const vec3d &v, bool is_unit_length = false) const noexcept;

    void to_float_array(float arr[3]) const noexcept;
};

class vec4d : public vector<double, 4>
{
public:
    vec4d() = default;
    vec4d(double x, double y, double z, double w);

    vec4d operator+(const vec4d &v) const noexcept;
    vec4d operator-(const vec4d &v) const noexcept;
    vec4d operator*(double c) const noexcept;
    vec4d operator/(double c) const;
    void operator+=(const vec4d &v) noexcept;
    void operator-=(const vec4d &v) noexcept;
    void operator*=(double c) noexcept;
    void operator/=(double c) noexcept;
    double operator[](int i) const;
    bool operator==(const vec4d &v) const noexcept;
    friend std::ostream &operator<<(std::ostream &os, const vec4d &v);

    double length() const noexcept;
    bool is_zero() const noexcept;
};

#endif