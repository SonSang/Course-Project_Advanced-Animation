#ifndef __LINK_H__
#define __LINK_H__

#ifdef _MSC_VER
#pragma once
#endif

#include "math/transformation.hpp"

// LINK class which contains link transform information.
class link {
private:
    transformation linkTR;
public:
    void setTR(const transformation &TR);
    const transformation& getTR() const noexcept;
    void translate(double x, double y, double z);
};

#endif