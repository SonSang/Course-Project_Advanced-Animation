#include <iostream>
#include "GL/glew.h"
#include "GL/freeglut.h"

#include "math/math.hpp"
#include "math/vector.hpp"
#include "scene/camera.hpp"
#include "scene/mouse.hpp"
#include "scene/render_basic.hpp"
#include "scene/geometry.hpp"
#include "scene/object.hpp"
#include "scene/property_render_geometry.hpp"
#include "ui/ui.hpp"

#include "body.hpp"

#include "imgui/imgui_include_freeglut.hpp"

GLuint window_width = 800, window_height = 600;
GLuint window_pos_x = 100, window_pos_y = 100;

// main things.
static camera		    main_camera;
static mouse		    main_mouse;
static scene_manager    main_scene_man;

// basic clear color.
static float clear_color[4] = {(float)(212.0 / 255.0), (float)(244.0 / 255.0), (float)(250.0 / 255.0), 1.00f};

static void init_camera()
{
    main_camera.update_view();
}
static void add_axis()
{
    std::shared_ptr<object>
        axis_obj = std::make_shared<object>("axis");
    main_scene_man.get_object_manager().add_object(axis_obj);
    // x
    {
        line
            nln;
        nln.set_beg(0, 0, 0);
        nln.set_end(1e+10, 0, 0);

        std::shared_ptr<property_render_geometry<line>>
            prg = std::make_shared<property_render_geometry<line>>();
        prg->set_geometry(nln);
        prg->get_config().inner_color[0] = 1;
        prg->get_config().inner_color[1] = 0;
        prg->get_config().inner_color[2] = 0;
        prg->get_config().size = 1;
        prg->set_valid(true);

        main_scene_man.add_object_property(axis_obj->get_id(), prg);
    }
    // y
    {
        line
            nln;
        nln.set_beg(0, 0, 0);
        nln.set_end(0, 1e+10, 0);

        std::shared_ptr<property_render_geometry<line>>
            prg = std::make_shared<property_render_geometry<line>>();
        prg->set_geometry(nln);
        prg->get_config().inner_color[0] = 0;
        prg->get_config().inner_color[1] = 1;
        prg->get_config().inner_color[2] = 0;
        prg->get_config().size = 1;
        prg->set_valid(true);

        main_scene_man.add_object_property(axis_obj->get_id(), prg);
    }
    // z
    {
        line
            nln;
        nln.set_beg(0, 0, 0);
        nln.set_end(0, 0, 1e+10);

        std::shared_ptr<property_render_geometry<line>>
            prg = std::make_shared<property_render_geometry<line>>();
        prg->set_geometry(nln);
        prg->get_config().inner_color[0] = 0;
        prg->get_config().inner_color[1] = 0;
        prg->get_config().inner_color[2] = 1;
        prg->get_config().size = 1;
        prg->set_valid(true);

        main_scene_man.add_object_property(axis_obj->get_id(), prg);
    }
}

