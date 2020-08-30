#include "ui_object.hpp"
#include "../scene/property_render_geometry.hpp"
#include "../imgui/imgui_include_freeglut.hpp"

#include <string>
#include <vector>
#include <typeinfo>

static bool show_object_window = false;

void set_show_object_window(bool b)
{
    show_object_window = b;
}
bool get_show_object_window()
{
    return show_object_window;
}

static void add_property_render_point(scene_manager &scene_man, std::shared_ptr<object> &object_)
{
    point
        npt;
    std::shared_ptr<property_render_geometry<point>>
        prg_ptr = std::make_shared<property_render_geometry<point>>(object_->get_name());
    prg_ptr->set_geometry(npt);
    scene_man.add_object_property(object_->get_id(), prg_ptr);
}
static void add_property_render_line(scene_manager &scene_man, std::shared_ptr<object> &object_)
{
    line
        nln;
    std::shared_ptr<property_render_geometry<line>>
        prg_ptr = std::make_shared<property_render_geometry<line>>(object_->get_name());
    prg_ptr->set_geometry(nln);
    scene_man.add_object_property(object_->get_id(), prg_ptr);
}
static void add_property_render_triangle(scene_manager &scene_man, std::shared_ptr<object> &object_)
{
    triangle
        ntr;
    std::shared_ptr<property_render_geometry<triangle>>
        prg_ptr = std::make_shared<property_render_geometry<triangle>>(object_->get_name());
    prg_ptr->set_geometry(ntr);
    scene_man.add_object_property(object_->get_id(), prg_ptr);
}
static void add_property_render_quad(scene_manager &scene_man, std::shared_ptr<object> &object_)
{
    quad
        nqd;
    std::shared_ptr<property_render_geometry<quad>>
        prg_ptr = std::make_shared<property_render_geometry<quad>>(object_->get_name());
    prg_ptr->set_geometry(nqd);
    scene_man.add_object_property(object_->get_id(), prg_ptr);
}
static void add_property_render_box(scene_manager &scene_man, std::shared_ptr<object> &object_)
{
    box
        nbox;
    std::shared_ptr<property_render_geometry<box>>
        prg_ptr = std::make_shared<property_render_geometry<box>>(object_->get_name());
    prg_ptr->set_geometry(nbox);
    scene_man.add_object_property(object_->get_id(), prg_ptr);
}

void ui_object_window(scene_manager &scene_man)
{
    if (get_show_object_window())
    {
        ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiCond_FirstUseEver);
        if (!ImGui::Begin("objects", &show_object_window))
        {
            ImGui::End();
            return;
        }

        // check if global variables are ok.

        // static variables.
        static unsigned int selected_object_id = -1;
        static unsigned int selected_property_id = -1;

        object_manager
            &object_man = scene_man.get_object_manager();

        // 1. Object window.
        ImGui::BeginChild("select object", ImVec2(100.0f, 0.0f), true);

        ImGui::Text("Objects");
        ImGui::SameLine();
        // 1-1. add new object.
        if (ImGui::Button("Add"))
            ImGui::OpenPopup("add object popup");
        if (ImGui::BeginPopup("add object popup"))
        {
            ImGui::PushItemWidth(100);
            static char name_str[64] = "Name";
            ImGui::InputText("", name_str, 64);
            ImGui::PopItemWidth();

            if (ImGui::Button("Complete"))
            {
                auto
                    nobject_ptr = std::make_shared<object>(name_str);
                object_man.add_object(nobject_ptr);
            }
            ImGui::EndPopup();
        }

        ImGui::Separator();

        // 1-2. show objects.
        if (scene_man.get_object_manager().size() == 0)
        {
            ImGui::Text("None");
        }
        else
        {
            for (auto it = object_man.get_list_begin(); it != object_man.get_list_end(); it++)
            {
                if (ImGui::Selectable(it->second->get_name().c_str()))
                {
                    selected_object_id = it->second->get_id();
                    selected_property_id = -1; // reset chosen proerty id.
                }
            }
        }

        ImGui::EndChild();

        ImGui::SameLine();

        // 2. Property window.
        ImGui::BeginChild("select property", ImVec2(130.0f, 0.0f), true);

        ImGui::Text("Properties");
        // 2-1. add new property.
        if (selected_object_id != -1)
        {
            std::shared_ptr<object>
                target_object = object_man.get_object(selected_object_id);
            ImGui::SameLine();
            if (ImGui::Button("Add"))
                ImGui::OpenPopup("add property popup");
            if (ImGui::BeginPopup("add property popup"))
            {
                if (ImGui::BeginMenu("Render"))
                {
                    if (ImGui::MenuItem("Point"))
                    {
                        add_property_render_point(scene_man, target_object);
                    }
                    if (ImGui::MenuItem("Line"))
                    {
                        add_property_render_line(scene_man, target_object);
                    }
                    if (ImGui::MenuItem("Triangle"))
                    {
                        add_property_render_triangle(scene_man, target_object);
                    }
                    if (ImGui::MenuItem("Quad"))
                    {
                        add_property_render_quad(scene_man, target_object);
                    }
                    if (ImGui::MenuItem("Box"))
                    {
                        add_property_render_box(scene_man, target_object);
                    }
                    ImGui::EndMenu();
                }
                ImGui::EndPopup();
            }
        }
        ImGui::Separator();

        // 2-2. show properties.
        if (selected_object_id != -1)
        {
            std::shared_ptr<object>
                target_object = object_man.get_object(selected_object_id);

            if (target_object->property_size() == 0)
            {
                ImGui::Text("None");
            }
            else
            {
                for (int i = 0; i < target_object->property_size(); i++)
                {
                    ImGui::PushID(i);
                    if (ImGui::Selectable(target_object->get_property(i)->type_name().c_str()))
                        selected_property_id = i;
                    ImGui::PopID();
                }
            }
        }

        ImGui::EndChild();

        ImGui::SameLine();

        // 3. Property configuration window.
        ImGui::BeginChild("property configuration", ImVec2(0.0f, 0.0f), true);

        if (selected_property_id != -1)
        {
            std::shared_ptr<property>
                property_ptr = object_man.get_object(selected_object_id)->get_property(selected_property_id);
            property_ptr->render_ui();
        }

        ImGui::EndChild();

        ImGui::End();
    }
}