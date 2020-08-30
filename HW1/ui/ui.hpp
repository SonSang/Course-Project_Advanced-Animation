#ifndef __CRENDERER_UI_H__
#define __CRENDERER_UI_H__

#ifdef _MSC_VER
#pragma once
#endif

#include "GL/freeglut.h"

// sub ui
//#include "../header/ui_scene_graph.h"
#include "ui_camera.hpp"
#include "ui_object.hpp"
#include "../scene/scene_manager.hpp"

void ui_main_menu_bar();

// callbacks.
void ui_display_callback(scene_manager &scene_man);
void ui_keyboard_callback(unsigned char key, int x, int y);
void ui_keyboard_up_callback(unsigned char key, int x, int y);
void ui_special_callback(int key, int x, int y);
void ui_special_up_callback(int key, int x, int y);
void ui_mouse_button_callback(GLint button, GLint action, GLint x, GLint y);
void ui_mouse_move_callback(GLint x, GLint y);
void ui_mouse_wheel_callback(GLint button, GLint dir, GLint x, GLint y);
void ui_reshape_callback(GLint width, GLint height);

void ui_display_callback_beg(scene_manager &scene_man);
void ui_display_callback_end();

void ui_beg();
void ui_end();

#endif