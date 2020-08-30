#ifndef __CRENDERER_PROPERTY_RENDER_GEOMETRY_H__
#define __CRENDERER_PROPERTY_RENDER_GEOMETRY_H__

#ifdef _MSC_VER
#pragma once
#endif

#include "property_render.hpp"
#include "geometry.hpp"

template <typename T>
class property_render_geometry : public property_render
{
    static_assert(std::is_base_of<geometry, T>::value,
                  "Geometry render property must contain a subclass object of [geometry] class.");

public:
    struct config
    {
    public:
        bool border = true;
        bool wireframe = false;
        double inner_color[3] = {1, 1, 1};
        double outer_color[3] = {0, 0, 0};
        double size = 1;
    };

private:
    T geometry_;
    config config_;

public:
    property_render_geometry() = default;
    property_render_geometry(const std::string &name) : property_render(name)
    {
    }
    void set_geometry(const T &geometry_)
    {
        this->geometry_ = geometry_;
    }
    T &get_geometry() noexcept
    {
        return this->geometry_;
    }
    const T &get_geometry_c() const noexcept
    {
        return this->geometry_;
    }
    config &get_config() noexcept
    {
        return this->config_;
    }
    virtual void render() const noexcept;
    virtual void render_ui();
};

#endif