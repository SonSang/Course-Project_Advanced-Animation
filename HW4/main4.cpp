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

#include "live_body.hpp"
#include "bvh_parser.hpp"

#include "imgui/imgui_include_freeglut.hpp"

GLuint window_width = 800, window_height = 600;
GLuint window_pos_x = 100, window_pos_y = 100;

// main things.
static camera		    main_camera;
static mouse		    main_mouse;
static scene_manager    main_scene_man;


std::shared_ptr<object> bodyobj;
std::shared_ptr<live_body> bodyptr;

double plane_y = -83;

// basic clear color.
static float clear_color[4] = {(float)(212.0 / 255.0), (float)(244.0 / 255.0), (float)(250.0 / 255.0), 1.00f};

static void init_camera()
{
    main_camera.update_view();
    main_camera.set_speed(50);
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
    case ' ': { // Jump 
        if(bodyptr->get_cur_state_c().name == "idle") {
            int mode = glutGetModifiers();
            if(mode == GLUT_ACTIVE_SHIFT) {
                bodyptr->set_next_state_id(bodyptr->get_state_id("high_jump"));
            }
            else {
                bodyptr->set_next_state_id(bodyptr->get_state_id("jump"));
            }
        }
        break;
    }
    // case 'k' : case 'K' :
    //     try {
    //         bodyptr->set_next_state_id(bodyptr->get_state_id("idle"));
    //     } catch(std::exception &e) {

    //     }
    //     break;
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
    switch (key)
    {
    case 'I' : case 'i' :   // move forward
        try {
            bodyptr->set_next_state_id(bodyptr->get_state_id("idle"));
        } catch(std::exception &e) {

        }
        break;
    }
    //ui callback.
    ui_keyboard_up_callback(key, x, y);
}
static void special_button_callback(int key, int x, int y)
{
    if(bodyptr->get_cur_state_c().name == "jump" ||
        bodyptr->get_cur_state_c().name == "high_jump")
        return;
    switch(key) {
    case GLUT_KEY_UP: {
        int mod = glutGetModifiers();
        try {
            if(mod == GLUT_ACTIVE_SHIFT) {
                bodyptr->set_next_state_id(bodyptr->get_state_id("fast_walk"));
            }                
            else
                bodyptr->set_next_state_id(bodyptr->get_state_id("normal_walk"));
        } catch(std::exception &e){}
        break;
    }        
    case GLUT_KEY_LEFT:
        try {
            if(bodyptr->get_cur_state_c().name == "normal_walk" ||
                bodyptr->get_cur_state_c().name == "normal_walk_veer_left")
                bodyptr->set_next_state_id(bodyptr->get_state_id("normal_walk_veer_left"));
            else if(bodyptr->get_cur_state_c().name == "fast_walk" ||
                bodyptr->get_cur_state_c().name == "fast_walk_veer_left")
                bodyptr->set_next_state_id(bodyptr->get_state_id("fast_walk_veer_left"));
        } catch(std::exception &e) {}
        break;
    case GLUT_KEY_RIGHT:
        try {
            if(bodyptr->get_cur_state_c().name == "normal_walk" ||
                bodyptr->get_cur_state_c().name == "normal_walk_veer_right")
                bodyptr->set_next_state_id(bodyptr->get_state_id("normal_walk_veer_right"));
            else if(bodyptr->get_cur_state_c().name == "fast_walk" ||
                bodyptr->get_cur_state_c().name == "fast_walk_veer_right")
                bodyptr->set_next_state_id(bodyptr->get_state_id("fast_walk_veer_right"));
        } catch(std::exception &e) {}
        break;
    case GLUT_KEY_F2:
        bodyptr->set_cur_state_id(bodyptr->get_root_state_id());
        bodyptr->set_next_state_id(bodyptr->get_root_state_id());
        bodyptr->cur_clip = bodyptr->get_clip(bodyptr->get_root_state_c().clip_id);
        bodyptr->cur_clip.anim.fID = 0;
        break;
    }
    

    // ui callback.
    ui_special_callback(key, x, y);
}
static void special_button_up_callback(int key, int x, int y)
{
    if(bodyptr->get_cur_state_c().name == "jump" ||
        bodyptr->get_cur_state_c().name == "high_jump")
        return;
    switch(key) {
    case GLUT_KEY_UP: {
        try {
            bodyptr->set_next_state_id(bodyptr->get_state_id("idle"));
        } catch(std::exception &e){}
        break;
    }
    case GLUT_KEY_LEFT:
        try {
            if(bodyptr->get_cur_state_c().name == "normal_walk_veer_left")
                bodyptr->set_next_state_id(bodyptr->get_state_id("normal_walk"));
            else if(bodyptr->get_cur_state_c().name == "fast_walk_veer_left")
                bodyptr->set_next_state_id(bodyptr->get_state_id("fast_walk"));
        } catch(std::exception &e) {}
        break;
    case GLUT_KEY_RIGHT:
        try {
            if(bodyptr->get_cur_state_c().name == "normal_walk_veer_right")
                bodyptr->set_next_state_id(bodyptr->get_state_id("normal_walk"));
            else if(bodyptr->get_cur_state_c().name == "fast_walk_veer_right")
                bodyptr->set_next_state_id(bodyptr->get_state_id("fast_walk"));
        } catch(std::exception &e) {}
        break;   
    }
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

    glColor3d(0.7, 0.4, 0.4);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_QUADS);
    glVertex3d(1000, plane_y, 1000);
    glVertex3d(1000, plane_y, -1000);
    glVertex3d(-1000, plane_y, -1000);
    glVertex3d(-1000, plane_y, 1000);
    glEnd();

    // draw UI.
    ui_display_callback_end();

    glutSwapBuffers();
}

