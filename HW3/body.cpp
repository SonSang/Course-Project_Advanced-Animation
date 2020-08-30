#include "body.hpp"
#include "scene/render_basic.hpp"
#include "imgui/imgui_include_freeglut.hpp"
#include "numerics/frprmn.hpp"
#include <GL/freeglut.h>
#include <iostream>
#include <algorithm>

#include "eigen/Eigen/Dense"

// node
void body::node::set_name(const std::string &name) {
    this->name = name;
}
std::string body::node::get_name() const noexcept {
    return this->name;
}
void body::node::set_link(const std::shared_ptr<link> linkptr) {
    this->linkptr = linkptr;
}
void body::node::set_joint(const std::shared_ptr<joint> jointptr) {
    this->jointptr = jointptr;
}
std::shared_ptr<link> body::node::get_link() const noexcept {
    return this->linkptr;
}
std::shared_ptr<joint> body::node::get_joint() const noexcept {
    return this->jointptr;
}
void body::node::set_joint_type(int type) noexcept {
    this->joint_type = type;
}
int body::node::get_joint_type() const noexcept {
    return this->joint_type;
}
void body::node::set_bodypart(const box &box) {
    this->bodypart = box;
}
const box& body::node::get_bodypart() const noexcept {
    return this->bodypart;
}
void body::node::set_parent(int id) {
    this->parent = id;
}
int body::node::get_parent() const noexcept {
    return this->parent;
}
void body::node::add_child(int id) {
    children.insert(id);
}
void body::node::del_child(int id) {
    children.erase(id);
}
void body::node::set_children(const std::set<int> &children) noexcept {
    this->children = children;
}
const std::set<int>& body::node::get_childern() const noexcept {
    return this->children;
}
std::vector<transformation>& body::node::get_anim() noexcept {
    return this->anim;
}
const std::vector<transformation>& body::node::get_anim_c() const noexcept {
    return this->anim;
}
void body::node::set_anim(const std::vector<transformation> &anim) noexcept {
    this->anim = anim;
}
bool body::node::valid_anim_id(int id) const noexcept {
    return (id >= 0) && (id < anim.size());
}
void body::node::validate_anim_id(int &id) const noexcept {
    if (id < 0)
        id = 0;
    else if (id >= anim.size())
        id = anim.size() - 1;
}

// body
body::body() {
    // make root.
    node rootnode;
    auto linkptr = std::make_shared<link>();
    auto jointptr = std::make_shared<joint>();
    auto bodypart = box();
    rootnode.set_link(linkptr);
    rootnode.set_joint(jointptr);
    rootnode.set_bodypart(bodypart);
    tree.insert({node_counter++, rootnode});
    root = node_counter - 1;
}

int body::get_node_id(const std::string &name) const noexcept {
    for(auto it = tree.begin(); it != tree.end(); it++) {
        if(it->second.get_name() == name) 
            return it->first;
    }
    return -1;
}

