#ifndef __BODY_H__
#define __BODY_H__

#ifdef _MSC_VER
#pragma once
#endif

#include "scene/property_render.hpp"
#include "scene/geometry.hpp"
#include "link.hpp"
#include "joint.hpp"

#include <memory>
#include <set>
#include <map>

// This class specifies tree structure of entire body with LINK & JOINT.

class body : public property_render {
public:
    class node {
    private:
        std::set<int> children;
        std::shared_ptr<link>   linkptr;
        std::shared_ptr<joint>  jointptr;
        box bodypart; // used for rendering.
    public:
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
    };

private:
    std::map<int, node> tree;
    int root;
    int node_counter = 0;   // counter used to give unique id to each node.

    void (*ui_callback)();
public:
    body();

    node& get_node(int id);
    const node& get_node_c(int id);

    int add_node(int parent, const node &node_); 

    int get_root() const noexcept;
    void set_root(int id);

    void set_ui_callback(void (*ui_callback)());

    void render_node(int id) const noexcept;
    virtual void render() const noexcept;
    virtual void render_ui();
};

#endif