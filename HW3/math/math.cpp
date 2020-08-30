#include "math.hpp"
#include <math.h>

double deg2rad(double degree)
{
    return (degree / 180.0) * PI;
}
double rad2deg(double radian)
{
    return (radian / PI) * 180.0;
}
double power(double base, double n)
{
    return (double)powl(base, n);
}
double max(double a, double b)
{
    return (a > b) ? a : b;
}