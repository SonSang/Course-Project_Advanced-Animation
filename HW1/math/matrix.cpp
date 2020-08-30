#include "matrix.hpp"
#include <iostream>

void mat4d::identity()
{
    clear();
    set(0, 0, 1);
    set(1, 1, 1);
    set(2, 2, 1);
    set(3, 3, 1);
}
mat4d mat4d::operator*(const mat4d &m) const
{
    mat4d
        ret;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            double
                value = 0;
            for (int k = 0; k < 4; k++)
                value += get(i, k) * m.get(k, j);
            ret.set(i, j, value);
        }
    }
    return ret;
}
vec4d mat4d::operator*(const vec4d &v) const
{
    vec4d
        ret;
    for (int i = 0; i < 4; i++)
    {
        double
            val = 0;
        for (int j = 0; j < 4; j++)
            val += get(i, j) * v[j];
        ret.set(i, val);
    }
    return vec4d(ret[0], ret[1], ret[2], ret[3]);
}
std::ostream &operator<<(std::ostream &os, const mat4d &m)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
            os << m.get(i, j) << ", ";
        if (i < 3)
            os << std::endl;
    }
    return os;
}
