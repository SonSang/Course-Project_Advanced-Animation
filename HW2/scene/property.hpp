#ifndef __CRENDERER_PROPERTY_H__
#define __CRENDERER_PROPERTY_H__

#ifdef _MSC_VER
#pragma once
#endif

#include <string>

class object;

class property
{ // property for an object.
private:
    object *owner;

public:
    property() = default;
    property(object *owner_);

    void set_owner(object *owner_);
    object *get_owner() const noexcept;

    virtual std::string type_name() const noexcept;

    virtual void render_ui(); // ui-related func.
};

#endif