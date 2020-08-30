#ifndef __BODY_H__
#define __BODY_H__

#ifdef _MSC_VER
#pragma once
#endif

#include "scene/property_render.hpp"
#include "scene/geometry.hpp"
#include "math/timer.hpp"
#include "link.hpp"
#include "joint.hpp"

#include <memory>
#include <set>
#include <map>
#include <vector>

// This class specifies tree structure of entire body with LINK & JOINT.
class body : public property_render {
public:
    class node {
    private:
        std::string name;
        std::set<int> children;
        std::shared_ptr<link>   linkptr;
        std::shared_ptr<joint>  jointptr;
        box bodypart; // used for rendering.

        // animation.
        std::vector<transformation> anim;  // array of JOINT transformations.
    public:
        void set_name(const std::string &name);
        std::string get_name() const noexcept;

        void set_link(const std::shared_ptr<link> linkptr);
        void set_joint(const std::shared_ptr<joint> jointptr);

        std::shared_ptr<link> get_link() const noexcept;
        std::shared_ptr<joint> get_joint() const noexcept;

        void set_bodypart(const box &box);
        const box& get_bodypart() const noexcept;

        void add_child(int id);
        void del_child(int id);
        void set_children(const std::set<int> &children) noexcept;
        const std::set<int>& get_childern() const noexcept;

        // animation
        std::vector<transformation>& get_anim() noexcept;
        const std::vector<transformation>& get_anim_c() const noexcept;
        void set_anim(const std::vector<transformation> &anim) noexcept;
        bool valid_anim_id(int id) const noexcept;
        void validate_anim_id(int &id) const noexcept;  // validate given id if it was invalid.
    };

private:
    std::map<int, node> tree;
    int root;
    int node_counter = 0;   // counter used to give unique id to each node.

    void (*ui_callback)();

    // animation settings.
    std::vector<vec3d> anim_pos;
    bool play_anim = false;
    int cur_anim_id = -1;    // current index of animation.
    stat_timer stimer;
    double frame_time = 0;  // frame time of animation.
public:
    body();

    node& get_node(int id);
    const node& get_node_c(int id);
    int get_size() const noexcept;

    int add_node(int parent, const node &node_); 

    int get_root() const noexcept;
    void set_root(int id);

    void set_play_anim(bool b) noexcept;
    bool get_play_anim() const noexcept;

    void set_cur_anim_id(int id);
    int get_cur_anim_id() const noexcept;

    void set_frame_time(double ft);
    double get_frame_time() const noexcept;

    std::vector<vec3d>& get_anim_pos() noexcept;
    const std::vector<vec3d>& get_anim_pos_c() const noexcept;
    void set_anim_pos(const std::vector<vec3d> &anim_pos) noexcept;

    void set_ui_callback(void (*ui_callback)());

    void render_node(int id) const noexcept;
    virtual void render() const noexcept;
    virtual void render_ui();
};

#endif