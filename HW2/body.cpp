#include "body.hpp"
#include "scene/render_basic.hpp"
#include "imgui/imgui_include_freeglut.hpp"
#include <GL/freeglut.h>

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
void body::node::set_bodypart(const box &box) {
    this->bodypart = box;
}
const box& body::node::get_bodypart() const noexcept {
    return this->bodypart;
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

body::node& body::get_node(int id) {
    return tree.at(id);
}
const body::node& body::get_node_c(int id) {
    return tree.at(id);
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
void render_box(const box &box) {
    // inner
     glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
     glColor3d(0.5, 0.5, 0.5);
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
    render_box(n.get_bodypart());

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
            (*idptr)++;
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