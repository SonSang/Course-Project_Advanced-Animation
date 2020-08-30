#ifndef __CRENDERER_CAMERA_H__
#define __CRENDERER_CAMERA_H__

#ifdef _MSC_VER
#pragma once
#endif

#include "../math/vector.hpp"

class camera {
private:
    // position
    vec3d pos;
    vec3d front;
    vec3d up;

    // angle
    double yaw;
    double pitch;

    // speed
    double speed;

    // magnify
    double mag; // 1.0 for default(fovy = 25)
public:
    camera();

    // Set
    void set_position(double x, double y, double z);
    void set_position(const vec3d &v);
    void set_front(double x, double y, double z);
    void set_front(const vec3d &v);
    void set_up(double x, double y, double z);
    void set_up(const vec3d &v);

    vec3d get_position() const noexcept;
    vec3d get_front() const noexcept;
    vec3d get_up() const noexcept;

    void set_yaw(double yaw);
    void set_pitch(double pitch);

    double get_yaw() const noexcept;
    double get_pitch() const noexcept;

    void set_speed(double speed);
    double get_speed() const noexcept;

    void set_mag(double mag);
    double get_mag() const noexcept;

    // Move
    void move_back_forth(int forward, double deltaTime = 1e-1);
    void move_left_right(int right, double deltaTime = 1e-1);

    // Update
    void update_front();
    void update_angle(double yawOffset = 0, double pitchOffset = 0);
    // == Actual post redisplay only occurs here.
    void update_view();
    void update_proj(int width, int height);
};

#endif