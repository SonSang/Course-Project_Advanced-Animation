#include "object.hpp"

using uint = object::uint;

// object : location
object::location::location() : origin(0, 0, 0)
{
}
// object
object::object(const std::string &name_) : name(name_)
{
}
void object::set_name(const std::string &name_)
{
    this->name = name_;
}
const std::string &object::get_name() const noexcept
{
    return this->name;
}
uint object::get_id() const noexcept
{
    return this->uid;
}
void object::set_id(uint id) noexcept
{
    this->uid = id;
}
void object::add_property(std::shared_ptr<property> property_)
{
    properties.push_back(property_);
    property_->set_owner(this);
}
void object::del_property(uint id)
{
    auto
        tar = properties.begin();
    std::advance(tar, id);
    properties.erase(tar);
}
std::shared_ptr<property> object::get_property(uint id)
{
    auto
        tar = properties.begin();
    std::advance(tar, id);
    return *tar;
}
int object::property_size() const noexcept
{
    return (int)properties.size();
}
vec3d object::get_loc_origin() const noexcept
{
    return this->loc.origin;
}
void object::set_transformation(const transformation &tr) noexcept
{
    this->loc.tr = tr;
    this->loc.origin = tr.apply(vec3d(0, 0, 0));
}
const transformation &object::get_transformation() const noexcept
{
    return this->loc.tr;
}
void object::translate(double x, double y, double z)
{
    this->loc.tr.translate(x, y, z);
    loc.origin += vec3d(x, y, z);
}
void object::rotate(const vec3d &axisA, const vec3d &axisB, double radian)
{
    this->loc.tr.rotate(axisA, axisB, radian);
    loc.origin = this->loc.tr.apply(vec3d(0, 0, 0));
}