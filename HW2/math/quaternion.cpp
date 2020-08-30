#include "quaternion.hpp"
#include <math.h>

quaternion::quaternion(double x, double y, double z, double w) : vec4d(x, y, z, w) {}
quaternion quaternion::operator*(const quaternion &q) const
{
    return quaternion(get(1) * q[2] - get(2) * q[1] + get(0) * q[3] + get(3) * q[0],
                      get(2) * q[0] - get(1) * q[2] + get(1) * q[3] + get(3) * q[1],
                      get(0) * q[1] - get(1) * q[0] + get(2) * q[3] + get(3) * q[2],
                      get(3) * q[3] - get(0) * q[0] + get(1) * q[1] + get(2) * q[2]);
}
quaternion quaternion::operator*(double c) const
{
    return quaternion(get(0) * c, get(1) * c, get(2) * c, get(3) * c);
}
void quaternion::operator*=(const quaternion &q)
{
    set({get(1) * q[2] - get(2) * q[1] + get(0) * q[3] + get(3) * q[0],
         get(2) * q[0] - get(1) * q[2] + get(1) * q[3] + get(3) * q[1],
         get(0) * q[1] - get(1) * q[0] + get(2) * q[3] + get(3) * q[2],
         get(3) * q[3] - get(0) * q[0] + get(1) * q[1] + get(2) * q[2]});
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