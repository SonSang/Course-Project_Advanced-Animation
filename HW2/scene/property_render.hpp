#ifndef __CRENDERER_PROPERTY_RENDER_H__
#define __CRENDERER_PROPERTY_RENDER_H__

#ifdef _MSC_VER
#pragma once
#endif

#include "property.hpp"

// This property gives an object information how to render it.
// User can modify selected object's apperance with this property.
class property_render : public property
{
private:
    std::string name;
    bool valid = true;
    //std::shared_ptr<scene_graph_node> render_ptr;
public:
    property_render() = default;
    property_render(const std::string &name_);

    const std::string &get_name() const noexcept;
    void set_name(const std::string &name);
    bool is_valid() const noexcept;
    void set_valid(bool v);
    virtual void render() const noexcept;
    virtual void render_ui();
    /*property_render() = default;
			property_render(std::shared_ptr<scene_graph_node> render_ptr);
			void set_render_ptr(std::shared_ptr<scene_graph_node> render_ptr) noexcept;
			std::shared_ptr<scene_graph_node> get_render_ptr() noexcept;
			virtual std::string type_name() const noexcept;*/
};

#endif