static void keyboard_button_callback(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'w':
    case 'W': // Forward
        main_camera.move_back_forth(1);
        break;
    case 's':
    case 'S': // Backward
        main_camera.move_back_forth(-1);
        break;
    case 'a':
    case 'A': // Left
        main_camera.move_left_right(-1);
        break;
    case 'd':
    case 'D': // Right
        main_camera.move_left_right(1);
        break;
    case 't':
    case 'T': // translate / rotate toggle.
        //drag_translate = !drag_translate;
        break;
    case (27): // ESC : End program.
        exit(0);
        break;
    default:
        break;
    }
    // ui callback.
    ui_keyboard_callback(key, x, y);
}
static void keyboard_button_up_callback(unsigned char key, int x, int y)
{
    // ui callback.
    ui_keyboard_up_callback(key, x, y);
}
static void special_button_callback(int key, int x, int y)
{
    // ui callback.
    ui_special_callback(key, x, y);
}
static void special_button_up_callback(int key, int x, int y)
{
    // ui callback.
    ui_special_up_callback(key, x, y);
}
static void mouse_button_callback(GLint button, GLint action, GLint x, GLint y)
{
    if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN) // Left button pressed
    {
        main_mouse.set_lpressed(true);
        //mouse_pick_check(x, y); // check if user has clicked on any object in the scene.
    }
    if (button == GLUT_RIGHT_BUTTON && action == GLUT_DOWN) // Right button pressed
        main_mouse.set_rpressed(true);
    if (button == GLUT_MIDDLE_BUTTON && action == GLUT_DOWN) // Middle button pressed
        main_mouse.set_mpressed(true);

    if (button == GLUT_LEFT_BUTTON && action == GLUT_UP) // Left button released
    {
        main_mouse.set_lpressed(false);
        //tr_arrow_picked = 0;
        //drag_start = true;
    }
    if (button == GLUT_RIGHT_BUTTON && action == GLUT_UP)
    { // Right button released
        main_mouse.set_rpressed(false);
        main_mouse.set_first_mouse(true);
    }
    if (button == GLUT_MIDDLE_BUTTON && action == GLUT_UP) // Middle button released
        main_mouse.set_mpressed(false);

    // // ui callback.
     ui_mouse_button_callback(button, action, x, y);
}
static void mouse_move_callback(GLint x, GLint y)
{
    if (main_mouse.get_lpressed())
    {
    }
    if (main_mouse.get_rpressed())
    {
        main_mouse.move_camera(main_camera, x, y); // mouse right button to change camera angle.
    }
    // // ui callback.
    ui_mouse_move_callback(x, y);
}
static void mouse_wheel_callback(GLint button, GLint dir, GLint x, GLint y)
{
    if (dir == 1) // zoom in
    {
        main_camera.set_mag(main_camera.get_mag() > 0.01 ? main_camera.get_mag() - 0.01 : 0.01);
        main_camera.update_proj(window_width, window_height);
    }
    else if (dir == -1) // zoom out
    {
        main_camera.set_mag(main_camera.get_mag() < 50 ? main_camera.get_mag() + 0.01 : 50);
        main_camera.update_proj(window_width, window_height);
    }

    // // ui callback.
    ui_mouse_wheel_callback(button, dir, x, y);
}
static void reshape_callback(GLint width, GLint height)
{
    window_width = width;
    window_height = height;
    glViewport(0, 0, width, height);

    main_camera.update_proj(width, height);

    // ui callback.
    ui_reshape_callback(width, height);
}

static void display_callback()
{
    ui_display_callback_beg(main_scene_man);

    glClearColor(clear_color[0], clear_color[1], clear_color[2], clear_color[3]);

    // draw canvas.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // render to screen.
    main_scene_man.render();

    // draw UI.
    ui_display_callback_end();

    glutSwapBuffers();
}

std::shared_ptr<body> bodyptr;
int base_id;
int head_id;

int left_arm_id;
int right_arm_id;

int left_thigh_id;
int right_thigh_id;
int left_leg_id;
int right_leg_id;

bool play_animation_head = false;
bool play_animation_arm = false;
bool play_animation_thigh_leg = false;

float head_rotation_axis[3] = { 1, 0, 0 };

void animation_head() {
    const static int step = 40;    
    const static double angle_step = (1.0 / step) * PI * 0.3;
    static bool increasing = true;
    static int curstep = 0;

    auto &node = bodyptr->get_node(head_id);
    auto jrptr = std::static_pointer_cast<joint_ballsocket>(node.get_joint());
    vec3d axis(head_rotation_axis[0], head_rotation_axis[1], head_rotation_axis[2]);
    if(axis.length() == 0.0)
        return;
    axis.normalize();

    if(increasing) {
        jrptr->rotate(axis, angle_step);
        curstep++;
    }
    else {
        jrptr->rotate(axis, -angle_step);
        curstep--;
    }
    if(curstep == step) 
        increasing = false;
    else if(curstep == -step)
        increasing = true;
}

