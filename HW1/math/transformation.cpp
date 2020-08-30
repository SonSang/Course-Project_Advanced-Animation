#include "transformation.hpp"
#include "quaternion.hpp"
#include "converter.hpp"

#include <math.h>
#include <iostream>

transformation::transformation()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            double
                val = (i == j ? 1 : 0);
            this->m.set(i, j, val);
            this->i.set(i, j, val);
            this->t.set(i, j, val);
            this->ti.set(i, j, val);
            this->r.set(i, j, val);
            this->ri.set(i, j, val);
        }
    }
}
transformation transformation::translation(double x, double y, double z)
{
    transformation
        ret;
    ret.m.set(0, 3, x);
    ret.m.set(1, 3, y);
    ret.m.set(2, 3, z);
    ret.i.set(0, 3, -x);
    ret.i.set(1, 3, -y);
    ret.i.set(2, 3, -z);

    // Set translation only, too.
    ret.t = ret.m;
    ret.ti = ret.i;
    return ret;
}
transformation transformation::rotation(const vec3d &axisA, const vec3d &axisB, double radian)
{
    transformation
        ret;
    vec3d
        axis = axisB - axisA;
    mat4d
        rot,
        irot;
    convert(quaternion::rotation(axis[0], axis[1], axis[2], radian), rot);
    convert(quaternion::rotation(axis[0], axis[1], axis[2], -radian), irot);

    ret.translate(axisA * -1.0);
    ret.m = rot * ret.m;
    ret.i = ret.i * irot;
    ret.translate(axisA);

    // Set rotation only, too.
    ret.r = rot;
    ret.ri = irot; // @BUGFIX : Only rotational part has to be included in [r], [ri].
    return ret;
}
transformation transformation::irotation(const vec3d &begin, const vec3d &end)
{
    transformation
        ret = transformation();
    vec3d
        nbegin = begin,
        nend = end;
    nbegin.normalize();
    nend.normalize();
    if (nbegin == nend)
        return ret;
    vec3d
        axishelper = nbegin.cross(nend);
    axishelper.normalize();
    double
        angle = acos(nbegin.dot(nend));
    ret.rotate(vec3d(0, 0, 0), axishelper, angle);
    return ret;
}
void transformation::translate(const vec3d &t)
{
    translate(t[0], t[1], t[2]);
}
void transformation::translate(double x, double y, double z)
{
    transformation
        apply = translation(x, y, z);
    m = apply.m * m;
    i = i * apply.i; // Inverse matrix must be multiplied in reverse order.

    t = apply.t * t;
    ti = ti * apply.ti;
}
void transformation::rotate(const vec3d &axisA, const vec3d &axisB, double radian)
{
    transformation
        apply = rotation(axisA, axisB, radian);
    m = apply.m * m;
    i = i * apply.i; // Inverse matrix must be multiplied in reverse order.

    r = apply.r * r;
    ri = ri * apply.ri;
}
void transformation::irotate(const vec3d &begin, const vec3d &end)
{
    transformation
        apply = irotation(begin, end);
    m = apply.m * m;
    i = i * apply.i; // Inverse matrix must be multiplied in reverse order.

    r = apply.r * r;
    ri = ri * apply.ri;
}

vec4d transformation::apply(const vec4d &vec) const
{
    return m * vec;
}
vec4d transformation::inverse(const vec4d &vec) const
{
    return i * vec;
}
vec3d transformation::apply(const vec3d &vec) const
{
    vec4d
        tmp(vec[0], vec[1], vec[2], 1),
        ret = apply(tmp);
    return vec3d(ret[0], ret[1], ret[2]); // just ignore [w].
}
vec3d transformation::inverse(const vec3d &vec) const
{
    vec4d
        tmp(vec[0], vec[1], vec[2], 1),
        ret = inverse(tmp);
    return vec3d(ret[0], ret[1], ret[2]);
}

