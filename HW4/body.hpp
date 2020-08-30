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
        int parent;
        std::set<int> children;
        std::shared_ptr<link>   linkptr;
        std::shared_ptr<joint>  jointptr;
        bool joint_type; // 0 for revolute, 1 for ball-and-socket.
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

        void set_joint_type(int type) noexcept;
        int get_joint_type() const noexcept;

        void set_bodypart(const box &box);
        const box& get_bodypart() const noexcept;

        void set_parent(int id);
        int get_parent() const noexcept;

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

public:
    std::map<int, node> tree;
    int root;
    int node_counter = 0;   // counter used to give unique id to each node.

    void (*ui_callback)();

    // animation settings.
    std::vector<vec3d> anim_pos;
    bool play_anim = false;
    int cur_anim_id = -1;    // current index of animation.
    int anim_speed = 1;
    stat_timer stimer;
    double frame_time = 3e-2;  // frame time of animation.
public:
    body();

    int get_node_id(const std::string &name) const noexcept;

    node& get_node(int id);
    const node& get_node_c(int id);

    node* get_node(const std::string &name);
    const node* get_node_c(const std::string &name) const;

    int get_size() const noexcept;

    int add_node(int parent, const node &node_); 

    int get_root() const noexcept;
    void set_root(int id);

    std::map<int, node>& get_tree() noexcept;
    const std::map<int, node>& get_tree_c() const noexcept;

    void set_play_anim(bool b) noexcept;
    bool get_play_anim() const noexcept;

    void set_cur_anim_id(int id);
    int get_cur_anim_id() const noexcept;

    void set_anim_speed(int speed);
    int get_anim_speed() const noexcept;

    void set_frame_time(double ft);
    double get_frame_time() const noexcept;

    std::vector<vec3d>& get_anim_pos() noexcept;
    const std::vector<vec3d>& get_anim_pos_c() const noexcept;
    void set_anim_pos(const std::vector<vec3d> &anim_pos) noexcept;

    std::vector<int> get_node_path(int node) const;         // get path from root to the specified node.
    transformation get_global_transform(const std::vector<int> path) const;
    transformation get_global_transform(int node) const;    // get global transformation of the specified node.

    void set_ui_callback(void (*ui_callback)());

    void render_box(const box &box, double r, double g, double b) const noexcept;
    virtual void render_node(int id, const transformation &tr, double r, double g, double b) const noexcept;
    virtual void render_node(int id) const noexcept;
    virtual void render() const noexcept;
    virtual void render_ui();

    // generate animation information to bring [ target_id ] node's transformation to [ target_tr ] transformation.
    // @deltaT : time interval used in the algorithm. 
    void inverse_kinematics(int target_id, const transformation &target_tr, double deltaT = 1e-3, int anim_size = 1000);
    void inverse_kinematics(const std::vector<int> &target_id, const std::vector<transformation> &target_tr, double deltaT = 1e-3, int anim_size = 1000, bool root_fix = true, double root_weight = 1);

    // conduct inverse kinematics in non-linear fashion.
    //void inverse_kinematics_nonlin(const std::vector<int> &target_id, const std::vector<transformation> &target_tr, bool root_fix = true);

    // get current Jacobian matrix for given body part.
    void jacobian(int target_id, double ***J, int &row, int &col, bool root_fix = true, double root_weight = 1) const;
    void jacobian(const std::vector<int> &target_id, double ***J, int &row, int &col, bool root_fix = true, double root_weight = 1) const;
};

#endif