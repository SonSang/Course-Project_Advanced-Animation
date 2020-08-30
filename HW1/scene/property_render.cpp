#include "property_render.hpp"

property_render::property_render(const std::string &name_) : name(name_)
{
}
const std::string &property_render::get_name() const noexcept
{
    return this->name;
}
void property_render::set_name(const std::string &name)
{
    this->name = name;
}
bool property_render::is_valid() const noexcept
{
    return this->valid;
}
void property_render::set_valid(bool v)
{
    this->valid = v;
}
void property_render::render() const noexcept
{
    return;
}