vec4d transformation::applyT(const vec4d &vec) const
{
    return t * vec;
}
vec4d transformation::inverseT(const vec4d &vec) const
{
    return ti * vec;
}
vec3d transformation::applyT(const vec3d &vec) const
{
    vec4d
        tmp(vec[0], vec[1], vec[2], 1),
        ret = applyT(tmp);
    return vec3d(ret[0], ret[1], ret[2]); // Ignore [w].
}
vec3d transformation::inverseT(const vec3d &vec) const
{
    vec4d
        tmp(vec[0], vec[1], vec[2], 1),
        ret = inverseT(tmp);
    return vec3d(ret[0], ret[1], ret[2]);
}

vec4d transformation::applyR(const vec4d &vec) const
{
    return r * vec;
}
vec4d transformation::inverseR(const vec4d &vec) const
{
    return ri * vec;
}
vec3d transformation::applyR(const vec3d &vec) const
{
    //vec4d
    //	tmp(vec[0], vec[1], vec[2], 1),
    //	ret = applyR(tmp);
    //return vec3d(ret[0], ret[1], ret[2]);	// ignore [w].
    return vec3d(r.get(0, 0) * vec[0] + r.get(0, 1) * vec[1] + r.get(0, 2) * vec[2],
                 r.get(1, 0) * vec[0] + r.get(1, 1) * vec[1] + r.get(1, 2) * vec[2],
                 r.get(2, 0) * vec[0] + r.get(2, 1) * vec[1] + r.get(2, 2) * vec[2]);
}
vec3d transformation::inverseR(const vec3d &vec) const
{
    return vec3d(ri.get(0, 0) * vec[0] + ri.get(0, 1) * vec[1] + ri.get(0, 2) * vec[2],
                 ri.get(1, 0) * vec[0] + ri.get(1, 1) * vec[1] + ri.get(1, 2) * vec[2],
                 ri.get(2, 0) * vec[0] + ri.get(2, 1) * vec[1] + ri.get(2, 2) * vec[2]);
}
transformation transformation::apply(const transformation &tr) const
{
    transformation
        ret;
    ret.m = this->m * tr.m;
    ret.i = tr.m * this->i;

    ret.t = this->t * tr.t;
    ret.ti = tr.ti * this->ti;

    ret.r = this->r * tr.r;
    ret.ri = tr.ri * this->ri;

    return ret;
}
transformation transformation::inverse(const transformation &tr) const
{
    transformation
        ret;
    ret.m = this->i * tr.m;
    ret.i = tr.i * this->m;

    ret.t = this->ti * tr.t;
    ret.ti = tr.ti * this->t;

    ret.r = this->ri * tr.r;
    ret.ri = tr.ri * this->r;

    return ret;
}
// Get
void transformation::get_translation(double t[3]) const
{
    for (int i = 0; i < 3; i++)
        t[i] = this->t.get(4 * i + 3);
}
void transformation::get_rotation(double r[3][3]) const
{
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            r[i][j] = this->r.get(4 * i + j);
}
void transformation::get_matrix_transpose(double m[16]) const
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            m[4 * i + j] = this->m.get(j, i);
        }
    }
}

transformation transformation::operator*(const transformation &tr) const noexcept
{
    transformation
        ret = *this;
    ret *= tr;
    return ret;
}
void transformation::operator*=(const transformation &tr) noexcept
{
    m = tr.m * m;
    i = i * tr.i;

    t = tr.t * t;
    ti = ti * tr.ti;

    r = tr.r * r;
    ri = ri * tr.ri;
}
std::ostream &operator<<(std::ostream &os, const transformation &tf)
{
    os << "Matrix : " << std::endl;
    os << tf.m << std::endl;
    os << "Inverse : " << std::endl;
    os << tf.i << std::endl;
    os << "Translation : " << std::endl;
    os << tf.t << std::endl;
    os << "Translation Inverse : " << std::endl;
    os << tf.ti << std::endl;
    os << "Rotation : " << std::endl;
    os << tf.r << std::endl;
    os << "Rotation Inverse : " << std::endl;
    os << tf.ri << std::endl;
    return os;
}