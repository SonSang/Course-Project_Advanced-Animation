#ifndef __CRENDERER_SCENE_MANAGER_H__
#define __CRENDERER_SCENE_MANAGER_H__

#ifdef _MSC_VER
#pragma once
#endif

#include "object_manager.hpp"
#include "render_manager.hpp"

// singleton, must exist once per scene.
class scene_manager
{
public:
    using uint = unsigned int;

private:
    object_manager object_man; // governs objects in the scene.
    render_manager render_man; // governs rendering details in the scene.

    uint cur_object;               // current target object.
    bool cur_object_valid = false; // current object validness.
public:
    scene_manager() = default;

    // object_man
    object_manager &get_object_manager() noexcept;
    void add_object_property(uint id, const std::shared_ptr<property> &property_) noexcept;
    void del_object_property(uint obj_id, uint prop_id);

    // render_man
    render_manager &get_render_manager() noexcept;

    // render funcs.
    void render() const noexcept;
    // render function for mouse pick support.
    void render_mouse_pick(void (*mouse_pick_callback)(unsigned int id)) const noexcept;

    // cur_object
    void set_cur_object(uint id);
    uint get_cur_object() const noexcept;
    void set_cur_object_valid(bool v);
    bool get_cur_object_valid() const noexcept;
};

#endif