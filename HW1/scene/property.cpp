#include "property.hpp"

property::property(object *owner_) : owner(owner_)
{
}
void property::set_owner(object *owner_)
{
    this->owner = owner_;
}
object *property::get_owner() const noexcept
{
    return this->owner;
}
std::string property::type_name() const noexcept
{
    return std::string("Basic property");
}
void property::render_ui()
{
    return;
}
