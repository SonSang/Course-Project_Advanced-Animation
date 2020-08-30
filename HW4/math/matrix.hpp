#ifndef __CMATH_MATRIX_H__
#define __CMATH_MATRIX_H__

#ifdef _MSC_VER
#pragma once
#endif

#include "vector.hpp"

// Simple [M] X [N] matrix that contains [T]-type values in row-major order.
template <class T, int M, int N>
class matrix
{
private:
    std::array<T, M * N> m;

public:
    matrix() = default;

    template <class... T2>
    matrix(T2... elements) : m({(T)elements...})
    {
        static_assert(sizeof...(T2) == M * N, "Invalid matrix initializer by size mismatch.");
    }
    void set(int index, T value)
    {
        m.at(index) = value;
    }
    void set(int row, int col, T value)
    {
        m.at(row * N + col) = value;
    }
    T get(int index) const
    {
        return m.at(index);
    }
    T get(int row, int col) const
    {
        return m.at(row * N + col);
    }
    void clear()
    {
        fill(0);
    }
    void fill(T val)
    {
        m.fill(val);
    }
};

template <int N, int M>
class matd : public matrix<double, N, M>
{
};

class mat4d : public matd<4, 4>
{
public:
    mat4d() = default;

    void identity();
    mat4d operator*(const mat4d &m) const;
    vec4d operator*(const vec4d &v) const;
    friend std::ostream &operator<<(std::ostream &os, const mat4d &m);
};

#endif