static void idle_callback()
{
    glutPostRedisplay();
}


// body UI callback.
void body_ui_callback() {
    // ImGui::Text("Animation");

    // ImGui::Text("Animation List");
    // for(auto it = bodyptr->anim_pool.begin(); it != bodyptr->anim_pool.end(); it++) {
    //     if(ImGui::Button(it->name.c_str())) 
    //         bodyptr->set_anim_id(std::distance(bodyptr->anim_pool.begin(), it));
    // }

    // static char anim_name[100] = "animation_name";
    // static char bvh_path[100] = "motion_data2/mrl/tpose2.bvh";
    // ImGui::InputText("Animation name", anim_name, 100);
    // ImGui::InputText("BVH file", bvh_path, 100);
    // if(ImGui::Button("Load animation")) {
    //     animation anim;
    //     bool e_flag = false;
    //     try {
    //         anim = bvh_parser::parse_anim(bvh_path);
    //     } catch(std::invalid_argument &e) {
    //         std::cerr<<"Invalid file name : "<<e.what()<<std::endl;
    //         e_flag = true;
    //     }
    //     if(!e_flag) {
    //         try {
    //             bodyptr->add_anim(anim_name, anim);        
    //         } catch(std::invalid_argument &e) {
    //             std::cerr<<e.what()<<std::endl;
    //         }            
    //     }            
    // }

    // static int anim_speed = 1;
    // ImGui::InputInt("animation speed", &anim_speed);
    // bodyptr->set_anim_speed(anim_speed);

    static double frame_time = 1e-3;
    ImGui::InputDouble("frame time", &frame_time);
    bodyptr->set_frame_time(frame_time);

    // if(bodyptr->get_play_mode() == bodyptr->STOP) {
    //     if(ImGui::Button("Play STATIC animation")) {
    //         if(bodyptr->valid_anim_id(bodyptr->get_anim_id()))
    //             bodyptr->set_play_mode(bodyptr->LIVE_STATIC);
    //         else
    //             std::cerr<<"Invalid animation id. Please select animation above."<<std::endl;            
    //     }
    // }
    // else if(bodyptr->get_play_mode() == bodyptr->LIVE_STATIC) {
    //     ImGui::Text("Animation length : %d", (int)bodyptr->anim_pool.at(bodyptr->anim_id).anim.fSequence.begin()->second.fJointTR.size());
    //     ImGui::Text("Current frame : %d", bodyptr->anim_pool.at(bodyptr->anim_id).anim.fID);
    //     if(ImGui::Button("Stop animation")) {
    //         bodyptr->set_play_mode(bodyptr->STOP);
    //     }
    // }

    // ImGui::Separator();

    // ImGui::Text("Clip List");
    // static int cur_clip_id = -1;
    // if(bodyptr->valid_clip_id(cur_clip_id)) {
    //     ImGui::Text("Current clip : %s", bodyptr->get_clip(cur_clip_id).name.c_str());
    // }
    // for(auto it = bodyptr->clip_pool.begin(); it != bodyptr->clip_pool.end(); it++) {
    //     if(ImGui::Button(it->name.c_str())) {
    //         cur_clip_id = std::distance(bodyptr->clip_pool.begin(), it);
    //     }
    // }

    // static char clip_name[100];
    // static int clip_range[2];
    // ImGui::InputText("clip name", clip_name, 100);
    // if(bodyptr->valid_anim_id(bodyptr->get_anim_id())) {
    //     ImGui::Text("Current anim's length : %d", bodyptr->anim_pool.at(bodyptr->anim_id).anim.length());
    //     ImGui::InputInt2("clip range", clip_range);
    //     if(ImGui::Button("Make clip")) {
    //         try {
    //             live_body::clip nclip;
    //             nclip.name = std::string(clip_name);
    //             nclip.anim = bodyptr->anim_pool.at(bodyptr->anim_id).anim.extract(clip_range[0], clip_range[1]);

    //             bodyptr->add_clip(nclip);
    //         } catch(std::exception &e) {
    //             std::cerr<<e.what()<<std::endl;
    //         }
    //     }
    //     if(ImGui::Button("Clip no move")) {
    //         if(bodyptr->valid_clip_id(cur_clip_id)) {
    //             live_body::clip *c = const_cast<live_body::clip *>(&bodyptr->get_clip(cur_clip_id));
    //             for(auto it = c->anim.fSequence.at("Root").fJointTR.begin(); 
    //                 it != c->anim.fSequence.at("Root").fJointTR.end(); 
    //                 it++) {
    //                     *it = transformation();
    //                 }
    //         }
    //     }
    //     if(ImGui::Button("Clip no move2")) {
    //         if(bodyptr->valid_clip_id(cur_clip_id)) {
    //             live_body::clip *c = const_cast<live_body::clip *>(&bodyptr->get_clip(cur_clip_id));
    //             for(auto it = c->anim.fSequence.begin(); it != c->anim.fSequence.end(); it++) {
    //                 for(auto it2 = it->second.fJointTR.begin(); it2 != it->second.fJointTR.end(); it2++) {
    //                     *it2 = transformation();
    //                 }
    //             }
    //         }
    //     }
    //     if(ImGui::Button("Set root y zero")) {
    //         if(bodyptr->valid_clip_id(cur_clip_id)) {
    //             live_body::clip *c = const_cast<live_body::clip *>(&bodyptr->get_clip(cur_clip_id));
    //             double t[3];
    //             c->anim.fSequence.at("Root").fJointTR.begin()->get_translation(t);
    //             double init_y = t[1];
    //             for(auto it = c->anim.fSequence.at("Root").fJointTR.begin(); 
    //                 it != c->anim.fSequence.at("Root").fJointTR.end();
    //                 it++) {
    //                     it->translate(0, -init_y, 0);
    //                 }
    //         }
    //     }
    // }   

    // ImGui::Separator();

    // ImGui::Text("State List");
    // ImGui::Text("Root state : ");
    // ImGui::SameLine();
    // if(bodyptr->valid_root_state()) 
    //     ImGui::Text("%s", bodyptr->get_root_state_c().name.c_str());
    // else
    //     ImGui::Text("Invalid root");

    // for(auto it = bodyptr->state_pool.begin(); it != bodyptr->state_pool.end(); it++) {
    //     int dist = std::distance(bodyptr->state_pool.begin(), it);
    //     ImGui::PushID(dist);
    //     if(ImGui::Button(it->name.c_str()))
    //         bodyptr->set_cur_state_id(dist);
    //     ImGui::SameLine();
    //     if(bodyptr->valid_clip_id(it->clip_id))
    //         ImGui::Text("Clip : %s", bodyptr->get_clip(it->clip_id).name.c_str());
    //     ImGui::SameLine();
    //     if(ImGui::Button("Set clip")) 
    //         it->clip_id = cur_clip_id;
    //     ImGui::SameLine();
    //     if(ImGui::Button("Set root"))
    //         bodyptr->set_root_state(dist);
    //     ImGui::PopID();
    // }

    // static char state_name[100];
    // ImGui::InputText("state name", state_name, 100);
    // if(ImGui::Button("Make state")) {
    //     bodyptr->add_state(std::string(state_name), -1);
    // }

    // if(bodyptr->get_play_mode() == bodyptr->STOP || bodyptr->get_play_mode() == bodyptr->LIVE_STATIC) {
    //     if(ImGui::Button("Play DYNAMIC animation")) {
    //         if(bodyptr->valid_root_state()) {
    //             bodyptr->set_cur_state_id(bodyptr->get_root_state_id());
    //             bodyptr->set_next_state_id(bodyptr->get_root_state_id());
    //             bodyptr->cur_clip = bodyptr->get_clip(bodyptr->get_root_state_c().clip_id);
    //             bodyptr->cur_clip.anim.fID = 0;
    //             bodyptr->set_play_mode(bodyptr->LIVE_DYNAMIC);
    //         }
    //         else
    //             std::cerr<<"Invalid root state id. Please select state above."<<std::endl;            
    //     }
    // }
    if(bodyptr->get_play_mode() == bodyptr->LIVE_DYNAMIC) {
        const live_body::clip& cur_clip = bodyptr->cur_clip;
        ImGui::Text("Current state : %s", bodyptr->get_cur_state_c().name.c_str());
        ImGui::Text("Animation length : %d", cur_clip.anim.length());
        ImGui::Text("Current frame : %d", cur_clip.anim.fID);
        // if(ImGui::Button("Stop animation")) {
        //     bodyptr->set_play_mode(bodyptr->STOP);
        // }
    }

    // ImGui::Separator();
    // static char path[100] = "file.txt";
    // ImGui::InputText("path", path, 100);
    // if(ImGui::Button("Save"))
    //     bodyptr->save(path);
    // if(ImGui::Button("Load"))
    //     bodyptr->load(path);

    // ImGui::InputDouble("plane", &plane_y);
}

static void compute_callback() {
    // create a body.
    bodyobj = std::make_shared<object>("Body");
    main_scene_man.get_object_manager().add_object(bodyobj);
    live_body animbody = bvh_parser::parse_live_body("motion_data/EmptyBody.bvh");
    bodyptr = std::make_shared<live_body>(animbody);

    main_scene_man.add_object_property(bodyobj->get_id(), bodyptr);

    // register body ui callback.
    bodyptr->set_ui_callback(body_ui_callback);

    bodyptr->load("myanim.txt");
    bodyptr->set_root_state("idle");
    bodyptr->set_cur_state_id(bodyptr->get_root_state_id());
    bodyptr->set_next_state_id(bodyptr->get_root_state_id());
    bodyptr->cur_clip = bodyptr->get_clip(bodyptr->get_root_state_c().clip_id);
    bodyptr->cur_clip.anim.fID = 0;
    bodyptr->set_play_mode(bodyptr->LIVE_DYNAMIC);

    bodyobj->rotate(vec3d(0, 0, 0), vec3d(0, 1, 0), PI);
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