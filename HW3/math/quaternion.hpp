#ifndef __CMATH_QUATERNION_H__
#define __CMATH_QUATERNION_H__

#ifdef _MSC_VER
#pragma once
#endif

#include "vector.hpp"

class quaternion : public vec4d
{
public:
    quaternion() = default;
    quaternion(double x, double y, double z, double w);

    quaternion operator*(const quaternion &q) const; // quaternion multiplication.
    quaternion operator*(double c) const;
    void operator*=(const quaternion &q);
    void operator*=(double c);

    double norm() const noexcept;
    double normsq() const noexcept;

    quaternion conjugate() const;

    quaternion inverse() const;

    vec3d log() const;

    // get quaternion that represents rotation. axis [ax, ay, az] goes through the origin.
    static quaternion rotation(double ax, double ay, double az, double radian);
};

#endif