#ifndef __LIVE_BODY_H__
#define __LIVE_BODY_H__

#ifdef _MSC_VER
#pragma once
#endif

#include "body.hpp"
#include "animation.hpp"
#include <string>
#include <vector>

class live_body : public body {
public:
    class clip {   // Animation clip to play
    public:
        std::string name;
        animation anim;
    };
    class state {  // State of this body. State and animation are different concepts.
    public:
        std::string name;
        int clip_id;  // Only can have one animation clip per a state.
    };
    enum play_mode {
        STOP = 0,
        LIVE_STATIC,    // Play a single animation in [anim_pool].
        LIVE_DYNAMIC    // Play proper clip according to current state.
    };
    std::vector<clip>   anim_pool;      // Raw animation list.
    std::vector<clip>   clip_pool;      // Edited animation list.
    clip                cur_clip;       // Clip that is currently used ( LIVE_DYNAMIC ).
    std::vector<state>  state_pool;
    int anim_id = -1;                   // Animation in the [ anim_pool ] to play when in [ LIVE_STATIC ] play mode.
    int cur_state_id = -1;              // State in the [ state_pool ] to play when in [ LIVE_DYNAMIC ] play mode.
    int next_state_id = -1;             // State to be changed in the next step.
    int root_state = -1;                // When this body is in [ LIVE_DYNAMIC ] play mode, set to this state by default.
    play_mode pm = play_mode::STOP;

    // Motion interpolation (LIVE_DYNAMIC)
    int interp_step = 10;               // Number of steps that are influenced when interolating two different clips.
    
    // Check if there is same named anim. Return true if there is.
    bool duplicity_anim(const std::string &name) const noexcept;
    int add_anim(const std::string &name, const std::string &filename);  // Load animation from .bvh file.
    int add_anim(const clip &c);    // Return clip id.
    int add_anim(const std::string &name, const animation &anim);
    int get_anim_id(const std::string &name);

    // Check if there is same named clip. Return true if there is.
    bool duplicity_clip(const std::string &name) const noexcept;
    int add_clip(const clip &c);    // Return clip id.
    int add_clip(const std::string &name, const animation &anim);
    const clip& get_clip(int id) const;
    int get_clip_id(const std::string &name);
    
    // Check if there is same named state. Return true if there is.
    bool duplicity_state(const std::string &name) const noexcept;
    void add_state(const state &s);
    void add_state(const std::string &name, int clip_id);
    void add_state(const std::string &name, const std::string &clip_name);
    int get_state_id(const std::string &name) const;

    // Current & Next state.
    const state& get_cur_state_c() const;   // Get current state
    state& get_cur_state();   // Get current state
    const state& get_next_state_c() const;  // Get next state
    int get_cur_state_id() const noexcept;  // Get current state's id.
    int get_next_state_id() const noexcept; // Get next state's id.
    void set_cur_state_id(int id);
    void set_next_state_id(int id);

    void set_anim_id(int id);
    int get_anim_id() const noexcept;
    
    bool valid_anim_id(int id) const noexcept;
    bool valid_clip_id(int id) const noexcept;
    bool valid_state_id(int id) const noexcept;

    void set_root_state(int id);
    void set_root_state(const std::string &name);
    int get_root_state_id() const noexcept;
    const state& get_root_state_c() const;
    const clip& get_state_clip_c() const;   // Get current state's clip.
    clip& get_state_clip();
    bool valid_root_state() const noexcept;

    play_mode get_play_mode() const noexcept;
    void set_play_mode(play_mode pm);

    // Update animation with [ cur_state_id ], [ next_state_id ].
    // Motion interpolation occurs in two different cases :
    // 1. [ cur_state_id ] == [ next_state_id ], but a loop has ended.
    // 2. [ cur_state_id ] != [ next_state_id ]
    void update_animation() const;  // @TODO : It MUST not be const!
    // This function is called when we change [ next_state_id ].
    // Set [ next_state_id ] to [ nsi ], and find the closest motion in the state's animation.
    // The closest motion will be used in the next step.
    // void update_next_state(int nsi);

    // Renderer
    virtual void render_node(int id, const transformation &tr, double r, double g, double b) const noexcept;
    virtual void render_node(int id) const noexcept;
    virtual void render() const noexcept;
    virtual void render_ui();

    // Save & Load
    void save(const std::string &path); // Save current clip pool, state pool.
    void load(const std::string &path); // Load clip pool, state pool.
};

#endif