#include "property_render_geometry.hpp"
#include "render_basic.hpp"
#include "GL/freeglut.h"

#define prg property_render_geometry

// draw specialization for each geometries.
template<>
void prg<point>::render() const noexcept
{

    glColor3d(config_.inner_color[0], config_.inner_color[1], config_.inner_color[2]);
    glPointSize((GLfloat)config_.size);
    glBegin(GL_POINTS);
    gl_draw_vertex(geometry_.get_point());
    glEnd();
}
template<>
void prg<line>::render() const noexcept
{

    glColor3d(config_.inner_color[0], config_.inner_color[1], config_.inner_color[2]);
    glLineWidth((GLfloat)config_.size);
    glBegin(GL_LINES);
    gl_draw_vertex(geometry_.get_beg());
    gl_draw_vertex(geometry_.get_end());
    glEnd();
}
template<>
void prg<triangle>::render() const noexcept
{

    // border
    if (config_.border || config_.wireframe)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glLineWidth((GLfloat)config_.size);
        glColor3d(config_.outer_color[0], config_.outer_color[1], config_.outer_color[2]);
        glBegin(GL_TRIANGLES);
        gl_draw_vertex(geometry_.get_vertices().at(0));
        gl_draw_vertex(geometry_.get_vertices().at(1));
        gl_draw_vertex(geometry_.get_vertices().at(2));
        glEnd();
    }
    // inner
    if (!config_.wireframe)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glColor3d(config_.inner_color[0], config_.inner_color[1], config_.inner_color[2]);
        glBegin(GL_TRIANGLES);
        gl_draw_vertex(geometry_.get_vertices().at(0));
        gl_draw_vertex(geometry_.get_vertices().at(1));
        gl_draw_vertex(geometry_.get_vertices().at(2));
        glEnd();
    }
}
template<>
void prg<quad>::render() const noexcept
{

    // border
    if (config_.border || config_.wireframe)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glLineWidth((GLfloat)config_.size);
        glColor3d(config_.outer_color[0], config_.outer_color[1], config_.outer_color[2]);
        glBegin(GL_QUADS);
        gl_draw_vertex(geometry_.get_vertices().at(0));
        gl_draw_vertex(geometry_.get_vertices().at(1));
        gl_draw_vertex(geometry_.get_vertices().at(2));
        gl_draw_vertex(geometry_.get_vertices().at(3));
        glEnd();
    }
    // inner
    if (!config_.wireframe)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glColor3d(config_.inner_color[0], config_.inner_color[1], config_.inner_color[2]);
        glBegin(GL_QUADS);
        gl_draw_vertex(geometry_.get_vertices().at(0));
        gl_draw_vertex(geometry_.get_vertices().at(1));
        gl_draw_vertex(geometry_.get_vertices().at(2));
        gl_draw_vertex(geometry_.get_vertices().at(3));
        glEnd();
    }
}
template<>
void prg<box>::render() const noexcept
{

    // border
    if (config_.border || config_.wireframe)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glLineWidth((GLfloat)config_.size);
        glColor3d(config_.outer_color[0], config_.outer_color[1], config_.outer_color[2]);
        for (int i = 0; i < 6; i++)
        {
            quad
                quad_ = geometry_.get_face(i);
            glBegin(GL_QUADS);
            gl_draw_vertex(quad_.get_vertices().at(0));
            gl_draw_vertex(quad_.get_vertices().at(1));
            gl_draw_vertex(quad_.get_vertices().at(2));
            gl_draw_vertex(quad_.get_vertices().at(3));
            glEnd();
        }
    }
    // inner
    if (!config_.wireframe)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glColor3d(config_.inner_color[0], config_.inner_color[1], config_.inner_color[2]);
        for (int i = 0; i < 6; i++)
        {
            quad
                quad_ = geometry_.get_face(i);
            glBegin(GL_QUADS);
            gl_draw_vertex(quad_.get_vertices().at(0));
            gl_draw_vertex(quad_.get_vertices().at(1));
            gl_draw_vertex(quad_.get_vertices().at(2));
            gl_draw_vertex(quad_.get_vertices().at(3));
            glEnd();
        }
    }
}
template<>
void prg<sphere>::render() const noexcept
{

    // border
    if (config_.border || config_.wireframe)
    {
        glColor3d(config_.outer_color[0], config_.outer_color[1], config_.outer_color[2]);
        glLineWidth((GLfloat)config_.size);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();

        glTranslated(geometry_.get_cen()[0], geometry_.get_cen()[1], geometry_.get_cen()[2]);
        glutWireSphere(geometry_.get_rad(), 10, 10);

        glPopMatrix();
    }
    // inner
    if (!config_.wireframe)
    {
        glColor3d(config_.inner_color[0], config_.inner_color[1], config_.inner_color[2]);
        glLineWidth((GLfloat)config_.size);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();

        glTranslated(geometry_.get_cen()[0], geometry_.get_cen()[1], geometry_.get_cen()[2]);
        glutSolidSphere(geometry_.get_rad(), 10, 10);

        glPopMatrix();
    }
}
template<>
void prg<mesh2>::render() const noexcept
{

    glColor3d(config_.inner_color[0], config_.inner_color[1], config_.inner_color[2]);
    glLineWidth((GLfloat)config_.size);
    glBegin(GL_LINE_STRIP);
    for (auto it = geometry_.get_vertices().begin(); it != geometry_.get_vertices().end(); it++)
        gl_draw_vertex(*it);
    glEnd();
}
template<>
void prg<mesh3>::render() const noexcept
{

    // border
    if (config_.border || config_.wireframe)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glLineWidth((GLfloat)config_.size);
        glColor3d(config_.outer_color[0], config_.outer_color[1], config_.outer_color[2]);
        for (auto it = geometry_.get_indices().begin(); it != geometry_.get_indices().end(); it++)
        {
            glBegin(GL_TRIANGLES);
            gl_draw_vertex(geometry_.get_vertices().at((*it)[0]));
            gl_draw_vertex(geometry_.get_vertices().at((*it)[1]));
            gl_draw_vertex(geometry_.get_vertices().at((*it)[2]));
            glEnd();
        }
    }
    // inner
    if (!config_.wireframe)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glColor3d(config_.inner_color[0], config_.inner_color[1], config_.inner_color[2]);
        for (auto it = geometry_.get_indices().begin(); it != geometry_.get_indices().end(); it++)
        {
            glBegin(GL_TRIANGLES);
            gl_draw_vertex(geometry_.get_vertices().at((*it)[0]));
            gl_draw_vertex(geometry_.get_vertices().at((*it)[1]));
            gl_draw_vertex(geometry_.get_vertices().at((*it)[2]));
            glEnd();
        }
    }
}
template<>
void prg<mesh4>::render() const noexcept
{

    // border
    if (config_.border || config_.wireframe)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glLineWidth((GLfloat)config_.size);
        glColor3d(config_.outer_color[0], config_.outer_color[1], config_.outer_color[2]);
        for (auto it = geometry_.get_indices().begin(); it != geometry_.get_indices().end(); it++)
        {
            glBegin(GL_QUADS);
            gl_draw_vertex(geometry_.get_vertices().at((*it)[0]));
            gl_draw_vertex(geometry_.get_vertices().at((*it)[1]));
            gl_draw_vertex(geometry_.get_vertices().at((*it)[2]));
            gl_draw_vertex(geometry_.get_vertices().at((*it)[3]));
            glEnd();
        }
    }
    // inner
    if (!config_.wireframe)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glColor3d(config_.inner_color[0], config_.inner_color[1], config_.inner_color[2]);
        for (auto it = geometry_.get_indices().begin(); it != geometry_.get_indices().end(); it++)
        {
            glBegin(GL_QUADS);
            gl_draw_vertex(geometry_.get_vertices().at((*it)[0]));
            gl_draw_vertex(geometry_.get_vertices().at((*it)[1]));
            gl_draw_vertex(geometry_.get_vertices().at((*it)[2]));
            gl_draw_vertex(geometry_.get_vertices().at((*it)[3]));
            glEnd();
        }
    }
}
template<>
void prg<mesh4normal>::render() const noexcept
{
    // border
    if (config_.border || config_.wireframe)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glLineWidth((GLfloat)config_.size);
        glColor3d(config_.outer_color[0], config_.outer_color[1], config_.outer_color[2]);
        for (auto it = geometry_.get_indices().begin(); it != geometry_.get_indices().end(); it++)
        {
            glBegin(GL_QUADS);
            gl_draw_vertex(geometry_.get_vertex_normals().at((*it)[0]).first);
            gl_draw_normal(geometry_.get_vertex_normals().at((*it)[0]).second);
            gl_draw_vertex(geometry_.get_vertex_normals().at((*it)[1]).first);
            gl_draw_normal(geometry_.get_vertex_normals().at((*it)[1]).second);
            gl_draw_vertex(geometry_.get_vertex_normals().at((*it)[2]).first);
            gl_draw_normal(geometry_.get_vertex_normals().at((*it)[2]).second);
            gl_draw_vertex(geometry_.get_vertex_normals().at((*it)[3]).first);
            gl_draw_normal(geometry_.get_vertex_normals().at((*it)[3]).second);
            glEnd();
        }
    }
    // inner
    if (!config_.wireframe)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glColor3d(config_.inner_color[0], config_.inner_color[1], config_.inner_color[2]);
        for (auto it = geometry_.get_indices().begin(); it != geometry_.get_indices().end(); it++)
        {
            glBegin(GL_QUADS);
            gl_draw_vertex(geometry_.get_vertex_normals().at((*it)[0]).first);
            gl_draw_normal(geometry_.get_vertex_normals().at((*it)[0]).second);
            gl_draw_vertex(geometry_.get_vertex_normals().at((*it)[1]).first);
            gl_draw_normal(geometry_.get_vertex_normals().at((*it)[1]).second);
            gl_draw_vertex(geometry_.get_vertex_normals().at((*it)[2]).first);
            gl_draw_normal(geometry_.get_vertex_normals().at((*it)[2]).second);
            gl_draw_vertex(geometry_.get_vertex_normals().at((*it)[3]).first);
            gl_draw_normal(geometry_.get_vertex_normals().at((*it)[3]).second);
            glEnd();
        }
    }
}