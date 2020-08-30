#ifndef __CMATH_TRANSFORMATION_H__
#define __CMATH_TRANSFORMATION_H__

#ifdef _MSC_VER
#pragma once
#endif

#include "matrix.hpp"
#include "quaternion.hpp"

class transformation
{
private:
    mat4d m; // original
    mat4d i; // inverse

    mat4d t; // translation only.
    mat4d ti;
    mat4d r; // rotation only.
    mat4d ri;

public:
    transformation();

    // Generate translation matrix.
    static transformation translation(double x, double y, double z);
    // Generate rotation matrix using quaternion.
    // @axisA : Start point of axis line.
    // @axisB : End point of axis line.
    // @radian : Radian angle to rotate.
    static transformation rotation(const vec3d &axisA, const vec3d &axisB, double radian);
    // return rotational transformation that transforms [begin] vector into [end] vector.
    static transformation irotation(const vec3d &begin, const vec3d &end);

    void translate(const vec3d &t);
    void translate(double x, double y, double z);

    void rotate(const vec3d &axisA, const vec3d &axisB, double radian);
    void irotate(const vec3d &begin, const vec3d &end);

    vec4d apply(const vec4d &vec) const;
    vec4d inverse(const vec4d &vec) const;
    vec3d apply(const vec3d &vec) const;
    vec3d inverse(const vec3d &vec) const;
    transformation apply(const transformation &tr) const;
    transformation inverse(const transformation &tr) const;

    // apply only translation.
    vec4d applyT(const vec4d &vec) const;
    vec4d inverseT(const vec4d &vec) const;
    vec3d applyT(const vec3d &vec) const;
    vec3d inverseT(const vec3d &vec) const;

    // apply only rotation.
    vec4d applyR(const vec4d &vec) const;
    vec4d inverseR(const vec4d &vec) const;
    vec3d applyR(const vec3d &vec) const;
    vec3d inverseR(const vec3d &vec) const;

    // get static array
    void get_translation(double t[3]) const;
    void get_rotation(double r[3][3]) const;
    void get_matrix_transpose(double m[16]) const;
    quaternion get_rotation_quaternion() const;
    vec3d get_rotation_vector() const;

    void set(double m[4][4]);
    void get(double m[4][4]);

    transformation operator*(const transformation &tr) const noexcept;
    void operator*=(const transformation &tr) noexcept;
    friend std::ostream &operator<<(std::ostream &os, const transformation &tf);
};

#endif