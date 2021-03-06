#include "camera.hpp"
#include "../math/math.hpp"
#include "GL/freeglut.h"

#include <math.h>

#define DEFAULT_POSITION		0, 50, 150   
#define DEFAULT_FRONT			0, 0, -1
#define DEFAULT_UP				0, 1, 0

#define PITCH_UPPER_BOUND		89
#define PITCH_LOWER_BOUND		-89
#define DEFAULT_YAW				-90
#define DEFAULT_PITCH			0

#define DEFAULT_SPEED			10
#define DEFAULT_FOVY			25
#define DEFAULT_MAG				1

void call_glu_look_at(const vec3d &pos, const vec3d &cen, const vec3d &up)
{
    gluLookAt(pos[0], pos[1], pos[2], cen[0], cen[1], cen[2], up[0], up[1], up[2]);
}

camera::camera() : pos(DEFAULT_POSITION), front(DEFAULT_FRONT), up(DEFAULT_UP), yaw(DEFAULT_YAW), pitch(DEFAULT_PITCH), speed(DEFAULT_SPEED), mag(DEFAULT_MAG)
{
}

void camera::set_position(double x, double y, double z)
{
    this->pos.set(x, y, z);
    update_view();
}
void camera::set_position(const vec3d &v)
{
    this->pos = v;
    update_view();
}
void camera::set_front(double x, double y, double z)
{
    this->front.set(x, y, z);
    update_view();
}
void camera::set_front(const vec3d &v)
{
    this->front = v;
    update_view();
}
void camera::set_up(double x, double y, double z)
{
    this->up.set(x, y, z);
    update_view();
};
void camera::set_up(const vec3d &v)
{
    this->up = v;
    update_view();
}

vec3d camera::get_position() const noexcept
{
    return this->pos;
}
vec3d camera::get_front() const noexcept
{
    return this->front;
}
vec3d camera::get_up() const noexcept
{
    return this->up;
}

void camera::set_yaw(double yaw)
{
    this->yaw = yaw;
}
void camera::set_pitch(double pitch)
{
    this->pitch = pitch;
}

double camera::get_yaw() const noexcept
{
    return this->yaw;
}
double camera::get_pitch() const noexcept
{
    return this->pitch;
}

void camera::set_speed(double speed)
{
    this->speed = speed;
}
double camera::get_speed() const noexcept
{
    return this->speed;
}

void camera::set_mag(double mag)
{
    this->mag = mag;
}
double camera::get_mag() const noexcept
{
    return this->mag;
}

// Move
void camera::move_back_forth(int forward, double deltaTime)
{
    double
        distance = get_speed() * deltaTime * forward;
    vec3d
        addition = get_front() * distance,
        nposition = get_position() + addition;
    set_position(nposition);
}
void camera::move_left_right(int right, double deltaTime)
{
    double
        distance = get_speed() * deltaTime * right;
    vec3d
        rvector = get_front().cross(get_up());
    rvector.normalize();
    vec3d
        addition = rvector * distance,
        nposition = get_position() + addition;
    set_position(nposition);
}

// Update
void camera::update_front()
{
    double
        pitchR = deg2rad(get_pitch()),
        yawR = deg2rad(get_yaw());
    vec3d
        nfront(cos(pitchR) * cos(yawR), sin(pitchR), cos(pitchR) * sin(yawR));
    set_front(nfront);
}
void camera::update_angle(double yawOffset, double pitchOffset)
{
    double
        nyaw = get_yaw() + yawOffset,
        npitch = get_pitch() + pitchOffset;
    if (npitch > PITCH_UPPER_BOUND)
        npitch = PITCH_UPPER_BOUND;
    else if (npitch < PITCH_LOWER_BOUND)
        npitch = PITCH_LOWER_BOUND;
    set_yaw(nyaw);
    set_pitch(npitch);
    update_front();
}
// == Actual post redisplay only occurs here.
void camera::update_view()
{
    vec3d
        center = get_position() + get_front();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    call_glu_look_at(get_position(), center, get_up());
    glutPostRedisplay();
}
void camera::update_proj(int width, int height)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(DEFAULT_FOVY * get_mag(), width / (double)height, 0.001, 2500);
}
