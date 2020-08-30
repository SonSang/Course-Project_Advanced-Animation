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
#include "bvh_parser.hpp"

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

std::shared_ptr<object> bodyobj;
std::shared_ptr<body> bodyptr;
std::shared_ptr<body> bodyptr001;
std::shared_ptr<body> bodyptr002;
std::shared_ptr<body> bodyptr004;

static void idle_callback()
{
    glutPostRedisplay();
}

void body_node_ui_callback(body::node &bn, int id) {
    const char *name = bn.get_name().c_str();
    ImGui::TextUnformatted(name);
    float x[2], y[2], z[2];
    x[0] = (float)bn.get_bodypart().get_xmin();
    x[1] = (float)bn.get_bodypart().get_xmax();
    y[0] = (float)bn.get_bodypart().get_ymin();
    y[1] = (float)bn.get_bodypart().get_ymax();
    z[0] = (float)bn.get_bodypart().get_zmin();
    z[1] = (float)bn.get_bodypart().get_zmax();

    ImGui::PushID(id);
    ImGui::InputFloat2("x", x);
    ImGui::InputFloat2("y", y);
    ImGui::InputFloat2("z", z);
    ImGui::PopID();

    box nbox;
    nbox.set_xmin(x[0]);
    nbox.set_xmax(x[1]);
    nbox.set_ymin(y[0]);
    nbox.set_ymax(y[1]);
    nbox.set_zmin(z[0]);
    nbox.set_zmax(z[1]);
    bn.set_bodypart(nbox);
}

// body UI callback.
void body_ui_callback() {
    if(ImGui::Button("Load Trial001")) {
        bodyobj->delete_derived_property<body>();
        main_scene_man.add_object_property(bodyobj->get_id(), bodyptr001);
        bodyptr = bodyptr001;

        bodyptr->set_play_anim(false);
        bodyptr->set_cur_anim_id(-1);
    }
    if(ImGui::Button("Load Trial002")) {
        bodyobj->delete_derived_property<body>();
        main_scene_man.add_object_property(bodyobj->get_id(), bodyptr002);
        bodyptr = bodyptr002;

        bodyptr->set_play_anim(false);
        bodyptr->set_cur_anim_id(-1);
    }
    if(ImGui::Button("Load Trial004")) {
        bodyobj->delete_derived_property<body>();
        main_scene_man.add_object_property(bodyobj->get_id(), bodyptr004);
        bodyptr = bodyptr004;

        bodyptr->set_play_anim(false);
        bodyptr->set_cur_anim_id(-1);
    }

    ImGui::Text("Animation");

    //static int frame_id;
    ImGui::Text("Frame number : %d", (int)bodyptr->get_anim_pos_c().size());
    // ImGui::InputInt("frame", &frame_id);
    // if(ImGui::Button("Show frame")) {
    //     int animsize = (int)bodyptr->get_anim_pos_c().size();
    //     if(frame_id >= animsize) {
    //         std::cerr << "Invalid frame ID" << std::endl;
    //     }
    //     else {
    //         transformation globalTR;
    //         globalTR.translate(bodyptr->get_anim_pos_c().at(frame_id));
    //         bodyptr->get_owner()->set_transformation(globalTR);

    //         int nodesize = (int)bodyptr->get_size();
    //         for (int i = 0; i < nodesize; i++)
    //         {
    //             auto &node = bodyptr->get_node(i);
    //             const transformation &TR = node.get_anim_c().at(frame_id);
    //             node.get_joint()->setTR(TR);
    //         }
    //     }
    // }

    if(bodyptr->get_play_anim()) {
        ImGui::Text("Current frame : %d", bodyptr->get_cur_anim_id());
    }

    if(bodyptr->get_play_anim()) {
        if (ImGui::Button("Stop animation"))
        {
            bodyptr->set_play_anim(false);
            bodyptr->set_cur_anim_id(-1);
        }
    }
    else {
        if (ImGui::Button("Play animation"))
        {
            bodyptr->set_play_anim(true);
            bodyptr->set_cur_anim_id(-1);
        }
    }
    

    // ImGui::Text("Rendering configuration");

    // for(int i = 0; i < bodyptr->get_size(); i++) {
    //     body_node_ui_callback(bodyptr->get_node(i), i);
    // }
}

static void compute_callback() {
    // create a body.
    bodyobj = std::make_shared<object>("body");
    main_scene_man.get_object_manager().add_object(bodyobj);
    body animbody001 = bvh_parser::parse("motion_data/Trial001.bvh");
    body animbody002 = bvh_parser::parse("motion_data/Trial002.bvh");
    body animbody004 = bvh_parser::parse("motion_data/Trial004.bvh");
    bodyptr001 = std::make_shared<body>(animbody001);
    bodyptr002 = std::make_shared<body>(animbody002);
    bodyptr004 = std::make_shared<body>(animbody004);

    main_scene_man.add_object_property(bodyobj->get_id(), bodyptr001);
    bodyptr = bodyptr001;

    // register body ui callback.
    bodyptr001->set_ui_callback(body_ui_callback);
    bodyptr002->set_ui_callback(body_ui_callback);
    bodyptr004->set_ui_callback(body_ui_callback);
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
    add_axis();

    // call compute.
    compute_callback();

    glutMainLoop();

    // clean up imgui.
    ui_end();

    return 0;
}