void animation_arm() {
    const static int step = 40;    
    const static double angle_step = (1.0 / step) * PI * 0.3;
    static bool increasing = true;
    static int curstep = 0;

    auto &left_node = bodyptr->get_node(left_arm_id);
    auto &right_node = bodyptr->get_node(right_arm_id);
    auto left_jrptr = std::static_pointer_cast<joint_revolute>(left_node.get_joint());
    auto right_jrptr = std::static_pointer_cast<joint_revolute>(right_node.get_joint());
    if(increasing) {
        left_jrptr->rotate(angle_step);
        right_jrptr->rotate(-angle_step);
        curstep++;
    }
    else {
        left_jrptr->rotate(-angle_step);
        right_jrptr->rotate(angle_step);
        curstep--;
    }
    if(curstep == step) 
        increasing = false;
    else if(curstep == -step)
        increasing = true;
}

void animation_thigh_leg() {
    const static int step = 40;    
    const static double thigh_angle_step = (1.0 / step) * PI * 0.3;
    const static double leg_angle_step = (1.0 / step) * PI * 0.2;
    static bool increasing = true;
    static int curstep = 0;

    auto &left_thigh_node = bodyptr->get_node(left_thigh_id);
    auto &right_thigh_node = bodyptr->get_node(right_thigh_id);
    auto &left_leg_node = bodyptr->get_node(left_leg_id);
    auto &right_leg_node = bodyptr->get_node(right_leg_id);

    auto left_thigh_jrptr = std::static_pointer_cast<joint_revolute>(left_thigh_node.get_joint());
    auto right_thigh_jrptr = std::static_pointer_cast<joint_revolute>(right_thigh_node.get_joint());
    auto left_leg_jrptr = std::static_pointer_cast<joint_revolute>(left_leg_node.get_joint());
    auto right_leg_jrptr = std::static_pointer_cast<joint_revolute>(right_leg_node.get_joint());

    if(increasing) {
        left_thigh_jrptr->rotate(thigh_angle_step);
        right_thigh_jrptr->rotate(-thigh_angle_step);
        if(curstep > 0) {
            left_leg_jrptr->rotate(leg_angle_step);
            right_leg_jrptr->rotate(leg_angle_step);
        }
        else if(curstep < 0) {
            left_leg_jrptr->rotate(-leg_angle_step);
            right_leg_jrptr->rotate(-leg_angle_step);
        }
        curstep++;
    }
    else {
        left_thigh_jrptr->rotate(-thigh_angle_step);
        right_thigh_jrptr->rotate(thigh_angle_step);
        if(curstep > 0) {
            left_leg_jrptr->rotate(-leg_angle_step);
            right_leg_jrptr->rotate(-leg_angle_step);
        }
        else if (curstep < 0) {
            left_leg_jrptr->rotate(leg_angle_step);
            right_leg_jrptr->rotate(leg_angle_step);
        }
        curstep--;
    }
    if(curstep == step) 
        increasing = false;
    else if(curstep == -step)
        increasing = true;
}

static void idle_callback()
{
    if(play_animation_head) 
        animation_head();
    if(play_animation_arm) 
        animation_arm();
    if(play_animation_thigh_leg)
        animation_thigh_leg();
    glutPostRedisplay();
}

// body UI callback.
void body_ui_callback() {
    ImGui::InputFloat3("Head rotation axis", head_rotation_axis);
    if(play_animation_head) {
        if(ImGui::Button("Stop head animation"))
            play_animation_head = false;
    }
    else {
        if(ImGui::Button("Play head animation"))
            play_animation_head = true;
    }

    if(play_animation_arm) {
        if(ImGui::Button("Stop arm animation"))
            play_animation_arm = false;
    }
    else {
        if(ImGui::Button("Play arm animation"))
            play_animation_arm = true;
    }

    if(play_animation_thigh_leg) {
        if(ImGui::Button("Stop thigh-leg animation"))
            play_animation_thigh_leg = false;
    }
    else {
        if(ImGui::Button("Play thigh-leg animation"))
            play_animation_thigh_leg = true;
    }
}

int add_head(int parent, std::shared_ptr<body> bodyptr) {
    body::node headnode;
    auto linkptr = std::make_shared<link>();
    auto jointptr = std::make_shared<joint_ballsocket>();
    linkptr->translate(0, 1.6, 0);
    headnode.set_link(linkptr);
    headnode.set_joint(jointptr);

    box box_;
    box_.set_xmin(-0.3); box_.set_xmax(0.3);
    box_.set_ymin(-0.0); box_.set_ymax(0.6);
    box_.set_zmin(-0.3); box_.set_zmax(0.3);
    headnode.set_bodypart(box_);

    return bodyptr->add_node(parent, headnode);
}

