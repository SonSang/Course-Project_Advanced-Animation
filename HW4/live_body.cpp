#include "live_body.hpp"
#include "bvh_parser.hpp"
#include "imgui/imgui_include_freeglut.hpp"
#include "math/converter.hpp"
#include <fstream>
#include <iostream>
#include <GL/freeglut.h>

bool live_body::duplicity_anim(const std::string &name) const noexcept {
    for(auto it = anim_pool.begin(); it != anim_pool.end(); it++) {
        if(it->name == name)
            return true;
    }
    return false;
}
int live_body::add_anim(const std::string &name, const std::string &filename) {
    if(duplicity_anim(name))
        throw std::invalid_argument(name + " already exists.");
    auto anim = bvh_parser::parse_anim(filename);
    return add_anim(name, anim);
}
int live_body::add_anim(const clip &c) {
    if(duplicity_anim(c.name))
        throw std::invalid_argument(c.name + " already exists.");
    anim_pool.push_back(c);
    return (int)anim_pool.size() - 1;
}
int live_body::add_anim(const std::string &name, const animation &anim) {
    clip c;
    c.name = name;
    c.anim = anim;
    return add_anim(c);
}
int live_body::get_anim_id(const std::string &name) {
    for(auto it = anim_pool.begin(); it != anim_pool.end(); it++) {
        if(it->name == name)
            return std::distance(anim_pool.begin(), it);
    }
    throw std::invalid_argument(name + " is not valid anim name.");
}
bool live_body::duplicity_clip(const std::string &name) const noexcept {
    for(auto it = clip_pool.begin(); it != clip_pool.end(); it++) {
        if(it->name == name)
            return true;
    }
    return false;
}
int live_body::add_clip(const clip &c) {
    if(duplicity_clip(c.name))
        throw std::invalid_argument(c.name + " already exists.");
    clip_pool.push_back(c);
    return (int)clip_pool.size() - 1;
}
int live_body::add_clip(const std::string &name, const animation &anim) {
    if(duplicity_clip(name))
        throw std::invalid_argument(name + " already exists.");
    clip nc;
    nc.name = name;
    nc.anim = anim;
    clip_pool.push_back(nc);
    return (int)clip_pool.size() - 1;
}
const live_body::clip& live_body::get_clip(int id) const {
    return clip_pool.at(id);
}
int live_body::get_clip_id(const std::string &name) {
    for(auto it = clip_pool.begin(); it != clip_pool.end(); it++) {
        if(it->name == name)
            return std::distance(clip_pool.begin(), it);
    }
    throw std::invalid_argument(name + std::string(" is not valid clip name"));
}
bool live_body::duplicity_state(const std::string &name) const noexcept {
    for(auto it = state_pool.begin(); it != state_pool.end(); it++) {
        if(it->name == name)
            return true;
    }
    return false;
}
void live_body::add_state(const state &s) {
    if(duplicity_state(s.name))
        throw std::invalid_argument(s.name + " already exists.");
    state_pool.push_back(s);
}
void live_body::add_state(const std::string &name, int clip_id) {
    if(duplicity_state(name))
        throw std::invalid_argument(name + " already exists.");
    state ns;
    ns.name = name;
    ns.clip_id = clip_id;
    state_pool.push_back(ns);
}
void live_body::add_state(const std::string &name, const std::string &clip_name) {
    int
        clip_id = get_clip_id(clip_name);
    add_state(name, clip_id);
}
const live_body::state& live_body::get_cur_state_c() const {
    return state_pool.at(get_cur_state_id());
}
live_body::state& live_body::get_cur_state() {
    return state_pool.at(get_cur_state_id());
}
const live_body::state& live_body::get_next_state_c() const {
    return state_pool.at(get_next_state_id());
}
int live_body::get_state_id(const std::string &name) const {
    for(auto it = state_pool.begin(); it != state_pool.end(); it++) {
        if(it->name == name)
            return std::distance(state_pool.begin(), it);
    }
    throw std::invalid_argument(name + " is not valid state name.");
}
void live_body::set_anim_id(int id) {
    this->anim_id = id;
}
int live_body::get_anim_id() const noexcept {
    return this->anim_id;
}
bool live_body::valid_clip_id(int id) const noexcept {
    if(id < 0 || id >= clip_pool.size())
        return false;
    return true;
}
void live_body::set_cur_state_id(int id) {
    this->cur_state_id = id;
}
void live_body::set_next_state_id(int id) {
    this->next_state_id = id;
}
int live_body::get_cur_state_id() const noexcept {
    return this->cur_state_id;
}
int live_body::get_next_state_id() const noexcept {
    return this->next_state_id;
}
void live_body::set_root_state(int id) {
    root_state = id;
}
void live_body::set_root_state(const std::string &name) {
    int id = get_state_id(name);
    set_root_state(id);
}
live_body::play_mode live_body::get_play_mode() const noexcept {
    return this->pm;
}
void live_body::set_play_mode(play_mode pm) {
    this->pm = pm;

    if(pm == LIVE_STATIC) {
        anim_pool.at(anim_id).anim.set_frame_id(0);
    }
}
void live_body::render_node(int id, const transformation &tr, double r, double g, double b) const noexcept {
    body::render_node(id, tr, r, g, b);
}
void live_body::render_node(int id) const noexcept {
    // Render according to current play mode.
    if(get_play_mode() == STOP) {
        const node& n = tree.at(id);

        // apply transformation. Link -> Joint.
        double linkmat[16];
    
        n.get_link()->getTR().get_matrix_transpose(linkmat);
    
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glMultMatrixd(linkmat);

        // render this node.
        render_box(n.get_bodypart(), 0.5, 0.5, 0.5);

        for(auto it = n.get_childern().begin(); it != n.get_childern().end(); it++) 
            render_node(*it);

        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
    }
    else if(get_play_mode() == LIVE_STATIC) {
        const node& n = tree.at(id);

        // apply transformation. Link -> Joint.
        double linkmat[16];
        double jointmat[16];
    
        n.get_link()->getTR().get_matrix_transpose(linkmat);
        anim_pool.at(anim_id).anim.get_valid_single_frame(n.get_name()).get_matrix_transpose(jointmat);
        
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glMultMatrixd(linkmat);
        glMultMatrixd(jointmat);

        // render this node.
        render_box(n.get_bodypart(), 0.5, 0.5, 0.5);

        for(auto it = n.get_childern().begin(); it != n.get_childern().end(); it++) 
            render_node(*it);

        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
    }
    else {  // LIVE_DYNAMIC
        const node& n = tree.at(id);

        // apply transformation. Link -> Joint.
        double linkmat[16];
        double jointmat[16];
    
        n.get_link()->getTR().get_matrix_transpose(linkmat);
        cur_clip.anim.get_single_frame(n.get_name()).get_matrix_transpose(jointmat);
        
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glMultMatrixd(linkmat);
        glMultMatrixd(jointmat);

        // render this node.
        render_box(n.get_bodypart(), 0.5, 0.5, 0.5);

        for(auto it = n.get_childern().begin(); it != n.get_childern().end(); it++) 
            render_node(*it);

        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
    }
}
void live_body::render() const noexcept {
    if(get_play_mode() == STOP) {

    }
    else if(get_play_mode() == LIVE_STATIC) {
        stat_timer *timerptr = const_cast<stat_timer *>(&stimer);
        timerptr->end();
        double et = timerptr->get_elapsed_time();
        
        int *idptr = const_cast<int *>(&anim_pool.at(anim_id).anim.fID);
        if(et > get_frame_time()) {
            // Increment current animation's fID.
            (*idptr) += anim_speed;
            timerptr->start();
        }

        if(*idptr >= anim_pool.at(anim_id).anim.fSequence.at("Root").fJointTR.size()) 
            *idptr = (int)anim_pool.at(anim_id).anim.fSequence.at("Root").fJointTR.size();
        transformation tr = anim_pool.at(anim_id).anim.get_valid_single_frame("Root", *idptr);
        double posmat[16];
        tr.get_matrix_transpose(posmat);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glMultMatrixd(posmat);
    }
    else {  // LIVE_DYNAMIC
        stat_timer *timerptr = const_cast<stat_timer *>(&stimer);
        timerptr->end();
        double et = timerptr->get_elapsed_time();

        
        if(et > get_frame_time()) {
            update_animation();
            timerptr->start();
        }

        transformation tr = cur_clip.anim.get_single_frame("Root", cur_clip.anim.fID);
        double posmat[16];
        tr.get_matrix_transpose(posmat);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glMultMatrixd(posmat);
    }
    
    // Render recursively from root node.
    render_node(get_root());

    if(get_play_mode() == LIVE_STATIC || get_play_mode() == LIVE_DYNAMIC) {
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
    }    
}
void live_body::render_ui() {
    ImGui::Text("Live Body configuration");

    ui_callback();
}
bool live_body::valid_state_id(int id) const noexcept {
    if(id < 0 || id >= state_pool.size())
        return false;
    return true;
}
bool live_body::valid_root_state() const noexcept {
    return valid_state_id(get_root_state_id());
}
const live_body::state& live_body::get_root_state_c() const {
    return state_pool.at(get_root_state_id());
}
int live_body::get_root_state_id() const noexcept {
    return this->root_state;
}
const live_body::clip& live_body::get_state_clip_c() const {
    return cur_clip;
}
live_body::clip& live_body::get_state_clip() {
    return cur_clip;
}
bool live_body::valid_anim_id(int id) const noexcept {
    if(id < 0 || id >= anim_pool.size())
        return false;
    return true;
}
void live_body::update_animation() const {
    // This is where next pose is decided.
    // Compare current state id and next state id, and if they are different, 
    // smoothly interpolate them in prespecified amount of steps.
    // Even if they are same, if the next [ fID ] floods, smoothly interpolate
    // between end motion and begin motion of the clip.
    if(get_cur_state_c().name == "jump" || get_cur_state_c().name == "high_jump") {
        if(cur_clip.anim.fID + 1 >= cur_clip.anim.length())
        {
            int *nid = const_cast<int *>(&next_state_id);
            *nid = get_state_id("idle");
        }
    }
    if(cur_state_id == next_state_id) {
        int *idptr = const_cast<int *>(&cur_clip.anim.fID);
        int nfid = *idptr + 1;
        if(nfid >= cur_clip.anim.length()) {
            // Since [ fID ] overflowed, update current state's clip 
            // to be aligned with the end of the current clip.
            clip nclip = clip_pool.at(get_cur_state_c().clip_id);

            // Get offset of root position and root rotation between the end motion
            // and the first motion of this state's clip.
            transformation root_pos_offset;
            transformation root_rot_offset;

            // @TODO : Hard coded root joint's name...
            transformation last_root_pos = cur_clip.anim.fSequence.at("Root").fJointTR.at(*idptr);
            transformation last_root_rot = cur_clip.anim.fSequence.at("pelvis").fJointTR.at(*idptr);
            transformation first_root_pos = nclip.anim.fSequence.at("Root").fJointTR.at(0);
            transformation first_root_rot = nclip.anim.fSequence.at("pelvis").fJointTR.at(0);

            // 1. Root rotation.
            quaternion last_root_rot_q = last_root_rot.get_rotation_quaternion();
            quaternion first_root_rot_q = first_root_rot.get_rotation_quaternion();
            // PLANE OPTIMAL POINT
            quaternion closest_q = first_root_rot_q.project_geodesic(last_root_rot_q, vec3d(0, 1, 0));
            quaternion qdiff = first_root_rot_q.inverse() * closest_q;
            vec3d angular_velocity = qdiff.log();
            double ang = angular_velocity.length() * 2.0;
            if(ang != 0.0) {
                angular_velocity /= (ang / 2.0);
                angular_velocity.set(0, 1, 0);
                root_rot_offset = transformation::rotation(vec3d(0, 0, 0), angular_velocity, ang);
            }
            
            for(auto it = nclip.anim.fSequence.at("pelvis").fJointTR.begin();
                it != nclip.anim.fSequence.at("pelvis").fJointTR.end();
                it++) {
                    (*it) = root_rot_offset * (*it);
                }

            first_root_rot_q = nclip.anim.fSequence.at("pelvis").fJointTR.at(0).get_rotation_quaternion();
            qdiff = first_root_rot_q.inverse() * last_root_rot_q;
            angular_velocity = qdiff.log();
            ang = angular_velocity.length() * 2.0;
            if(ang != 0.0) {
                angular_velocity /= (ang / 2.0);
                int valid_interp_step = (interp_step <= nclip.anim.length() ? interp_step : nclip.anim.length());
                ang /= valid_interp_step;
                for(int i = 0; i < valid_interp_step; i++) {
                    transformation &tmp = nclip.anim.fSequence.at("pelvis").fJointTR.at(i);
                    double tang = ang * (valid_interp_step - i);
                    transformation apply = transformation::rotation(vec3d(0, 0, 0), angular_velocity, tang);
                    tmp = apply * tmp;
                }
            }

            // 2. Root position.
            double last_root_pos_arr[3], first_root_pos_arr[3];
            last_root_pos.get_translation(last_root_pos_arr);
            first_root_pos.get_translation(first_root_pos_arr);

            std::vector<vec3d> pos_offset;
            for(auto it = nclip.anim.fSequence.at("Root").fJointTR.begin();
                it != nclip.anim.fSequence.at("Root").fJointTR.end();
                it++) {
                    // Get root position offset at each frame from the beginning frame.
                    double tmp_pos_arr[3];
                    it->get_translation(tmp_pos_arr);
                    pos_offset.push_back(vec3d(tmp_pos_arr[0] - first_root_pos_arr[0], tmp_pos_arr[1] - first_root_pos_arr[1], tmp_pos_arr[2] - first_root_pos_arr[2]));
                }
            double yoffset =  last_root_pos_arr[1] - first_root_pos_arr[1];
            int valid_interp_step = (interp_step <= nclip.anim.length() ? interp_step : nclip.anim.length());
            double yoffset_step = yoffset / valid_interp_step;
            int tmp_interp_step = 0;
            for(auto it = nclip.anim.fSequence.at("Root").fJointTR.begin();
                it != nclip.anim.fSequence.at("Root").fJointTR.end();
                it++) {
                    double y = first_root_pos_arr[1]; // Y invariance.
                    if(tmp_interp_step < valid_interp_step)
                        y += yoffset_step * (valid_interp_step - tmp_interp_step);
                    tmp_interp_step++;

                    vec3d beg(last_root_pos_arr[0], y, last_root_pos_arr[2]);   // Y Invariance.
                    
                    vec3d move = beg + root_rot_offset.applyR(pos_offset.at(std::distance(nclip.anim.fSequence.at("Root").fJointTR.begin(), it)));

                    *it = transformation::translation(move[0], move[1], move[2]);
                }

            // Get each joint's offset and apply...
            for(auto it = cur_clip.anim.fSequence.begin();
                it != cur_clip.anim.fSequence.end();
                it++) {
                    if(it->first == "Root" || it->first == "pelvis")
                        continue;   // Exclude roots.
                    // Get offset in the first place.
                    quaternion last_rot_q = it->second.fJointTR.at(*idptr).get_rotation_quaternion();
                    quaternion first_rot_q = nclip.anim.fSequence.at(it->first).fJointTR.at(0).get_rotation_quaternion();
                    quaternion qdiff = first_rot_q.inverse() * last_rot_q;
                    vec3d angular_velocity = qdiff.log();
                    double ang = angular_velocity.length() * 2.0;
                    if(ang != 0.0) {
                        angular_velocity /= (ang / 2.0);
                        int valid_interp_step = (interp_step <= it->second.fJointTR.size() ? interp_step : it->second.fJointTR.size());
                        ang /= valid_interp_step;
                        for(int i = 0; i < valid_interp_step; i++) {
                            transformation &tmp = nclip.anim.fSequence.at(it->first).fJointTR.at(i);
                            double tang = ang * (valid_interp_step - i);
                            transformation apply = transformation::rotation(vec3d(0, 0, 0), angular_velocity, tang);
                            tmp = apply * (tmp);
                        }
                    }
                }
            nclip.anim.fID = 0;
            clip *tmp = const_cast<clip *>(&cur_clip);
            *tmp = nclip;
        }
        else
        {
            *idptr = nfid;
        }        
    }
    else {
        int id = cur_clip.anim.fID;
        // Since [ fID ] overflowed, update current state's clip 
        // to be aligned with the end of the current clip.
        clip nclip = clip_pool.at(state_pool.at(next_state_id).clip_id);

        // Get offset of root position and root rotation between the end motion
        // and the first motion of this state's clip.
        transformation root_pos_offset;
        transformation root_rot_offset;

        // @TODO : Hard coded root joint's name...
        transformation last_root_pos = cur_clip.anim.fSequence.at("Root").fJointTR.at(id);
        transformation last_root_rot = cur_clip.anim.fSequence.at("pelvis").fJointTR.at(id);
        transformation first_root_pos = nclip.anim.fSequence.at("Root").fJointTR.at(0);
        transformation first_root_rot = nclip.anim.fSequence.at("pelvis").fJointTR.at(0);

        // 1. Root rotation.
        quaternion last_root_rot_q = last_root_rot.get_rotation_quaternion();
        quaternion first_root_rot_q = first_root_rot.get_rotation_quaternion();
        // PLANE OPTIMAL POINT
        quaternion closest_q = first_root_rot_q.project_geodesic(last_root_rot_q, vec3d(0, 1, 0));
        quaternion qdiff = first_root_rot_q.inverse() * closest_q;
        vec3d angular_velocity = qdiff.log();
        double ang = angular_velocity.length() * 2.0;
        if(ang != 0.0) {
            angular_velocity /= (ang / 2.0);
            angular_velocity.set(0, 1, 0);
            root_rot_offset = transformation::rotation(vec3d(0, 0, 0), angular_velocity, ang);
        }
        
        for(auto it = nclip.anim.fSequence.at("pelvis").fJointTR.begin();
            it != nclip.anim.fSequence.at("pelvis").fJointTR.end();
            it++) {
                (*it) = root_rot_offset * (*it);
            }

        first_root_rot_q = nclip.anim.fSequence.at("pelvis").fJointTR.at(0).get_rotation_quaternion();
        qdiff = first_root_rot_q.inverse() * last_root_rot_q;
        angular_velocity = qdiff.log();
        ang = angular_velocity.length() * 2.0;
        if(ang != 0.0) {
            angular_velocity /= (ang / 2.0);
            int valid_interp_step = (interp_step <= nclip.anim.length() ? interp_step : nclip.anim.length());
            ang /= valid_interp_step;
            for(int i = 0; i < valid_interp_step; i++) {
                transformation &tmp = nclip.anim.fSequence.at("pelvis").fJointTR.at(i);
                double tang = ang * (valid_interp_step - i);
                transformation apply = transformation::rotation(vec3d(0, 0, 0), angular_velocity, tang);
                tmp = apply * tmp;
            }
        }

        // 2. Root position.
        double last_root_pos_arr[3], first_root_pos_arr[3];
        last_root_pos.get_translation(last_root_pos_arr);
        first_root_pos.get_translation(first_root_pos_arr);

        std::vector<vec3d> pos_offset;
        for(auto it = nclip.anim.fSequence.at("Root").fJointTR.begin();
            it != nclip.anim.fSequence.at("Root").fJointTR.end();
            it++) {
                // Get root position offset at each frame from the beginning frame.
                double tmp_pos_arr[3];
                it->get_translation(tmp_pos_arr);
                pos_offset.push_back(vec3d(tmp_pos_arr[0] - first_root_pos_arr[0], tmp_pos_arr[1] - first_root_pos_arr[1], tmp_pos_arr[2] - first_root_pos_arr[2]));
            }
        double yoffset =  last_root_pos_arr[1] - first_root_pos_arr[1];
        int valid_interp_step = (interp_step <= nclip.anim.length() ? interp_step : nclip.anim.length());
        double yoffset_step = yoffset / valid_interp_step;
        int tmp_interp_step = 0;
        for(auto it = nclip.anim.fSequence.at("Root").fJointTR.begin();
            it != nclip.anim.fSequence.at("Root").fJointTR.end();
            it++) {
                double y = first_root_pos_arr[1]; // Y invariance.
                if(tmp_interp_step < valid_interp_step)
                    y += yoffset_step * (valid_interp_step - tmp_interp_step);
                tmp_interp_step++;

                vec3d beg(last_root_pos_arr[0], y, last_root_pos_arr[2]);   // Y Invariance.
                
                vec3d move = beg + root_rot_offset.applyR(pos_offset.at(std::distance(nclip.anim.fSequence.at("Root").fJointTR.begin(), it)));

                *it = transformation::translation(move[0], move[1], move[2]);
            }

        // Get each joint's offset and apply...
        for(auto it = cur_clip.anim.fSequence.begin();
            it != cur_clip.anim.fSequence.end();
            it++) {
                if(it->first == "Root" || it->first == "pelvis")
                    continue;   // Exclude roots.
                // Get offset in the first place.
                quaternion last_rot_q = it->second.fJointTR.at(id).get_rotation_quaternion();
                quaternion first_rot_q = nclip.anim.fSequence.at(it->first).fJointTR.at(0).get_rotation_quaternion();
                quaternion qdiff = first_rot_q.inverse() * last_rot_q;
                vec3d angular_velocity = qdiff.log();
                double ang = angular_velocity.length() * 2.0;
                if(ang != 0.0) {
                    angular_velocity /= (ang / 2.0);
                    int valid_interp_step = (interp_step <= it->second.fJointTR.size() ? interp_step : it->second.fJointTR.size());
                    ang /= valid_interp_step;
                    for(int i = 0; i < valid_interp_step; i++) {
                        transformation &tmp = nclip.anim.fSequence.at(it->first).fJointTR.at(i);
                        double tang = ang * (valid_interp_step - i);
                        transformation apply = transformation::rotation(vec3d(0, 0, 0), angular_velocity, tang);
                        tmp = apply * (tmp);
                    }
                }
            }
        nclip.anim.fID = 0;
        clip *tmp = const_cast<clip *>(&cur_clip);
        *tmp = nclip;
        
        int *csid = const_cast<int *>(&cur_state_id);
        *csid = next_state_id;
    }
}
void live_body::save(const std::string &path) {
    std::ofstream ofs(path);
    if(!ofs.is_open()) {
        std::cerr<<"Could not save, check path : "<<path<<std::endl;
        return;
    }
    // Save clip pool.
    for(auto it = clip_pool.begin(); it != clip_pool.end(); it++) {
        clip &c = *it;
        ofs << "C " << std::endl << c.name << std::endl;
        for(auto it2 = c.anim.fSequence.begin(); it2 != c.anim.fSequence.end(); it2++) {
            ofs<<"N " << std::endl << it2->first<<std::endl;
            ofs<<it2->second.fJointTR.size()<<std::endl;
            for(auto it3 = it2->second.fJointTR.begin(); it3 != it2->second.fJointTR.end(); it3++) {
                double m[4][4];
                it3->get(m);
                for(int i = 0; i < 4; i++)
                    for(int j = 0; j < 4; j++)
                        ofs<<m[i][j]<<std::endl;
            }
        }
        ofs<<"E"<<std::endl; // end
    }

    // save state pool.
    for(auto it = state_pool.begin(); it != state_pool.end(); it++) {
        state &s = *it;
        ofs << "S "<<std::endl<<s.name << std::endl;
        ofs<<s.clip_id<<std::endl;
    }

    ofs.close();

    std::cout<<"Save completed"<<std::endl;
}
void live_body::load(const std::string &path) {
    clip_pool.clear();
    state_pool.clear();

    std::ifstream ifs(path);
    if(!ifs.is_open()) {
        std::cerr<<"Could not load, check path : "<<path<<std::endl;
        return;
    }
    // Load clip pool.
    char str[100];
    while(!ifs.eof()) {
        ifs.getline(str, 100);
        if(str[0] == 'C') {
            // start of new clip.
            clip nclip;
            ifs.getline(str, 100);
            nclip.name = std::string(str);
            while(true) {
                ifs.getline(str, 100);
                if(str[0] == 'E')
                    break;
                ifs.getline(str, 100);
                std::string node_name = std::string(str);
                ifs.getline(str, 100);
                int size = atoi(str);

                std::vector<transformation> TRs;
                for(int i = 0; i < size; i++) {
                    double m[4][4];
                    for(int j = 0; j < 4; j++) {
                        for(int k = 0; k < 4; k++) {
                            ifs.getline(str, 100);
                            m[j][k] = atof(str);
                        }
                    }
                    transformation TR;
                    TR.set(m);
                    TRs.push_back(TR);
                }
                nclip.anim.add_frame(node_name, TRs);
            }
            clip_pool.push_back(nclip);
        }
        else if(str[0] == 'S') {
            // Start of new state.
            state ns;
            ifs.getline(str, 100);
            ns.name = std::string(str);
            ifs.getline(str, 100);
            ns.clip_id = atoi(str);
            state_pool.push_back(ns);
        }
    }
    ifs.close();

    std::cout<<"Load completed"<<std::endl;
}