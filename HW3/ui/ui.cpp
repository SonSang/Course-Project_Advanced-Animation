#include "ui.hpp"
#include "../imgui/imgui_include_freeglut.hpp"

// Main menu bar ui.
void ui_main_menu_bar()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Camera"))
        {
            if (ImGui::MenuItem("Show camera configuration"))
            {
                set_show_camera_config_window(true);
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Objects"))
        {
            if (ImGui::MenuItem("Show objects"))
            {
                set_show_object_window(true);
            }
            ImGui::EndMenu();
        }
        /*if (ImGui::BeginMenu("Scene graph"))
				{
					if (ImGui::MenuItem("Show scene graph")) { set_show_scene_graph_window(true); }
					ImGui::EndMenu();
				}*/
        ImGui::EndMainMenuBar();
    }
}

// This is where root ui windows are rendered.
void ui_display_callback(scene_manager &scene_man)
{
    ui_main_menu_bar();

    // camera.
    //ui_camera_config_window();
    // object.
    ui_object_window(scene_man);
    // scene graph.
    //ui_scene_graph_window();
    //ui_scene_graph_node_add_window();
    //ui_scene_graph_node_config_window();
}
void ui_keyboard_callback(unsigned char key, int x, int y)
{
    ImGui_ImplGLUT_KeyboardFunc(key, x, y);
}
void ui_keyboard_up_callback(unsigned char key, int x, int y)
{
    ImGui_ImplGLUT_KeyboardUpFunc(key, x, y);
}
void ui_special_callback(int key, int x, int y)
{
    ImGui_ImplGLUT_SpecialFunc(key, x, y);
}
void ui_special_up_callback(int key, int x, int y)
{
    ImGui_ImplGLUT_SpecialUpFunc(key, x, y);
}
void ui_mouse_button_callback(GLint button, GLint action, GLint x, GLint y)
{
    ImGui_ImplGLUT_MouseFunc(button, action, x, y);
}
void ui_mouse_move_callback(GLint x, GLint y)
{
    ImGui_ImplGLUT_MotionFunc(x, y);
}
void ui_mouse_wheel_callback(GLint button, GLint dir, GLint x, GLint y)
{
    ImGui_ImplGLUT_MouseWheelFunc(button, dir, x, y);
}
void ui_reshape_callback(GLint width, GLint height)
{
    ImGui_ImplGLUT_ReshapeFunc(width, height);
}

void ui_display_callback_beg(scene_manager &scene_man)
{
    // imgui frame start.
    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplGLUT_NewFrame();
    ui_display_callback(scene_man);

    // rendering.
    ImGui::Render();
    ImGuiIO &io = ImGui::GetIO();
}
void ui_display_callback_end()
{
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
}

void ui_beg()
{
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    // == Setup Dear ImGui style
    //ImGui::StyleColorsDark();
    ImGui::StyleColorsClassic();
    // == Setup Platform/Renderer bindings
    ImGui_ImplGLUT_Init();
    //ImGui_ImplGLUT_InstallFuncs();
    ImGui_ImplOpenGL2_Init();
}
void ui_end()
{
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGLUT_Shutdown();
    ImGui::DestroyContext();
}