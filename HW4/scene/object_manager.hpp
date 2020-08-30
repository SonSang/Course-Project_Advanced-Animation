#ifndef __CRENDERER_OBJECT_MANAGER_H__
#define __CRENDERER_OBJECT_MANAGER_H__

#ifdef _MSC_VER
#pragma once
#endif

#include "object.hpp"

class object_manager
{
public:
    using uint = object::uint;

private:
    uint next_uid;
    std::map<uint, std::shared_ptr<object>> list;

public:
    object_manager();

    void add_object(std::shared_ptr<object> object_ptr) noexcept;
    void del_object(std::shared_ptr<object> object_ptr);
    void del_object(uint id);
    std::shared_ptr<object> get_object(uint id);
    const std::shared_ptr<object> get_object_c(uint id) const;
    auto get_list_begin() -> decltype(list.begin());
    auto get_list_end() -> decltype(list.end());
    int size() const noexcept;
};

#endif