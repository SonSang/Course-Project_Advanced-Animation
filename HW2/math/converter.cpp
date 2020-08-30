#include "converter.hpp"

void convert(const quaternion &from, mat4d &to)
{
    to.clear();
    double
        norm = from.norm(),
        s = 2.0 / norm,
        qxqx = from[0] * from[0],
        qxqy = from[0] * from[1],
        qxqz = from[0] * from[2],
        qxqw = from[0] * from[3],
        qyqy = from[1] * from[1],
        qyqz = from[1] * from[2],
        qyqw = from[1] * from[3],
        qzqz = from[2] * from[2],
        qzqw = from[2] * from[3];
    to.set(0, 0, 1 - s * (qyqy + qzqz));
    to.set(0, 1, s * (qxqy - qzqw));
    to.set(0, 2, s * (qxqz + qyqw));
    to.set(1, 0, s * (qxqy + qzqw));
    to.set(1, 1, 1 - s * (qxqx + qzqz));
    to.set(1, 2, s * (qyqz - qxqw));
    to.set(2, 0, s * (qxqz - qyqw));
    to.set(2, 1, s * (qyqz + qxqw));
    to.set(2, 2, 1 - s * (qyqy + qxqx));
    to.set(3, 3, 1);
}
