#include "quaternion.hpp"
#include "math.hpp"
#include <math.h>

quaternion::quaternion(double x, double y, double z, double w) : vec4d(x, y, z, w) {}
quaternion quaternion::operator*(const quaternion &q) const
{
    double x1, y1, z1, w1;
    double x2, y2, z2, w2;
    x1 = get(0); y1 = get(1); z1 = get(2); w1 = get(3);
    x2 = q.get(0); y2 = q.get(1); z2 = q.get(2); w2 = q.get(3);
    return quaternion(  x1 * w2 + w1 * x2 + y1 * z2 - z1 * y2,
                        y1 * w2 + w1 * y2 + z1 * x2 - x1 * z2,
                        w1 * z2 + z1 * w2 + x1 * y2 - x2 * y1,
                        w1 * w2 - x1 * x2 - y1 * y2 - z1 * z2);
    // return quaternion(get(1) * q[2] - get(2) * q[1] + get(0) * q[3] + get(3) * q[0],
    //                   get(2) * q[0] - get(1) * q[2] + get(1) * q[3] + get(3) * q[1],
    //                   get(0) * q[1] - get(1) * q[0] + get(2) * q[3] + get(3) * q[2],
    //                   get(3) * q[3] - get(0) * q[0] + get(1) * q[1] + get(2) * q[2]);
}
quaternion quaternion::operator*(double c) const
{
    return quaternion(get(0) * c, get(1) * c, get(2) * c, get(3) * c);
}
void quaternion::operator*=(const quaternion &q)
{
    *this = (*this) * q;
    // set({get(1) * q[2] - get(2) * q[1] + get(0) * q[3] + get(3) * q[0],
    //      get(2) * q[0] - get(1) * q[2] + get(1) * q[3] + get(3) * q[1],
    //      get(0) * q[1] - get(1) * q[0] + get(2) * q[3] + get(3) * q[2],
    //      get(3) * q[3] - get(0) * q[0] + get(1) * q[1] + get(2) * q[2]});
}
void quaternion::operator*=(double c)
{
    set({get(0) * c, get(1) * c, get(2) * c, get(3) * c});
}
double quaternion::norm() const noexcept
{
    return length();
}
double quaternion::normsq() const noexcept
{
    return get(0) * get(0) + get(1) * get(1) + get(2) * get(2) + get(3) * get(3);
}
quaternion quaternion::conjugate() const
{
    return quaternion(-get(0), -get(1), -get(2), get(3));
}
quaternion quaternion::inverse() const
{
    quaternion
        conj = conjugate();
    double
        ns = normsq(),
        denom = 1.0 / ns;
    return (conj * denom);
}
quaternion quaternion::rotation(double ax, double ay, double az, double radian)
{
    double
        phi = radian * 0.5,
        sinp = sin(phi),
        cosp = cos(phi);
    vec3d
        normalAxis(ax, ay, az);
    normalAxis.normalize();
    normalAxis *= sinp;
    return quaternion(normalAxis[0], normalAxis[1], normalAxis[2], cosp);
}

vec3d quaternion::log() const {
    double 
        w = get(3),
        x = get(0),
        y = get(1),
        z = get(2);
    vec3d
        v;
    if(w == 0.0) 
        v = vec3d(x, y, z) * PI * 0.5;
    else if(w == 1.0) 
        v.set(0, 0, 0);
    else {
        v = vec3d(x, y, z);
        double vlen = v.length();
        double mult = atan(vlen / w);
        mult = (mult < 0) ? mult + PI : mult;   // continuity of [atan] at w == 0.
        v = (v / vlen) * mult;
    }
    return v;
}