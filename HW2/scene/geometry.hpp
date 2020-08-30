#ifndef __CRENDERER_GEOMETRY_H__
#define __CRENDERER_GEOMETRY_H__

#ifdef _MSC_VER
#pragma once
#endif

#include "../math/vector.hpp"
#include <array>
#include <vector>

class geometry
{
protected:
};

class point : public geometry
{
private:
    vec3d point_;

public:
    point() = default;
    point(const vec3d &pt);
    const vec3d &get_point() const noexcept;
    void set_point(const vec3d &pt);
    void set_point(double x, double y, double z);
};

class line : public geometry
{
private:
    vec3d beg;
    vec3d end;

public:
    const vec3d &get_beg() const noexcept;
    const vec3d &get_end() const noexcept;
    void set_beg(const vec3d &v);
    void set_end(const vec3d &v);
    void set_beg(double x, double y, double z);
    void set_end(double x, double y, double z);
};

class triangle : public geometry
{
private:
    std::array<vec3d, 3> vertices;

public:
    const std::array<vec3d, 3> &get_vertices() const noexcept;
    void set_vertices(const vec3d &a, const vec3d &b, const vec3d &c);
    void set_vertices(const std::array<vec3d, 3> &vertices);
};

class quad : public geometry
{
private:
    std::array<vec3d, 4> vertices;

public:
    const std::array<vec3d, 4> &get_vertices() const noexcept;
    void set_vertices(const vec3d &a, const vec3d &b, const vec3d &c, const vec3d &d);
    void set_vertices(const std::array<vec3d, 4> &vertices);
};

class sphere : public geometry
{
private:
    vec3d cen;
    double rad;

public:
    const vec3d &get_cen() const noexcept;
    double get_rad() const noexcept;
    void set_cen(const vec3d &c) noexcept;
    void set_rad(double r) noexcept;
};

class box : public geometry
{
private:
    double xmin, xmax, ymin, ymax, zmin, zmax;

public:
    quad get_face(int i) const; // get face : i = 0 ~ 5.
    void set_xmin(double v);
    void set_xmax(double v);
    void set_ymin(double v);
    void set_ymax(double v);
    void set_zmin(double v);
    void set_zmax(double v);
    double get_xmin() const noexcept;
    double get_xmax() const noexcept;
    double get_ymin() const noexcept;
    double get_ymax() const noexcept;
    double get_zmin() const noexcept;
    double get_zmax() const noexcept;
};

// line mesh == curve
class mesh2 : public geometry
{
private:
    std::vector<vec3d> vertices;

public:
    const std::vector<vec3d> &get_vertices() const noexcept;
    void set_vertices(const std::vector<vec3d> &vertices) noexcept;
    void add_vertex(const vec3d &vertex) noexcept;
};

// triangle mesh
class mesh3 : public geometry
{
public:
    using index3 = std::array<int, 3>;

private:
    std::vector<vec3d> vertices;
    std::vector<index3> indices;

public:
    int has_vertex(const vec3d &v) const noexcept; // -1 for not found.
    const std::vector<vec3d> &get_vertices() const noexcept;
    const std::vector<index3> &get_indices() const noexcept;
    void set_vertices(const std::vector<vec3d> &vertices) noexcept;
    void set_indices(const std::vector<index3> &indices) noexcept;
    void add_vertex(const vec3d &v, bool multiplicity_check = true) noexcept;
    void add_triangle(const vec3d &a, const vec3d &b, const vec3d &c) noexcept;
    void add_triangle(int a_index, int b_index, int c_index) noexcept;
    void add_triangle(const index3 &index) noexcept;
};

// quad mesh
class mesh4 : public geometry
{
public:
    using index4 = std::array<int, 4>;

private:
    std::vector<vec3d> vertices;
    std::vector<index4> indices;

public:
    int has_vertex(const vec3d &v) const noexcept; // -1 for not found.
    const std::vector<vec3d> &get_vertices() const noexcept;
    const std::vector<index4> &get_indices() const noexcept;
    void set_vertices(const std::vector<vec3d> &vertices) noexcept;
    void set_indices(const std::vector<index4> &indices) noexcept;
    void add_vertex(const vec3d &v, bool multiplicity_check = true) noexcept;
    void add_quad(const vec3d &a, const vec3d &b, const vec3d &c, const vec3d &d) noexcept;
    void add_quad(int a_index, int b_index, int c_index, int d_index) noexcept;
    void add_quad(const index4 &index) noexcept;
};

// quad mesh with normal
class mesh4normal : public geometry
{
public:
    using vertex_normal = std::pair<vec3d, vec3d>;
    using index4 = std::array<int, 4>;

private:
    std::vector<vertex_normal> vertex_normals;
    std::vector<index4> indices;

public:
    int has_vertex_normal(const vertex_normal &vn) const noexcept; // -1 for not found.
    const std::vector<vertex_normal> &get_vertex_normals() const noexcept;
    const std::vector<index4> &get_indices() const noexcept;
    void set_vertex_normals(const std::vector<vertex_normal> &vertex_normals) noexcept;
    void set_indices(const std::vector<index4> &indices) noexcept;
    void add_vertex_normal(const vertex_normal &vn, bool multiplicity_check = true) noexcept;
    void add_quad(const vertex_normal &a, const vertex_normal &b, const vertex_normal &c, const vertex_normal &d) noexcept;
    void add_quad(int a_index, int b_index, int c_index, int d_index) noexcept;
    void add_quad(const index4 &index) noexcept;
};

#endif