int add_left_arm(int parent, std::shared_ptr<body> bodyptr) {
    body::node left_arm_node;
    auto linkptr = std::make_shared<link>();
    auto jointptr = std::make_shared<joint_revolute>();
    linkptr->translate(1.5, 1.3, 0);
    jointptr->set_axis(vec3d(1, 0, 0)); // rotate around x-axis.
    jointptr->rotate(0);
    left_arm_node.set_link(linkptr);
    left_arm_node.set_joint(jointptr);

    box box_;
    box_.set_xmin(0.3); box_.set_xmax(-0.3);
    box_.set_ymin(-2); box_.set_ymax(0.0);
    box_.set_zmin(-0.3); box_.set_zmax(0.3);
    left_arm_node.set_bodypart(box_);

    return bodyptr->add_node(parent, left_arm_node);
}

int add_right_arm(int parent, std::shared_ptr<body> bodyptr) {
    body::node right_arm_node;
    auto linkptr = std::make_shared<link>();
    auto jointptr = std::make_shared<joint_revolute>();
    linkptr->translate(-1.5, 1.3, 0);
    jointptr->set_axis(vec3d(1, 0, 0)); // rotate around x-axis.
    jointptr->rotate(0);
    right_arm_node.set_link(linkptr);
    right_arm_node.set_joint(jointptr);

    box box_;
    box_.set_xmin(-0.3); box_.set_xmax(0.3);
    box_.set_ymin(-2); box_.set_ymax(0.0);
    box_.set_zmin(-0.3); box_.set_zmax(0.3);
    right_arm_node.set_bodypart(box_);

    return bodyptr->add_node(parent, right_arm_node);
}

int add_left_thigh(int parent, std::shared_ptr<body> bodyptr) {
    body::node left_thigh_node;
    auto linkptr = std::make_shared<link>();
    auto jointptr = std::make_shared<joint_revolute>();
    linkptr->translate(0.75, -1.4, 0);
    jointptr->set_axis(vec3d(1, 0, 0)); // rotate around x-axis.
    jointptr->rotate(0);
    left_thigh_node.set_link(linkptr);
    left_thigh_node.set_joint(jointptr);

    box box_;
    box_.set_xmin(-0.3); box_.set_xmax(0.3);
    box_.set_ymin(-1); box_.set_ymax(0.0);
    box_.set_zmin(-0.3); box_.set_zmax(0.3);
    left_thigh_node.set_bodypart(box_);

    return bodyptr->add_node(parent, left_thigh_node);
}

int add_right_thigh(int parent, std::shared_ptr<body> bodyptr) {
    body::node right_thigh_node;
    auto linkptr = std::make_shared<link>();
    auto jointptr = std::make_shared<joint_revolute>();
    linkptr->translate(-0.75, -1.4, 0);
    jointptr->set_axis(vec3d(1, 0, 0)); // rotate around x-axis.
    jointptr->rotate(0);
    right_thigh_node.set_link(linkptr);
    right_thigh_node.set_joint(jointptr);

    box box_;
    box_.set_xmin(-0.3); box_.set_xmax(0.3);
    box_.set_ymin(-1); box_.set_ymax(0.0);
    box_.set_zmin(-0.3); box_.set_zmax(0.3);
    right_thigh_node.set_bodypart(box_);

    return bodyptr->add_node(parent, right_thigh_node);
}

int add_left_leg(int parent, std::shared_ptr<body> bodyptr) {
    body::node left_leg_node;
    auto linkptr = std::make_shared<link>();
    auto jointptr = std::make_shared<joint_revolute>();
    linkptr->translate(0, -1.3, 0);
    jointptr->set_axis(vec3d(1, 0, 0)); // rotate around x-axis.
    jointptr->rotate(0);
    left_leg_node.set_link(linkptr);
    left_leg_node.set_joint(jointptr);

    box box_;
    box_.set_xmin(-0.3); box_.set_xmax(0.3);
    box_.set_ymin(-1.3); box_.set_ymax(0.0);
    box_.set_zmin(-0.3); box_.set_zmax(0.3);
    left_leg_node.set_bodypart(box_);

    return bodyptr->add_node(parent, left_leg_node);
}

