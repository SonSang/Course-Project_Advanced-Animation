#include "render_basic.hpp"
#include "GL/freeglut.h"

void gl_draw_vertex(const vec3d &v)
{
    glVertex3d(v[0], v[1], v[2]);
}
void gl_draw_normal(const vec3d &v)
{
    glNormal3d(v[0], v[1], v[2]);
}
