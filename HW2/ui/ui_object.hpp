#ifndef __CRENDERER_UI_OBJECT_H__
#define __CRENDERER_UI_OBJECT_H__

#ifdef _MSC_VER
#pragma once
#endif

#include "../scene/scene_manager.hpp"

void ui_object_window(scene_manager &scene_man);

// [show] var.
void set_show_object_window(bool b);
bool get_show_object_window();

//void ui_set_main_object_list(object_list &mol_);
//void ui_object_set_main_scene_graph(scene_graph &msg_);

#endif