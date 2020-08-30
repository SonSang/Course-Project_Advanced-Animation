#include "joint.hpp"

const transformation &joint::getTR() const noexcept {
    return this->jointTR;
}

void joint::setTR(const transformation &TR) noexcept {
    this->jointTR = TR;
}

// joint_revolute
vec3d joint_revolute::get_axis() const noexcept {
    return axis;
}
void joint_revolute::set_axis(const vec3d &axis) {
    this->axis = axis;
    this->axis.normalize();
}
void joint_revolute::rotate(double angle) {
    jointTR.rotate(vec3d(0, 0, 0), axis, angle);
}  

// joint_ballsocket
void joint_ballsocket::rotate(const vec3d &axis, double angle) {
    jointTR.rotate(vec3d(0, 0, 0), axis, angle);
}