#ifndef __CRENDERER_OBJECT_H__
#define __CRENDERER_OBJECT_H__

#ifdef _MSC_VER
#pragma once
#endif

#include "property.hpp"
#include "../math/transformation.hpp"

#include <string>
#include <memory>
#include <list>
#include <map>
#include <set>
#include <typeinfo>

class object
{
public:
    using uint = unsigned int;

    // spatial configuration.
    struct location
    {
        vec3d origin;
        transformation tr;
        location();
    };

private:
    std::string name;
    uint uid; // id of this particular object in [object_list]. An object must be included in particular [object_list].
    std::list<std::shared_ptr<property>> properties;
    location loc;

public:
    object() = default;
    object(const std::string &name_);

    // name, identity
    void set_name(const std::string &name_);
    const std::string &get_name() const noexcept;
    uint get_id() const noexcept;
    void set_id(uint id) noexcept;

    // property
    void add_property(std::shared_ptr<property> property_);
    void del_property(uint id);
    std::shared_ptr<property> get_property(uint id);
    // get properties with type <T> at runtime.
    template <class T>
    std::set<std::shared_ptr<T>> get_property() noexcept
    {
        std::set<std::shared_ptr<T>>
            ret;
        for (auto it = properties.begin(); it != properties.end(); it++)
        {
            if (typeid(**it) == typeid(T))
                ret.insert(std::dynamic_pointer_cast<T>(*it));
        }
        return ret;
    }
    template <class T>
    std::set<std::shared_ptr<T>> get_derived_property() noexcept
    {
        std::set<std::shared_ptr<T>>
            ret;
        for (auto it = properties.begin(); it != properties.end(); it++)
        {
            auto
                cast = std::dynamic_pointer_cast<T>(*it);
            if (cast != nullptr)
                ret.insert(cast);
        }
        return ret;
    }
    template <class T>
    std::set<uint> get_derived_property_id() noexcept
    {
        std::set<uint>
            ret;
        uint
            count = 0;
        for (auto it = properties.begin(); it != properties.end(); it++)
        {
            auto
                cast = std::dynamic_pointer_cast<T>(*it);
            if (cast != nullptr)
                ret.insert(count);
            count++;
        }
        return ret;
    }
    template <class T>
    void delete_derived_property()
    {
        std::set<uint>
            id_list = get_derived_property_id<T>();
        for (auto it = id_list.rbegin(); it != id_list.rend(); it++)
        {
            auto
                it2 = properties.begin();
            std::advance(it2, *it);
            properties.erase(it2);
        }
    }
    int property_size() const noexcept;

    // transformation
    vec3d get_loc_origin() const noexcept;
    void set_transformation(const transformation &tr) noexcept;
    const transformation &get_transformation() const noexcept;
    void translate(double x, double y, double z);
    void rotate(const vec3d &axisA, const vec3d &axisB, double radian);
};
#endif