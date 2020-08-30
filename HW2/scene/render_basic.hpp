#ifndef __CRENDERER_RENDER_BASIC_H__
#define __CRENDERER_RENDER_BASIC_H__

#ifdef _MSC_VER
#pragma once
#endif

#include "../math/vector.hpp"

void gl_draw_vertex(const vec3d &v);
void gl_draw_normal(const vec3d &v);

#endif