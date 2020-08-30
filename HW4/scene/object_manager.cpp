#include "object_manager.hpp"

// object_manager
object_manager::object_manager() : next_uid(0)
{
}
void object_manager::add_object(std::shared_ptr<object> object_ptr) noexcept
{
    object_ptr->set_id(next_uid++);
    this->list[object_ptr->get_id()] = object_ptr;
}
void object_manager::del_object(std::shared_ptr<object> object_ptr)
{
    del_object(object_ptr->get_id());
}
void object_manager::del_object(uint id)
{
    this->list.erase(id);
}
std::shared_ptr<object> object_manager::get_object(uint id)
{
    return this->list.at(id);
}
const std::shared_ptr<object> object_manager::get_object_c(uint id) const
{
    return this->list.at(id);
}
auto object_manager::get_list_begin() -> decltype(list.begin())
{
    return this->list.begin();
}
auto object_manager::get_list_end() -> decltype(list.end())
{
    return this->list.end();
}
int object_manager::size() const noexcept
{
    return (int)this->list.size();
}