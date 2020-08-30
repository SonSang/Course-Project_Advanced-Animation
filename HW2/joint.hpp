#ifndef __JOINT_H__
#define __JOINT_H__

#ifdef _MSC_VER
#pragma once
#endif

#include "math/vector.hpp"
#include "math/transformation.hpp"
#include "link.h"

class joint {
protected:
    transformation jointTR;
public:
    const transformation& getTR() const noexcept;
    void setTR(const transformation &TR) noexcept;
};

// Class for revolute joint. 
class joint_revolute : public joint {
private:
    vec3d axis;
    double angle;   // radian angle.
public:
    void set_axis(const vec3d &axis);
    void rotate(double angle);                      // rotate around [axis] for given angle.
};

// Class for ball-and-socket joint. 
class joint_ballsocket : public joint {
public:
    void rotate(const vec3d &axis, double angle);   // rotate around arbitrary axis for given angle.
};

#endif