int add_right_leg(int parent, std::shared_ptr<body> bodyptr) {
    body::node right_leg_node;
    auto linkptr = std::make_shared<link>();
    auto jointptr = std::make_shared<joint_revolute>();
    linkptr->translate(0, -1.3, 0);
    jointptr->set_axis(vec3d(1, 0, 0)); // rotate around x-axis.
    jointptr->rotate(0);
    right_leg_node.set_link(linkptr);
    right_leg_node.set_joint(jointptr);

    box box_;
    box_.set_xmin(-0.3); box_.set_xmax(0.3);
    box_.set_ymin(-1.3); box_.set_ymax(0.0);
    box_.set_zmin(-0.3); box_.set_zmax(0.3);
    right_leg_node.set_bodypart(box_);

    return bodyptr->add_node(parent, right_leg_node);
}

static void compute_callback() {
    // create a body.
    std::shared_ptr<object>
        bodyobj = std::make_shared<object>("body");
    bodyptr = std::make_shared<body>();
    main_scene_man.get_object_manager().add_object(bodyobj);
    main_scene_man.add_object_property(bodyobj->get_id(), bodyptr);

    // 0. register ui callback.
    bodyptr->set_ui_callback(body_ui_callback);

    // 1. create base.
    box base;
    base.set_xmax(1); base.set_xmin(-1);
    base.set_ymax(1.3); base.set_ymin(-1.3);
    base.set_zmax(0.3); base.set_zmin(-0.3);
    bodyptr->get_node(bodyptr->get_root()).set_bodypart(base);
    base_id = bodyptr->get_root();

    // 2. create head.
    head_id = add_head(base_id, bodyptr);

    // 3. create arms.
    left_arm_id = add_left_arm(base_id, bodyptr);
    right_arm_id = add_right_arm(base_id, bodyptr);

    // 4. create thighs.
    left_thigh_id = add_left_thigh(base_id, bodyptr);
    right_thigh_id = add_right_thigh(base_id, bodyptr);

    // 5. create legs.
    left_leg_id = add_left_leg(left_thigh_id, bodyptr);
    right_leg_id = add_right_leg(right_thigh_id, bodyptr);
}

int main(int argc, char **argv) {
    // initialize glut.
    glutInit(&argc, argv);

    // let program continue after main loop teminates.
#ifdef __FREEGLUT_EXT_H__
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
#endif
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutInitWindowSize(window_width, window_height);
    glutInitWindowPosition(window_pos_x, window_pos_y);
    glutCreateWindow("Renderer");

    // initialize glew.
    if (glewInit() != GLEW_OK)
    {
        std::cout << "GLEW failed : " << glewGetErrorString(glewInit()) << std::endl;
        exit(-1);
    }

    // callback registration.
    glutDisplayFunc(display_callback);
    glutKeyboardFunc(keyboard_button_callback);
    glutKeyboardUpFunc(keyboard_button_up_callback);
    glutSpecialFunc(special_button_callback);
    glutSpecialUpFunc(special_button_up_callback);
    glutMouseFunc(mouse_button_callback);
    glutMotionFunc(mouse_move_callback);
    glutPassiveMotionFunc(mouse_move_callback);
#ifdef __FREEGLUT_EXT_H__
    glutMouseWheelFunc(mouse_wheel_callback);
#endif
    glutReshapeFunc(reshape_callback);
    glutIdleFunc(idle_callback);

    // == initialize UI(imgui).
    ui_beg();

    // rendering settings.
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.0f, 1.0f);

    // initialize camera settings.
    init_camera();

    //init_mouse_pick();
    // initialize ui settings.
    //init_ui_settings();
    //add_axis();

    // call compute.
    compute_callback();

    glutMainLoop();

    // clean up imgui.
    ui_end();

    return 0;
}