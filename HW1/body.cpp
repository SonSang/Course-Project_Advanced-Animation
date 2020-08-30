#include "body.hpp"
#include "scene/render_basic.hpp"
#include "imgui/imgui_include_freeglut.hpp"
#include <GL/freeglut.h>

// node
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
    render_node(get_root());
}
void body::render_ui() {
    ImGui::Text("Body configuration");

    ui_callback();
}