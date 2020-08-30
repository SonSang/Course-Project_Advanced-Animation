// implement [render_ui] functions of each property using imgui.

#include "property_render_geometry.hpp"
#include "../imgui/imgui_include_freeglut.hpp"

#include <string.h>

// property_render
void property_render::render_ui()
{
    ImGui::Text("Render configuration");

    if (is_valid())
    {
        if (ImGui::Button("Invalid"))
        {
            set_valid(false);
        }
    }
    else
    {
        if (ImGui::Button("Valid"))
        {
            set_valid(true);
        }
    }
}
// property_render_geometry
#define prg property_render_geometry
template<>
void prg<point>::render_ui()
{
    property_render::render_ui();

    ImGui::Text("Point configuration");

    // inputs.
    static char input_name[64];
    static double input_pos[3];
    static float input_size;
    static float input_color[3];

    ImGui::Text("Name : ");
    ImGui::PushItemWidth(50);
    strncpy(input_name, this->get_name().c_str(), 64);
    ImGui::InputText("", input_name, 64);
    ImGui::PopItemWidth();

    ImGui::Text("Position : ");
    ImGui::PushItemWidth(100);
    input_pos[0] = this->get_geometry().get_point()[0];
    input_pos[1] = this->get_geometry().get_point()[1];
    input_pos[2] = this->get_geometry().get_point()[2];
    ImGui::InputDouble("x", &input_pos[0]);
    ImGui::SameLine();
    ImGui::InputDouble("y", &input_pos[1]);
    ImGui::SameLine();
    ImGui::InputDouble("z", &input_pos[2]);
    ImGui::PopItemWidth();

    ImGui::Text("Size : ");
    ImGui::PushItemWidth(100);
    input_size = (float)this->get_config().size;
    ImGui::SliderFloat("size", &input_size, 1, 100);
    ImGui::PopItemWidth();

    ImGui::Text("Color : ");

    for (int i = 0; i < 3; i++)
        input_color[i] = (float)this->get_config().inner_color[i];
    ImGui::ColorEdit3("", input_color);

    if (ImGui::Button("Toggle wireframe"))
    {
        this->get_config().wireframe = !this->get_config().wireframe;
    }

    point
        npt;
    npt.set_point(input_pos[0], input_pos[1], input_pos[2]);
    this->set_name(input_name);
    this->set_geometry(npt);
    this->get_config().inner_color[0] = input_color[0];
    this->get_config().inner_color[1] = input_color[1];
    this->get_config().inner_color[2] = input_color[2];
    this->get_config().size = input_size;
}
template<>
void prg<line>::render_ui()
{
    property_render::render_ui();
    ImGui::Text("Line configuration");

    // inputs.
    static char input_name[64];
    static double input_beg[3];
    static double input_end[3];
    static float input_size;
    static float input_color[3];

    ImGui::Text("Name : ");
    ImGui::PushItemWidth(50);
    strncpy(input_name, this->get_name().c_str(), 64);
    ImGui::InputText("", input_name, 64);
    ImGui::PopItemWidth();

    ImGui::Text("Begin : ");
    ImGui::PushItemWidth(100);
    input_beg[0] = this->get_geometry().get_beg()[0];
    input_beg[1] = this->get_geometry().get_beg()[1];
    input_beg[2] = this->get_geometry().get_beg()[2];
    ImGui::PushID(0);
    ImGui::InputDouble("x", &input_beg[0]);
    ImGui::SameLine();
    ImGui::InputDouble("y", &input_beg[1]);
    ImGui::SameLine();
    ImGui::InputDouble("z", &input_beg[2]);
    ImGui::PopID();
    ImGui::PopItemWidth();

    ImGui::Text("End : ");
    ImGui::PushItemWidth(100);
    input_end[0] = this->get_geometry().get_end()[0];
    input_end[1] = this->get_geometry().get_end()[1];
    input_end[2] = this->get_geometry().get_end()[2];
    ImGui::PushID(1);
    ImGui::InputDouble("x", &input_end[0]);
    ImGui::SameLine();
    ImGui::InputDouble("y", &input_end[1]);
    ImGui::SameLine();
    ImGui::InputDouble("z", &input_end[2]);
    ImGui::PopID();
    ImGui::PopItemWidth();

    ImGui::Text("Size : ");
    ImGui::PushItemWidth(100);
    input_size = (float)this->get_config().size;
    ImGui::SliderFloat("size", &input_size, 1, 10);
    ImGui::PopItemWidth();

    ImGui::Text("Color : ");
    for (int i = 0; i < 3; i++)
        input_color[i] = (float)this->get_config().inner_color[i];
    ImGui::ColorEdit3("", input_color);

    if (ImGui::Button("Toggle wireframe"))
    {
        this->get_config().wireframe = !this->get_config().wireframe;
    }

    line
        nln;
    nln.set_beg(input_beg[0], input_beg[1], input_beg[2]);
    nln.set_end(input_end[0], input_end[1], input_end[2]);
    this->set_name(input_name);
    this->set_geometry(nln);
    this->get_config().inner_color[0] = input_color[0];
    this->get_config().inner_color[1] = input_color[1];
    this->get_config().inner_color[2] = input_color[2];
    this->get_config().size = input_size;
}
template<>
void prg<triangle>::render_ui()
{
    property_render::render_ui();
    ImGui::Text("Triangle configuration");

    // inputs.
    static char input_name[64];
    static double input_pos[3][3];
    static float input_size;
    static float input_inner_color[3];
    static float input_outer_color[3];

    ImGui::Text("Name : ");
    ImGui::PushItemWidth(50);
    strncpy(input_name, this->get_name().c_str(), 64);
    ImGui::InputText("", input_name, 64);
    ImGui::PopItemWidth();

    ImGui::Text("Position : ");
    for (int i = 0; i < 3; i++)
    {
        ImGui::PushItemWidth(100);
        input_pos[i][0] = this->get_geometry().get_vertices().at(i)[0];
        input_pos[i][1] = this->get_geometry().get_vertices().at(i)[1];
        input_pos[i][2] = this->get_geometry().get_vertices().at(i)[2];
        ImGui::PushID(i);
        ImGui::InputDouble("x", &input_pos[i][0]);
        ImGui::SameLine();
        ImGui::InputDouble("y", &input_pos[i][1]);
        ImGui::SameLine();
        ImGui::InputDouble("z", &input_pos[i][2]);
        ImGui::PopID();
        ImGui::PopItemWidth();
    }

    ImGui::Text("Size : ");
    ImGui::PushItemWidth(100);
    input_size = (float)this->get_config().size;
    ImGui::SliderFloat("size", &input_size, 1, 10);
    ImGui::PopItemWidth();

    ImGui::Text("Inner Color : ");
    for (int i = 0; i < 3; i++)
        input_inner_color[i] = (float)this->get_config().inner_color[i];
    ImGui::ColorEdit3("inner", input_inner_color);
    for (int i = 0; i < 3; i++)
        input_outer_color[i] = (float)this->get_config().outer_color[i];
    ImGui::ColorEdit3("outer", input_outer_color);

    if (ImGui::Button("Toggle wireframe"))
    {
        this->get_config().wireframe = !this->get_config().wireframe;
    }

    triangle
        ntr;
    ntr.set_vertices(vec3d(input_pos[0][0], input_pos[0][1], input_pos[0][2]),
                     vec3d(input_pos[1][0], input_pos[1][1], input_pos[1][2]),
                     vec3d(input_pos[2][0], input_pos[2][1], input_pos[2][2]));
    this->set_name(input_name);
    this->set_geometry(ntr);
    this->get_config().inner_color[0] = input_inner_color[0];
    this->get_config().inner_color[1] = input_inner_color[1];
    this->get_config().inner_color[2] = input_inner_color[2];
    this->get_config().outer_color[0] = input_outer_color[0];
    this->get_config().outer_color[1] = input_outer_color[1];
    this->get_config().outer_color[2] = input_outer_color[2];
    this->get_config().size = input_size;
}
template<>
void prg<quad>::render_ui()
{
    property_render::render_ui();
    ImGui::Text("Quad configuration");

    // inputs.
    static char input_name[64];
    static double input_pos[4][3];
    static float input_size;
    static float input_inner_color[3];
    static float input_outer_color[3];

    ImGui::Text("Name : ");
    ImGui::PushItemWidth(50);
    strncpy(input_name, this->get_name().c_str(), 64);
    ImGui::InputText("", input_name, 64);
    ImGui::PopItemWidth();

    ImGui::Text("Position : ");
    for (int i = 0; i < 4; i++)
    {
        ImGui::PushItemWidth(100);
        input_pos[i][0] = this->get_geometry().get_vertices().at(i)[0];
        input_pos[i][1] = this->get_geometry().get_vertices().at(i)[1];
        input_pos[i][2] = this->get_geometry().get_vertices().at(i)[2];
        ImGui::PushID(i);
        ImGui::InputDouble("x", &input_pos[i][0]);
        ImGui::SameLine();
        ImGui::InputDouble("y", &input_pos[i][1]);
        ImGui::SameLine();
        ImGui::InputDouble("z", &input_pos[i][2]);
        ImGui::PopID();
        ImGui::PopItemWidth();
    }

    ImGui::Text("Size : ");
    ImGui::PushItemWidth(100);
    input_size = (float)this->get_config().size;
    ImGui::SliderFloat("size", &input_size, 1, 10);
    ImGui::PopItemWidth();

    ImGui::Text("Inner Color : ");
    for (int i = 0; i < 3; i++)
        input_inner_color[i] = (float)this->get_config().inner_color[i];
    ImGui::ColorEdit3("inner", input_inner_color);
    for (int i = 0; i < 3; i++)
        input_outer_color[i] = (float)this->get_config().outer_color[i];
    ImGui::ColorEdit3("outer", input_outer_color);

    if (ImGui::Button("Toggle wireframe"))
    {
        this->get_config().wireframe = !this->get_config().wireframe;
    }

    quad
        nqd;
    nqd.set_vertices(vec3d(input_pos[0][0], input_pos[0][1], input_pos[0][2]),
                     vec3d(input_pos[1][0], input_pos[1][1], input_pos[1][2]),
                     vec3d(input_pos[2][0], input_pos[2][1], input_pos[2][2]),
                     vec3d(input_pos[3][0], input_pos[3][1], input_pos[3][2]));
    this->set_name(input_name);
    this->set_geometry(nqd);
    this->get_config().inner_color[0] = input_inner_color[0];
    this->get_config().inner_color[1] = input_inner_color[1];
    this->get_config().inner_color[2] = input_inner_color[2];
    this->get_config().outer_color[0] = input_outer_color[0];
    this->get_config().outer_color[1] = input_outer_color[1];
    this->get_config().outer_color[2] = input_outer_color[2];
    this->get_config().size = input_size;
}
template<>
void prg<box>::render_ui()
{
    property_render::render_ui();
    ImGui::Text("Box configuration");

    // inputs.
    static char input_name[64];
    static double input_min[3];
    static double input_max[3];
    static float input_size;
    static float input_inner_color[3];
    static float input_outer_color[3];

    ImGui::Text("Name : ");
    ImGui::PushItemWidth(50);
    strncpy(input_name, this->get_name().c_str(), 64);
    ImGui::InputText("", input_name, 64);
    ImGui::PopItemWidth();

    ImGui::Text("Position : ");
    // min
    {
        ImGui::Text("Min : ");
        ImGui::PushItemWidth(100);
        input_min[0] = this->get_geometry().get_xmin();
        input_min[1] = this->get_geometry().get_ymin();
        input_min[2] = this->get_geometry().get_zmin();
        ImGui::PushID(0);
        ImGui::InputDouble("x", &input_min[0]);
        ImGui::SameLine();
        ImGui::InputDouble("y", &input_min[1]);
        ImGui::SameLine();
        ImGui::InputDouble("z", &input_min[2]);
        ImGui::PopID();
        ImGui::PopItemWidth();
    }
    // max
    {
        ImGui::Text("Max : ");
        ImGui::PushItemWidth(100);
        input_max[0] = this->get_geometry().get_xmax();
        input_max[1] = this->get_geometry().get_ymax();
        input_max[2] = this->get_geometry().get_zmax();
        ImGui::PushID(1);
        ImGui::InputDouble("x", &input_max[0]);
        ImGui::SameLine();
        ImGui::InputDouble("y", &input_max[1]);
        ImGui::SameLine();
        ImGui::InputDouble("z", &input_max[2]);
        ImGui::PopID();
        ImGui::PopItemWidth();
    }

    ImGui::Text("Border width : ");
    ImGui::PushItemWidth(100);
    input_size = (float)this->get_config().size;
    ImGui::SliderFloat("size", &input_size, 1, 10);
    ImGui::PopItemWidth();

    ImGui::Text("Face Color : ");
    for (int i = 0; i < 3; i++)
        input_inner_color[i] = (float)this->get_config().inner_color[i];
    ImGui::ColorEdit3("inner", input_inner_color);
    ImGui::Text("Boundary Color : ");
    for (int i = 0; i < 3; i++)
        input_outer_color[i] = (float)this->get_config().outer_color[i];
    ImGui::ColorEdit3("outer", input_outer_color);

    if (ImGui::Button("Toggle wireframe"))
    {
        this->get_config().wireframe = !this->get_config().wireframe;
    }

    box
        nbox;
    nbox.set_xmin(input_min[0]);
    nbox.set_ymin(input_min[1]);
    nbox.set_zmin(input_min[2]);
    nbox.set_xmax(input_max[0]);
    nbox.set_ymax(input_max[1]);
    nbox.set_zmax(input_max[2]);
    this->set_name(input_name);
    this->set_geometry(nbox);
    this->get_config().inner_color[0] = input_inner_color[0];
    this->get_config().inner_color[1] = input_inner_color[1];
    this->get_config().inner_color[2] = input_inner_color[2];
    this->get_config().outer_color[0] = input_outer_color[0];
    this->get_config().outer_color[1] = input_outer_color[1];
    this->get_config().outer_color[2] = input_outer_color[2];
    this->get_config().size = input_size;
}
template<>
void prg<mesh2>::render_ui()
{
    property_render::render_ui();
    ImGui::Text("Mesh2 configuration");

    // inputs.
    static char input_name[64];
    static float input_size;
    static float input_color[3];

    ImGui::Text("Name : ");
    ImGui::PushItemWidth(50);
    strncpy(input_name, this->get_name().c_str(), 64);
    ImGui::InputText("", input_name, 64);
    ImGui::PopItemWidth();

    ImGui::Text("Border width : ");
    ImGui::PushItemWidth(100);
    input_size = (float)this->get_config().size;
    ImGui::SliderFloat("size", &input_size, 1, 10);
    ImGui::PopItemWidth();

    ImGui::Text("Color : ");
    for (int i = 0; i < 3; i++)
        input_color[i] = (float)this->get_config().inner_color[i];
    ImGui::ColorEdit3("color", input_color);

    if (ImGui::Button("Toggle wireframe"))
    {
        this->get_config().wireframe = !this->get_config().wireframe;
    }

    this->set_name(input_name);
    this->get_config().inner_color[0] = input_color[0];
    this->get_config().inner_color[1] = input_color[1];
    this->get_config().inner_color[2] = input_color[2];
    this->get_config().size = input_size;
}
template<>
void prg<mesh3>::render_ui()
{
    property_render::render_ui();
    ImGui::Text("Mesh3 configuration");

    // inputs.
    static char input_name[64];
    static float input_size;
    static float input_inner_color[3];
    static float input_outer_color[3];

    ImGui::Text("Name : ");
    ImGui::PushItemWidth(50);
    strncpy(input_name, this->get_name().c_str(), 64);
    ImGui::InputText("", input_name, 64);
    ImGui::PopItemWidth();

    ImGui::Text("Border width : ");
    ImGui::PushItemWidth(100);
    input_size = (float)this->get_config().size;
    ImGui::SliderFloat("size", &input_size, 1, 10);
    ImGui::PopItemWidth();

    ImGui::Text("Face Color : ");
    for (int i = 0; i < 3; i++)
        input_inner_color[i] = (float)this->get_config().inner_color[i];
    ImGui::ColorEdit3("inner", input_inner_color);
    ImGui::Text("Boundary Color : ");
    for (int i = 0; i < 3; i++)
        input_outer_color[i] = (float)this->get_config().outer_color[i];
    ImGui::ColorEdit3("outer", input_outer_color);

    if (ImGui::Button("Toggle wireframe"))
    {
        this->get_config().wireframe = !this->get_config().wireframe;
    }

    this->set_name(input_name);
    this->get_config().inner_color[0] = input_inner_color[0];
    this->get_config().inner_color[1] = input_inner_color[1];
    this->get_config().inner_color[2] = input_inner_color[2];
    this->get_config().outer_color[0] = input_outer_color[0];
    this->get_config().outer_color[1] = input_outer_color[1];
    this->get_config().outer_color[2] = input_outer_color[2];
    this->get_config().size = input_size;
}
template<>
void prg<mesh4>::render_ui()
{
    property_render::render_ui();
    ImGui::Text("Mesh4 configuration");

    // inputs.
    static char input_name[64];
    static float input_size;
    static float input_inner_color[3];
    static float input_outer_color[3];

    ImGui::Text("Name : ");
    ImGui::PushItemWidth(50);
    strncpy(input_name, this->get_name().c_str(), 64);
    ImGui::InputText("", input_name, 64);
    ImGui::PopItemWidth();

    ImGui::Text("Border width : ");
    ImGui::PushItemWidth(100);
    input_size = (float)this->get_config().size;
    ImGui::SliderFloat("size", &input_size, 1, 10);
    ImGui::PopItemWidth();

    ImGui::Text("Face Color : ");
    for (int i = 0; i < 3; i++)
        input_inner_color[i] = (float)this->get_config().inner_color[i];
    ImGui::ColorEdit3("inner", input_inner_color);
    ImGui::Text("Boundary Color : ");
    for (int i = 0; i < 3; i++)
        input_outer_color[i] = (float)this->get_config().outer_color[i];
    ImGui::ColorEdit3("outer", input_outer_color);

    if (ImGui::Button("Toggle wireframe"))
    {
        this->get_config().wireframe = !this->get_config().wireframe;
    }

    this->set_name(input_name);
    this->get_config().inner_color[0] = input_inner_color[0];
    this->get_config().inner_color[1] = input_inner_color[1];
    this->get_config().inner_color[2] = input_inner_color[2];
    this->get_config().outer_color[0] = input_outer_color[0];
    this->get_config().outer_color[1] = input_outer_color[1];
    this->get_config().outer_color[2] = input_outer_color[2];
    this->get_config().size = input_size;
}