#ifndef __CRENDERER_UI_CAMERA_H__
#define __CRENDERER_UI_CAMERA_H__

#ifdef _MSC_VER
#pragma once
#endif

#include "../scene/camera.hpp"

void ui_camera_config_window();
void ui_set_main_camera(camera &camera_); // set ui-controllable camera.

// Manipulate [show] var.
void set_show_camera_config_window(bool b);
bool get_show_camera_config_window();

#endif