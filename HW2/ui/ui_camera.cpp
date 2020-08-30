#include "ui_camera.hpp"

// imgui
#include "../imgui/imgui_include_freeglut.hpp"

#include <string>
#include <iostream>

camera *mc = nullptr;

// [show] var.
bool show_camera_config_window = false;
void set_show_camera_config_window(bool b)
{
    show_camera_config_window = b;
}
bool get_show_camera_config_window()
{
    return show_camera_config_window;
}

void is_mc_valid()
{
    if (mc == nullptr)
    {
        std::cerr<<"Main camera is not initialized..."<<std::endl;
    }
}

void ui_camera_config_window()
{
    if (show_camera_config_window)
    {
        if (!ImGui::Begin("Camera", &show_camera_config_window))
        {
            ImGui::End();
            return;
        }

        ImGui::Text("Camera configuration");

        ImGui::Text("Camera speed : ");
        ImGui::PushItemWidth(50);
        static char camera_speed_str[64] = "0.0";
        ImGui::InputText("speed", camera_speed_str, 64, ImGuiInputTextFlags_CharsDecimal);
        ImGui::PopItemWidth();

        if (ImGui::Button("Apply"))
        {
            is_mc_valid();
            mc->set_speed(std::stod(std::string(camera_speed_str)));
            show_camera_config_window = false;
        }

        ImGui::End();
    }
}
void ui_set_main_camera(camera &camera_)
{
    mc = &camera_;
}