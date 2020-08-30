#ifndef __CMATH_CONVERTER_H__
#define __CMATH_CONVERTER_H__

#ifdef _MSC_VER
#pragma once
#endif

#include "matrix.hpp"
#include "quaternion.hpp"

// template <typename F, typename T>
// void convert(const F &from, T &to)
// {
//     static_assert(0, "There is no available converter for these types.");
// }

void convert(const quaternion &from, mat4d &to);

#endif