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

std::shared_ptr<object> bodyobj;
std::shared_ptr<body> bodyptr;

std::vector<int> bodypart;
std::vector<transformation> bodypartTR;
struct bodypartTRinput {
    float targetLocation[3] = {0.0f, 0.0f, 0.0f};
    float targetRotation[3] = {0.0f, 0.0f, 0.0f};
    float prevTargetLocation[3] = {0.0f, 0.0f, 0.0f};
    float prevTargetRotation[3] = {0.0f, 0.0f, 0.0f};
};
std::vector<bodypartTRinput> bodypartTRi;

static void display_callback()
{
    ui_display_callback_beg(main_scene_man);

    glClearColor(clear_color[0], clear_color[1], clear_color[2], clear_color[3]);

    // draw canvas.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // render to screen.
    main_scene_man.render();

    // render moving body part.
    if(!bodypart.empty()) {
        int size = (int)bodypart.size();
        for(int i = 0; i < size; i++) {
            bodyptr->render_node(bodypart.at(i), bodypartTR.at(i), 1, 0, 0);
        }
    } 
        

    // draw UI.
    ui_display_callback_end();

    glutSwapBuffers();
}

static void idle_callback()
{
    glutPostRedisplay();
}

void toggle_body_part(int id) {
    int size = (int)bodypart.size();
    int index = -1;
    for(int i = 0; i < size; i++) {
        if(bodypart.at(i) == id)
            index = i;
    }
    if(index == -1) {
        // add to bodypart.
        bodypart.push_back(id);
        bodypartTR.push_back(bodyptr->get_global_transform(id));
        bodypartTRi.push_back(bodypartTRinput());
    }
    else {
        // delete from bodypart.
        bodypart.erase(bodypart.begin() + index);
        bodypartTR.erase(bodypartTR.begin() + index);
        bodypartTRi.erase(bodypartTRi.begin() + index);
    }
}

void update_bodypartTR_input(transformation &TR, bodypartTRinput &input) {
    if(input.prevTargetLocation[0] != input.targetLocation[0] ||
    input.prevTargetLocation[1] != input.targetLocation[1] ||
    input.prevTargetLocation[2] != input.targetLocation[2]) {
        // if there was translation update
        float 
            xoffset = input.targetLocation[0] - input.prevTargetLocation[0],
            yoffset = input.targetLocation[1] - input.prevTargetLocation[1],
            zoffset = input.targetLocation[2] - input.prevTargetLocation[2];
        TR.translate(xoffset, yoffset, zoffset);

        input.prevTargetLocation[0] = input.targetLocation[0]; 
        input.prevTargetLocation[1] = input.targetLocation[1]; 
        input.prevTargetLocation[2] = input.targetLocation[2]; 
    }
    if(input.prevTargetRotation[0] != input.targetRotation[0]) {   // rotation around local x axis.
        double roffset = input.targetRotation[0] - input.prevTargetRotation[0];
        double offset[3];
        TR.get_translation(offset);
        TR.rotate(vec3d(offset[0], offset[1], offset[2]), vec3d(offset[0] + 1.0, offset[1], offset[2]), roffset);
        input.prevTargetRotation[0] = input.targetRotation[0];
    }
    if(input.prevTargetRotation[1] != input.targetRotation[1]) {   // rotation around local y axis.
        double roffset = input.targetRotation[1] - input.prevTargetRotation[1];
        double offset[3];
        TR.get_translation(offset);
        TR.rotate(vec3d(offset[0], offset[1], offset[2]), vec3d(offset[0], offset[1] + 1.0, offset[2]), roffset);
        input.prevTargetRotation[1] = input.targetRotation[1];
    }
    if(input.prevTargetRotation[2] != input.targetRotation[2]) {   // rotation around local z axis.
        double roffset = input.targetRotation[2] - input.prevTargetRotation[2];
        double offset[3];
        TR.get_translation(offset);
        TR.rotate(vec3d(offset[0], offset[1], offset[2]), vec3d(offset[0], offset[1], offset[2] + 1.0), roffset);
        input.prevTargetRotation[2] = input.targetRotation[2];
    }
}

