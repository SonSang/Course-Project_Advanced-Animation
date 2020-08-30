#include "scene_manager.hpp"
#include "render_basic.hpp"
#include "GL/freeglut.h"

#include <typeinfo>

// object_man
object_manager &scene_manager::get_object_manager() noexcept
{
    return this->object_man;
}
void scene_manager::add_object_property(uint id, const std::shared_ptr<property> &property_) noexcept
{
    this->object_man.get_object(id)->add_property(property_);
    if (std::dynamic_pointer_cast<property_render>(property_))
    { // if [property_] is derived class of [property_render]
        if (!this->render_man.has_node(id))
        {
            render_manager::node
                new_node(this->object_man.get_object(id));
            this->render_man.add_node(new_node, -1, true); // add this object to [render_man].
        }
    }
}
void scene_manager::del_object_property(uint obj_id, uint prop_id)
{
}

// render_man
render_manager &scene_manager::get_render_manager() noexcept
{
    return this->render_man;
}
static void draw_transform_arrows(const vec3d &origin)
{
    // x arrow
    {
        glColor3d(1, 0, 0);
        glLineWidth(10);
        glBegin(GL_LINES);
        gl_draw_vertex(origin);
        gl_draw_vertex(origin + vec3d(1, 0, 0));
        glEnd();
    }
    // y arrow
    {
        glColor3d(0, 1, 0);
        glLineWidth(10);
        glBegin(GL_LINES);
        gl_draw_vertex(origin);
        gl_draw_vertex(origin + vec3d(0, 1, 0));
        glEnd();
    }
    // z arrow
    {
        glColor3d(0, 0, 1);
        glLineWidth(10);
        glBegin(GL_LINES);
        gl_draw_vertex(origin);
        gl_draw_vertex(origin + vec3d(0, 0, 1));
        glEnd();
    }
}
void scene_manager::render() const noexcept
{
    this->render_man.render();

    // if [cur_object] exists, render arrows for object transformation.
    if (get_cur_object_valid())
    {
        vec3d
            o = object_man.get_object_c(get_cur_object() - 4)->get_loc_origin(); // minus 4 is necessary...;
        draw_transform_arrows(o);
    }
}

static void draw_transform_arrows_mouse_pick(const vec3d &origin, void (*mouse_pick_callback)(unsigned int id))
{
    // x arrow
    {
        mouse_pick_callback(1); //	 1 for x arrow.
        glColor3d(1, 0, 0);
        glLineWidth(10);
        glBegin(GL_LINES);
        gl_draw_vertex(origin);
        gl_draw_vertex(origin + vec3d(1, 0, 0));
        glEnd();
    }
    // y arrow
    {
        mouse_pick_callback(2); //	 2 for y arrow.
        glColor3d(0, 1, 0);
        glLineWidth(10);
        glBegin(GL_LINES);
        gl_draw_vertex(origin);
        gl_draw_vertex(origin + vec3d(0, 1, 0));
        glEnd();
    }
    // z arrow
    {
        mouse_pick_callback(3); //	 3 for x arrow.
        glColor3d(0, 0, 1);
        glLineWidth(10);
        glBegin(GL_LINES);
        gl_draw_vertex(origin);
        gl_draw_vertex(origin + vec3d(0, 0, 1));
        glEnd();
    }
}
void scene_manager::render_mouse_pick(void (*mouse_pick_callback)(unsigned int id)) const noexcept
{
    this->render_man.render_mouse_pick(mouse_pick_callback);

    // if [cur_object] exists, render arrows for object transformation.
    if (get_cur_object_valid())
    {
        vec3d
            o = object_man.get_object_c(get_cur_object() - 4)->get_loc_origin(); // minus 4 is necessary...;
        draw_transform_arrows_mouse_pick(o, mouse_pick_callback);
    }
}

// cur_object
void scene_manager::set_cur_object(uint id)
{
    this->cur_object = id;
}
scene_manager::uint scene_manager::get_cur_object() const noexcept
{
    return this->cur_object;
}
void scene_manager::set_cur_object_valid(bool v)
{
    cur_object_valid = v;
}
bool scene_manager::get_cur_object_valid() const noexcept
{
    return this->cur_object_valid;
}