body::node& body::get_node(int id) {
    return tree.at(id);
}
const body::node& body::get_node_c(int id) {
    return tree.at(id);
}
body::node* body::get_node(const std::string &name) {
    for(auto it = tree.begin(); it != tree.end(); it++) {
        if(it->second.get_name() == name) {
            return &it->second;
        }
    }
    return nullptr;
}
const body::node* body::get_node_c(const std::string &name) const {
    for(auto it = tree.begin(); it != tree.end(); it++) {
        if(it->second.get_name() == name) {
            return &it->second;
        }
    }
    return nullptr;
}
int body::get_size() const noexcept {
    return (int)tree.size();
}
int body::add_node(int parent, const node &node_)
{
    int id = node_counter++;
    tree.insert({id, node_});
    tree.at(parent).add_child(id);
    return id;
}
int body::get_root() const noexcept {
    return root;
}
void body::set_root(int id) {
    this->root = id;
}
std::map<int, body::node>& body::get_tree() noexcept {
    return this->tree;
}
const std::map<int, body::node>& body::get_tree_c() const noexcept {
    return this->tree;
}
void body::set_play_anim(bool b) noexcept {
    this->play_anim = b;
}
bool body::get_play_anim() const noexcept {
    return this->play_anim;
}
void body::set_cur_anim_id(int id) {
    this->cur_anim_id = id;
}
int body::get_cur_anim_id() const noexcept {
    return this->cur_anim_id;
}
void body::set_anim_speed(int speed) {
    this->anim_speed = speed;
}
int body::get_anim_speed() const noexcept {
    return this->anim_speed;
}
void body::set_frame_time(double ft) {
    this->frame_time = ft;
}
double body::get_frame_time() const noexcept {
    return this->frame_time;
}
std::vector<vec3d>& body::get_anim_pos() noexcept {
    return this->anim_pos;
}
const std::vector<vec3d>& body::get_anim_pos_c() const noexcept {
    return this->anim_pos;
}
void body::set_anim_pos(const std::vector<vec3d> &anim_pos) noexcept {
    this->anim_pos = anim_pos;
}
std::vector<int> body::get_node_path(int node) const {
    if(node >= tree.size()) {
        std::cerr<<"Invalid node id for getting node path : node id is larger than tree size."<<std::endl;
        return std::vector<int>();  // error.
    }
    std::vector<int> inverse;
    int curnode = node;
    inverse.push_back(node);

    while(curnode != get_root()) {
        int parent_node = tree.find(curnode)->second.get_parent();
        inverse.push_back(parent_node);
        curnode = parent_node;
    }

    std::vector<int> ret;
    for(auto it = inverse.rbegin(); it != inverse.rend(); it++) {
        ret.push_back(*it);
    }
    return ret;
}
transformation body::get_global_transform(const std::vector<int> path) const {
    transformation ret;
    for(auto it = path.rbegin(); it != path.rend(); it++) {
        ret *= tree.at(*it).get_joint()->getTR();
        ret *= tree.at(*it).get_link()->getTR();
    }
    return ret;
}
transformation body::get_global_transform(int node) const {
    std::vector<int> path = get_node_path(node);
    return get_global_transform(path);
}
void render_box(const box &box, double r, double g, double b) {
    // inner
     glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
     glColor3d(r, g, b);
     for (int i = 0; i < 6; i++)
     {
         quad
             quad_ = box.get_face(i);
         glBegin(GL_QUADS);
         gl_draw_vertex(quad_.get_vertices().at(0));
         gl_draw_vertex(quad_.get_vertices().at(1));
         gl_draw_vertex(quad_.get_vertices().at(2));
         gl_draw_vertex(quad_.get_vertices().at(3));
         glEnd();
     }     

     // border
     glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
     glLineWidth(5);
     glColor3d(0, 0, 0);
     for (int i = 0; i < 6; i++)
     {
         quad
             quad_ = box.get_face(i);
         glBegin(GL_QUADS);
         gl_draw_vertex(quad_.get_vertices().at(0));
         gl_draw_vertex(quad_.get_vertices().at(1));
         gl_draw_vertex(quad_.get_vertices().at(2));
         gl_draw_vertex(quad_.get_vertices().at(3));
         glEnd();
     }
}
void body::set_ui_callback(void (*ui_callback)()) {
    this->ui_callback = ui_callback;
}
void body::render_node(int id, const transformation &tr, double r, double g, double b) const noexcept {
    const node& n = tree.at(id);

    double mat[16];
    tr.get_matrix_transpose(mat);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glMultMatrixd(mat);

    // render this node.
    render_box(n.get_bodypart(), r, g, b);

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}
void body::render_node(int id) const noexcept
{
    const node& n = tree.at(id);

    // apply transformation. Link -> Joint.
    double linkmat[16];
    double jointmat[16];
    
    n.get_link()->getTR().get_matrix_transpose(linkmat);
    if(get_play_anim()) {   // if playing animation
        int id = get_cur_anim_id();
        n.validate_anim_id(id);
        n.get_anim_c().at(id).get_matrix_transpose(jointmat);
    }
    else 
        n.get_joint()->getTR().get_matrix_transpose(jointmat);
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
void body::render() const noexcept {
    if(get_play_anim()) {
        stat_timer *timerptr = const_cast<stat_timer *>(&stimer);
        timerptr->end();
        if(timerptr->get_elapsed_time() > get_frame_time()) {
            // increment [cur_anim_id].
            int *idptr = const_cast<int *>(&cur_anim_id);
            (*idptr) += anim_speed;
            tree.at(get_root()).validate_anim_id(*idptr);
            timerptr->start();
        }
        
        double posmat[16];
        vec3d curpos = anim_pos.at(cur_anim_id);
        transformation tr;
        tr.translate(curpos);
        tr.get_matrix_transpose(posmat);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glMultMatrixd(posmat);
    }
    render_node(get_root());

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}
void body::render_ui() {
    ImGui::Text("Body configuration");

    ui_callback();
}
void body::inverse_kinematics(int target_id, const transformation &target_tr, double deltaT, int anim_size) {
    // This algorithm uses simple Euler intergration method.
    vec3d linear_velocity;      // linear velocity that should be accomplished in certain time step.
    vec3d angular_velocity;     // angular velocity that should be accomplished in certain time step.

    std::map<int, transformation> originalTR;   // original joint transformation.
    for(auto it = tree.begin(); it != tree.end(); it++) 
        originalTR[it->first] = it->second.get_joint()->getTR();

    // initialize animation data.
    get_anim_pos().clear();
    for(auto it = tree.begin(); it != tree.end(); it++) 
        it->second.get_anim().clear();

    std::vector<int> path = get_node_path(target_id);

    for(int iter = 0; iter < anim_size; iter++) {
        // 1. Get [ linear_velocity ] and [ angular_velocity ].
        transformation curTR = get_global_transform(target_id);

        // get [ linear_velocity ] from translation offset.
        double curTranslation[3];
        double tarTranslation[3];
        curTR.get_translation(curTranslation);
        target_tr.get_translation(tarTranslation);

        linear_velocity.set(
            tarTranslation[0] - curTranslation[0], 
            tarTranslation[1] - curTranslation[1], 
            tarTranslation[2] - curTranslation[2]); 
        linear_velocity *= deltaT;

        // get [ angular_velocity ] from rotation ( quaternion ) offset.
        quaternion curOrientation;
        quaternion tarOrientation;
        curOrientation = curTR.get_rotation_quaternion();
        tarOrientation = target_tr.get_rotation_quaternion();

        quaternion qoffset = curOrientation.inverse() * tarOrientation;
        angular_velocity = qoffset.log();
        angular_velocity *= deltaT;

        // std::cout<<"Linear velocity : "<<linear_velocity<<std::endl;
        // std::cout<<iter<<" th"<<std::endl;
        // std::cout<<"Target orientation : "<<tarOrientation<<", "<<tarOrientation[3]<<std::endl;
        // std::cout<<"Current orientation : "<<curOrientation<<", "<<curOrientation[3]<<std::endl;
        // std::cout<<"Quaternion offset : "<<qoffset<<", "<<qoffset[3]<<std::endl;
        // vec3d tav = angular_velocity;
        // tav.normalize();
        // std::cout<< "Angular velocity : "<<tav<<std::endl;
        // std::cout<<"Angular speed : "<<angular_velocity.length()<<std::endl;

        // 2. Get each joint's movement in this frame by using Jacobian matrix.
        double **J;
        int row, col;
        jacobian(target_id, &J, row, col);

        // use Eigen to solve least square problem.
        // @TODO : root does not move.
        get_anim_pos().push_back(vec3d(0, 0, 0));
        bool first = true;
        for(auto it = path.rbegin(); it != path.rend(); it++) {
            int id = *it;

            double xr, yr, zr;

            Eigen::MatrixXd eJ(6, 3);
            Eigen::VectorXd eV(6);

            for(int i = 0; i < 6; i++)
                for(int j = 0; j < 3; j++)
                    eJ(i, j) = J[i][3 * id + j];
            
            for(int i = 0; i < 3; i++) {
                eV(i) = linear_velocity[i];
                eV(i + 3) = angular_velocity[i];
            }

            // if(first) {
            //     std::cout<<"Current quaternion : "<<curOrientation<<" "<<curOrientation[3]<<std::endl;
            //     std::cout<<"Target quaternion : "<<tarOrientation<<" "<<tarOrientation[3]<<std::endl;
            //     std::cout<<"Q offset : "<<qoffset<<" "<<qoffset[3]<<std::endl;
            //     std::cout<<"Angular velocity : "<<angular_velocity<<std::endl;
            // }
                

            Eigen::VectorXd movement = eJ.bdcSvd(Eigen::ComputeThinU | Eigen::ComputeThinV).solve(eV);
            Eigen::VectorXd result = eJ * movement;

            linear_velocity -= vec3d(result(0), result(1), result(2));
            angular_velocity -= vec3d(result(3), result(4), result(5));

            xr = movement(0);
            yr = movement(1);
            zr = movement(2);

            // if(first) 
            //     std::cout<<xr<<", "<<yr<<", "<<zr<<std::endl<<std::endl;
            first = false;

            transformation
                resultTR = get_node(id).get_joint()->getTR() * 
                    transformation::rotation(vec3d(0, 0, 0), vec3d(1, 0, 0), xr) *
                    transformation::rotation(vec3d(0, 0, 0), vec3d(0, 1, 0), yr) * 
                    transformation::rotation(vec3d(0, 0, 0), vec3d(0, 0, 1), zr);

            get_node(id).get_anim().push_back(resultTR);
            get_node(id).get_joint()->setTR(resultTR);
        }

        for(auto it = tree.begin(); it != tree.end(); it++) {
            int id = it->first;
            auto find = std::find(path.begin(), path.end(), id);
            if(find != path.end())
                continue;
            get_node(id).get_anim().push_back(transformation());
        }

    //     Eigen::MatrixXd eJ(row, col);
    //     for(int i = 0; i < row; i++) 
    //         for(int j = 0; j < col; j++)
    //             eJ(i, j) = J[i][j];

    //     Eigen::VectorXd eV(6);
    //     for(int i = 0; i < 3; i++) {
    //         eV(i) = linear_velocity[i];
    //         eV(i + 3) = angular_velocity[i];
    //     }

    //     Eigen::VectorXd movement = eJ.bdcSvd(Eigen::ComputeThinU | Eigen::ComputeThinV).solve(eV);
    //     // Eigen::VectorXd test = eJ * movement;
    //     // for(int i = 0; i < test.size(); i++) {
    //     //     double d = test(i);
    //     //     std::cout<<d<<std::endl;
    //     // }
    //     // double movement_sum = 0;
    //     // for(int i = 0; i < movement.size(); i++) {
    //     //     movement_sum += movement(i) * movement(i);
    //     // }
    //     // movement_sum = sqrt(movement_sum);
    //     // std::cout<<"Sum : "<<movement_sum<<std::endl;

    //     // std::cout<<iter<<" th iteration"<<std::endl;
    //     // for(int i = 0; i < movement.size(); i++) {
    //     //     if(i % 3 == 0) {
    //     //         std::cout<<"Node "<< tree.at(i / 3).get_name() <<std::endl;
    //     //         std::cout<<"X : "<<movement(i)<<" ";
    //     //     }
    //     //     else if(i % 3 == 1) {
    //     //         std::cout<<"Y : "<<movement(i)<<" ";
    //     //     }
    //     //     else {
    //     //         std::cout<<"Z : "<<movement(i)<<" "<<std::endl;
    //     //     }
    //     // }
    //     // std::cout<<std::endl;
        
    //     // 3. apply result to animation data.

    //     // @TODO : root does not move.
    //     get_anim_pos().push_back(vec3d(0, 0, 0));

    //     int colcnt = 0;
    //     for(auto it = tree.begin(); it != tree.end(); it++) {
    //         int id = it->first;

    //         if(id == get_root()) {
    //             // root does not move.
    //             it->second.get_anim().push_back(transformation());
    //             if(it->second.get_joint_type() == 0)
    //                 colcnt++;
    //             else
    //                 colcnt += 3;
    //             continue;
    //         }

    //         bool is_path = (std::find(path.begin(), path.end(), id) != path.end());

    //         if(!is_path) {
    //             it->second.get_anim().push_back(transformation());
    //             if(it->second.get_joint_type() == 0)
    //                 colcnt++;
    //             else
    //                 colcnt += 3;
    //             continue;
    //         }

    //         if(it->second.get_joint_type() == 0) {
    //             double
    //                 r = movement(colcnt++);

    //             vec3d
    //                 axis = std::dynamic_pointer_cast<joint_revolute>(it->second.get_joint())->get_axis();

    //             transformation
    //                 result = get_node(id).get_joint()->getTR() * 
    //                     transformation::rotation(vec3d(0, 0, 0), axis, r);
            
    //             it->second.get_anim().push_back(result);
                
    //             get_node(id).get_joint()->setTR(result);
    //         }
    //         else {
    //             double 
    //                 xr = movement(colcnt++),
    //                 yr = movement(colcnt++),
    //                 zr = movement(colcnt++);

    //             // @ Node weight.
    //             // double
    //             //     weight;     // give higher weight to the nodes that are close to target node.
    //             // int
    //             //     path_index = -1;
    //             // for(int i = 0; i < path.size(); i++) {
    //             //     if(path.at(i) == id)
    //             //         path_index = i;
    //             // }
    //             // if(path_index == -1)
    //             //     weight = 0.0;
    //             // else {
    //             //     int path_offset = path.size() - path_index - 1;
    //             //     weight = pow(0.5, path_offset);
    //             // }

    //             // xr *= weight;
    //             // yr *= weight;
    //             // zr *= weight;

    //             transformation
    //                 result = get_node(id).get_joint()->getTR() * 
    //                     transformation::rotation(vec3d(0, 0, 0), vec3d(1, 0, 0), xr) *
    //                     transformation::rotation(vec3d(0, 0, 0), vec3d(0, 1, 0), yr) * 
    //                     transformation::rotation(vec3d(0, 0, 0), vec3d(0, 0, 1), zr);
            
    //             it->second.get_anim().push_back(result);
                
    //             get_node(id).get_joint()->setTR(result);
    //         }
    //     }
    }

    // restore original joint transformation
    for(auto it = originalTR.begin(); it != originalTR.end(); it++) {
        get_node(it->first).get_joint()->setTR(it->second);
    }
}
void body::inverse_kinematics(const std::vector<int> &target_id, const std::vector<transformation> &target_tr, double deltaT, int anim_size, bool root_fix, double root_weight) {
    // This algorithm uses simple Euler intergration method.
    int size = (int)target_id.size();
    std::vector<vec3d> linear_velocity;      // linear velocity that should be accomplished in certain time step.
    std::vector<vec3d> angular_velocity;     // angular velocity that should be accomplished in certain time step.

    std::map<int, transformation> originalTR;   // original joint transformation.
    transformation originalRootLink = get_node(get_root()).get_link()->getTR();
    for(auto it = tree.begin(); it != tree.end(); it++) 
        originalTR[it->first] = it->second.get_joint()->getTR();

    // initialize animation data.
    get_anim_pos().clear();
    for(auto it = tree.begin(); it != tree.end(); it++) 
        it->second.get_anim().clear();

    std::vector<std::vector<int>> paths;
    for(int i = 0; i < size; i++) {
        paths.push_back(get_node_path(target_id.at(i)));
    }

    double minError = 1e+10;
    int minErrorIndex = -1;
    for(int iter = 0; iter < anim_size; iter++) {
        // 1. Get [ linear_velocity ] and [ angular_velocity ].
        double error = 0;
        std::vector<transformation> curTRs;
        for(int i = 0; i < size; i++)
            curTRs.push_back(get_global_transform(target_id.at(i)));

        // get [ linear_velocity ] from translation offset.
        for(int i = 0; i < size; i++) {
            double curTranslation[3];
            double tarTranslation[3];
            curTRs.at(i).get_translation(curTranslation);
            target_tr.at(i).get_translation(tarTranslation);

            vec3d lv;
            lv.set(
                tarTranslation[0] - curTranslation[0], 
                tarTranslation[1] - curTranslation[1], 
                tarTranslation[2] - curTranslation[2]);
            error += lv.length();
            lv *= deltaT;
            linear_velocity.push_back(lv);
        }      

        // get [ angular_velocity ] from rotation ( quaternion ) offset.
        for(int i = 0; i < size; i++) {
            quaternion curOrientation;
            quaternion tarOrientation;
            curOrientation = curTRs.at(i).get_rotation_quaternion();
            tarOrientation = target_tr.at(i).get_rotation_quaternion();

            if(tarOrientation[3] * curOrientation[3] < 0) {
                // since there is 2-to-1 correspondence between quaternion and orientation, align two quaternions if needed.
                tarOrientation *= -1;
            }

            quaternion qoffset = curOrientation.inverse() * tarOrientation;
            vec3d av;
            av = qoffset.log();
            error += av.length();
            av *= deltaT;
            angular_velocity.push_back(av);
        }

        if(error < minError) {
            minError = error;
            minErrorIndex = iter;
        }

        // 2. Get each joint's movement in this frame by using Jacobian matrix.
        double **J;
        int row, col;
        jacobian(target_id, &J, row, col, root_fix, root_weight);

        Eigen::MatrixXd eJ(row, col);
        for(int i = 0; i < row; i++) 
            for(int j = 0; j < col; j++)
                eJ(i, j) = J[i][j];

        Eigen::VectorXd eV(row);
        for(int i = 0; i < size; i++) {
            for(int j = 0; j < 3; j++) {
                eV(i * 6 + j) = linear_velocity.at(i)[j];
                eV(i * 6 + 3 + j) = angular_velocity.at(i)[j];
            }
        }

        linear_velocity.clear();
        angular_velocity.clear();

        Eigen::VectorXd movement = eJ.bdcSvd(Eigen::ComputeThinU | Eigen::ComputeThinV).solve(eV);

        // 3. apply result to animation data.
        int colcnt = 0;
        for(auto it = tree.begin(); it != tree.end(); it++) {
            int id = it->first;

            if(id == get_root()) {
                // apply translation first.
                double xt, yt, zt;
                xt = movement(colcnt++);
                yt = movement(colcnt++);
                zt = movement(colcnt++);
                vec3d rt(xt, yt, zt);
                rt *= root_weight;
                get_node(id).get_link()->translate(rt[0], rt[1], rt[2]);
                
                double pos[3];
                get_node(id).get_link()->getTR().get_translation(pos);
                get_anim_pos().push_back(vec3d(pos[0], pos[1], pos[2]));
            }

            if(it->second.get_joint_type() == 0) {
                double
                    r = movement(colcnt++);

                vec3d
                    axis = std::dynamic_pointer_cast<joint_revolute>(it->second.get_joint())->get_axis();

                transformation
                    result = get_node(id).get_joint()->getTR() * 
                        transformation::rotation(vec3d(0, 0, 0), axis, r);
            
                it->second.get_anim().push_back(result);
                
                get_node(id).get_joint()->setTR(result);
            }
            else {
                double 
                    xr = movement(colcnt++),
                    yr = movement(colcnt++),
                    zr = movement(colcnt++);
                transformation
                    result = get_node(id).get_joint()->getTR() * 
                        transformation::rotation(vec3d(0, 0, 0), vec3d(1, 0, 0), xr) *
                        transformation::rotation(vec3d(0, 0, 0), vec3d(0, 1, 0), yr) * 
                        transformation::rotation(vec3d(0, 0, 0), vec3d(0, 0, 1), zr);
            
                it->second.get_anim().push_back(result);
                
                get_node(id).get_joint()->setTR(result);
            }
        }
    }

    // set last animation to error minimized one.
    if(minErrorIndex > 0) {
        get_anim_pos().erase(get_anim_pos().begin() + minErrorIndex, get_anim_pos().end());
        for(auto it = tree.begin(); it != tree.end(); it++) {
            it->second.get_anim().erase(it->second.get_anim().begin() + minErrorIndex, it->second.get_anim().end());
        }
    } 

    // restore original joint transformation
    get_node(get_root()).get_link()->setTR(originalRootLink);
    for(auto it = originalTR.begin(); it != originalTR.end(); it++) {
        get_node(it->first).get_joint()->setTR(it->second);
    }
}
void body::jacobian(int target_id, double ***J, int &row, int &col, bool root_fix, double root_weight) const {
    int tree_size = (int)tree.size();
    std::vector<int> path = get_node_path(target_id);
    
    transformation targetGlobalTR = get_global_transform(target_id);
    double target_translation[3];
    targetGlobalTR.get_translation(target_translation);

    row = 6;    // 3 for linear velocity, 3 for angular velocity.
    col = 0;
    for(auto it = tree.begin(); it != tree.end(); it++) {
        if(it->first == get_root()) 
            // root has translation and rotation.
            col += 3;   // translation
        if(it->second.get_joint_type() == 0)    // revolute joint has 1 DOF.
            col += 1;
        else                                    // ball-and-socket joint has 3 DOF.
            col += 3;
    }

    (*J) = new double*[row];     
    for(int i = 0; i < 6; i++)
        (*J)[i] = new double[col];         

    int colcnt = 0;
    for(auto it = tree.begin(); it != tree.end(); it++) {
        int id = it->first;
        bool is_path = (std::find(path.begin(), path.end(), id) != path.end());
        int dof;

        if(it->second.get_joint_type() == 0)
            dof = 1;
        else
            dof = 3;

        if(it->first == get_root())
            dof += 3;   // for root translation.

        if(!is_path) {
            // if this node is not on the path, it cannot affect velocity of [ target_id ] node.
            for(int i = 0; i < dof; i++) {
                for(int j = 0; j < 6; j++) 
                    (*J)[j][colcnt] = 0.0;
                colcnt++;
            }
        }
        else {
            if(it->first == get_root()) {
                // assign translation terms first.
                if(root_fix) {
                    // root does not move.
                    for(int i = 0; i < dof; i++) {
                        for(int j = 0; j < 6; j++) 
                            (*J)[j][colcnt] = 0.0;
                        colcnt++;
                    }
                    continue;
                }
                else {
                    // root translation is directly related to global x, y, z translation.
                    // x
                    (*J)[0][colcnt] = root_weight; (*J)[1][colcnt] = 0; (*J)[2][colcnt] = 0;
                    (*J)[3][colcnt] = 0; (*J)[4][colcnt] = 0; (*J)[5][colcnt] = 0;
                    colcnt++;
                    // y
                    (*J)[0][colcnt] = 0; (*J)[1][colcnt] = root_weight; (*J)[2][colcnt] = 0;
                    (*J)[3][colcnt] = 0; (*J)[4][colcnt] = 0; (*J)[5][colcnt] = 0;
                    colcnt++;
                    // z
                    (*J)[0][colcnt] = 0; (*J)[1][colcnt] = 0; (*J)[2][colcnt] = root_weight;
                    (*J)[3][colcnt] = 0; (*J)[4][colcnt] = 0; (*J)[5][colcnt] = 0;
                    colcnt++;
                }
            }
            transformation globalTR = get_global_transform(id);

            double global_translation[3];
            globalTR.get_translation(global_translation);
            vec3d offset(
                target_translation[0] - global_translation[0], 
                target_translation[1] - global_translation[1], 
                target_translation[2] - global_translation[2]);

            if(dof == 1) {
                vec3d axis = std::dynamic_pointer_cast<joint_revolute>(it->second.get_joint())->get_axis();
                // angular velocity.
                vec3d angular_velocity = globalTR.applyR(axis);

                // linear velocity.    
                vec3d linear_velocity = angular_velocity.cross(offset);

                for(int i = 0; i < 3; i++) {
                    (*J)[i][colcnt] = linear_velocity[i];
                    (*J)[i + 3][colcnt] = angular_velocity[i];
                }
                colcnt++;
            }
            else {
                // when rotating with x axis.
                {
                    // angular velocity.
                    vec3d angular_velocity = globalTR.applyR(vec3d(1, 0, 0));

                    // linear velocity.    
                    vec3d linear_velocity = angular_velocity.cross(offset);

                    for(int i = 0; i < 3; i++) {
                        (*J)[i][colcnt] = linear_velocity[i];
                        (*J)[i + 3][colcnt] = angular_velocity[i];
                    }
                    colcnt++;
                }

                // when rotating with y axis.
                {
                    // angular velocity.
                    vec3d angular_velocity = globalTR.applyR(vec3d(0, 1, 0));

                    // linear velocity.    
                    vec3d linear_velocity = angular_velocity.cross(offset);

                    for(int i = 0; i < 3; i++) {
                        (*J)[i][colcnt] = linear_velocity[i];
                        (*J)[i + 3][colcnt] = angular_velocity[i];
                    }
                    colcnt++;
                }

                // when rotating with z axis.
                {
                    // angular velocity.
                    vec3d angular_velocity = globalTR.applyR(vec3d(0, 0, 1));

                    // linear velocity.    
                    vec3d linear_velocity = angular_velocity.cross(offset);

                    for(int i = 0; i < 3; i++) {
                        (*J)[i][colcnt] = linear_velocity[i];
                        (*J)[i + 3][colcnt] = angular_velocity[i];
                    }
                    colcnt++;
                }
            }            
        }
    }
}
void body::jacobian(const std::vector<int> &target_id, double ***J, int &row, int &col, bool root_fix, double root_weight) const {
    int size = (int)target_id.size();

    row = 6 * size;

    (*J) = new double*[row];     

    for(int i = 0; i < size; i++) {
        double **tJ;
        int trow, tcol;
        jacobian(target_id.at(i), &tJ, trow, tcol, root_fix, root_weight);

        if(i == 0) {
            for(int j = 0; j < row; j++) 
                (*J)[j] = new double[tcol];
            col = tcol;
        }

        for(int j = 0; j < trow; j++) {
            for(int k = 0; k < tcol; k++) {
                (*J)[6 * i + j][k] = tJ[j][k];
            }
        }
    }
}