// body UI callback.
void body_ui_callback() {
    // Joint flexibility configuration. User can choose between revolute joint and ball-and-socket joint.
    // ImGui::Text("Joint configuration");
    // for(auto it = bodyptr->get_tree().begin(); it != bodyptr->get_tree().end(); it++) {
    //     ImGui::Text("%s", it->second.get_name().c_str());
    //     ImGui::PushID(it->first);
    //     if(it->second.get_joint_type() == 0) {
    //         // revolute joint 
    //         vec3d axis;
    //         std::shared_ptr<joint_revolute> jptr = std::dynamic_pointer_cast<joint_revolute>(it->second.get_joint());
    //         axis = jptr->get_axis();

    //         float axisf[3] = { (float)axis[0], (float)axis[1], (float)axis[2] };
            
    //         ImGui::InputFloat3("axis", axisf);
            
    //         axis.set(axisf[0], axisf[1], axisf[2]);
    //         if(axis.length() == 0.0)
    //             axis.set(1, 0, 0);
    //         jptr->set_axis(axis);

    //         if(ImGui::Button("Change to ball-and-socket")) {
    //             std::shared_ptr<joint_ballsocket> njptr = std::make_shared<joint_ballsocket>();
    //             it->second.set_joint(njptr);
    //             it->second.set_joint_type(1);
    //         }
    //     }   
    //     else {
    //         if(ImGui::Button("Change to revolute")) {
    //             std::shared_ptr<joint_revolute> njptr = std::make_shared<joint_revolute>();
    //             njptr->set_axis(vec3d(1, 0, 0));
    //             it->second.set_joint(njptr);
    //             it->second.set_joint_type(0);
    //         }
    //     }
    //     ImGui::PopID();
    // }

    if(ImGui::Button("Set target : null")) {
        bodypart.clear();
        bodypartTR.clear();
        bodypartTRi.clear();
    }

    if(ImGui::Button("Toggle target : Left hand")) {
        toggle_body_part(bodyptr->get_node_id("lhand"));
    }

    if(ImGui::Button("Toggle target : Right hand")) {
        toggle_body_part(bodyptr->get_node_id("rhand"));
    }

    if(ImGui::Button("Toggle target : Left toes")) {
        toggle_body_part(bodyptr->get_node_id("ltoes"));
    }

    if(ImGui::Button("Toggle target : Right toes")) {
        toggle_body_part(bodyptr->get_node_id("rtoes"));
    }

    if(!bodypart.empty()) {
        int size = (int)bodypart.size();
        for(int i = 0; i < size; i++) {
            int id = bodypart.at(i);

            ImGui::PushID(id);
            ImGui::Text("Move %s with sliders below!", bodyptr->get_node(id).get_name().c_str());

            ImGui::SliderFloat3("translation", bodypartTRi.at(i).targetLocation, -200, 200);
            ImGui::SliderFloat3("rotation", bodypartTRi.at(i).targetRotation, -3.14, 3.14);

            update_bodypartTR_input(bodypartTR.at(i), bodypartTRi.at(i));

            ImGui::PopID();
        }

        static double deltaT = 1e-2;
        static int animSize = 1000;
        static bool root_fix = true;
        static double root_weight = 10;
        ImGui::InputDouble("delta time", &deltaT);
        ImGui::InputInt("anim size", &animSize);
        if(root_fix) {
            if(ImGui::Button("Move root")) root_fix = false;
        }
        else {
            ImGui::InputDouble("Root flexibility", &root_weight);
            if(ImGui::Button("Fix root")) root_fix = true;
        }

        if(ImGui::Button("Inverse kinematics")) {
            bodyptr->inverse_kinematics(bodypart, bodypartTR, deltaT, animSize, root_fix, root_weight);
        }

        ImGui::Text("Animation");

        static int anim_speed = 1;
        ImGui::Text("Frame number : %d", (int)bodyptr->get_anim_pos_c().size());
        if(bodyptr->get_play_anim()) {
            ImGui::Text("Current frame : %d", bodyptr->get_cur_anim_id());
        }

        ImGui::InputInt("animation speed", &anim_speed);
        bodyptr->set_anim_speed(anim_speed);

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
    }
}

static void compute_callback() {
    // create a body.
    bodyobj = std::make_shared<object>("body");
    main_scene_man.get_object_manager().add_object(bodyobj);
    body animbody = bvh_parser::parse("motion_data/EmptyBody.bvh");
    bodyptr = std::make_shared<body>(animbody);

    main_scene_man.add_object_property(bodyobj->get_id(), bodyptr);

    // register body ui callback.
    bodyptr->set_ui_callback(body_ui_callback);
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