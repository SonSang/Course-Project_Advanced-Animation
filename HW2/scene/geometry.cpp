#include "geometry.hpp"
#include <algorithm>

// point
point::point(const vec3d &pt)
{
    this->point_ = pt;
}
const vec3d &point::get_point() const noexcept
{
    return this->point_;
}
void point::set_point(const vec3d &pt)
{
    this->point_ = pt;
}
void point::set_point(double x, double y, double z)
{
    this->point_.set(x, y, z);
}

// line
const vec3d &line::get_beg() const noexcept
{
    return this->beg;
}
const vec3d &line::get_end() const noexcept
{
    return this->end;
}
void line::set_beg(const vec3d &v)
{
    this->beg = v;
}
void line::set_end(const vec3d &v)
{
    this->end = v;
}
void line::set_beg(double x, double y, double z)
{
    this->beg.set(x, y, z);
}
void line::set_end(double x, double y, double z)
{
    this->end.set(x, y, z);
}

// triangle
const std::array<vec3d, 3> &triangle::get_vertices() const noexcept
{
    return this->vertices;
}
void triangle::set_vertices(const vec3d &a, const vec3d &b, const vec3d &c)
{
    this->vertices = {a, b, c};
}
void triangle::set_vertices(const std::array<vec3d, 3> &vertices)
{
    this->vertices = vertices;
}

// quad
const std::array<vec3d, 4> &quad::get_vertices() const noexcept
{
    return this->vertices;
}
void quad::set_vertices(const vec3d &a, const vec3d &b, const vec3d &c, const vec3d &d)
{
    this->vertices = {a, b, c, d};
}
void quad::set_vertices(const std::array<vec3d, 4> &vertices)
{
    this->vertices = vertices;
}

// sphere
const vec3d &sphere::get_cen() const noexcept
{
    return this->cen;
}
double sphere::get_rad() const noexcept
{
    return this->rad;
}
void sphere::set_cen(const vec3d &c) noexcept
{
    this->cen = c;
}
void sphere::set_rad(double r) noexcept
{
    this->rad = r;
}

// box
quad box::get_face(int i) const
{
    quad
        ret;
    vec3d
        a,
        b, c, d;
    if (i == 0)
    {
        a.set(get_xmin(), get_ymin(), get_zmax());
        b.set(get_xmax(), get_ymin(), get_zmax());
        c.set(get_xmax(), get_ymax(), get_zmax());
        d.set(get_xmin(), get_ymax(), get_zmax());
        ret.set_vertices({a, b, c, d});
    }
    else if (i == 1)
    {
        a.set(get_xmin(), get_ymax(), get_zmax());
        b.set(get_xmax(), get_ymax(), get_zmax());
        c.set(get_xmax(), get_ymax(), get_zmin());
        d.set(get_xmin(), get_ymax(), get_zmin());
        ret.set_vertices({a, b, c, d});
    }
    else if (i == 2)
    {
        a.set(get_xmax(), get_ymax(), get_zmax());
        b.set(get_xmax(), get_ymin(), get_zmax());
        c.set(get_xmax(), get_ymin(), get_zmin());
        d.set(get_xmax(), get_ymax(), get_zmin());
        ret.set_vertices({a, b, c, d});
    }
    else if (i == 3)
    {
        a.set(get_xmin(), get_ymin(), get_zmax());
        b.set(get_xmin(), get_ymin(), get_zmin());
        c.set(get_xmax(), get_ymin(), get_zmin());
        d.set(get_xmax(), get_ymin(), get_zmax());
        ret.set_vertices({a, b, c, d});
    }
    else if (i == 4)
    {
        a.set(get_xmin(), get_ymax(), get_zmax());
        b.set(get_xmin(), get_ymax(), get_zmin());
        c.set(get_xmin(), get_ymin(), get_zmin());
        d.set(get_xmin(), get_ymin(), get_zmax());
        ret.set_vertices({a, b, c, d});
    }
    else
    {
        a.set(get_xmin(), get_ymin(), get_zmin());
        b.set(get_xmin(), get_ymax(), get_zmin());
        c.set(get_xmax(), get_ymax(), get_zmin());
        d.set(get_xmax(), get_ymin(), get_zmin());
        ret.set_vertices({a, b, c, d});
    }
    return ret;
}
void box::set_xmin(double v)
{
    this->xmin = v;
}
void box::set_xmax(double v)
{
    this->xmax = v;
}
void box::set_ymin(double v)
{
    this->ymin = v;
}
void box::set_ymax(double v)
{
    this->ymax = v;
}
void box::set_zmin(double v)
{
    this->zmin = v;
}
void box::set_zmax(double v)
{
    this->zmax = v;
}
double box::get_xmin() const noexcept
{
    return this->xmin;
}
double box::get_xmax() const noexcept
{
    return this->xmax;
}
double box::get_ymin() const noexcept
{
    return this->ymin;
}
double box::get_ymax() const noexcept
{
    return this->ymax;
}
double box::get_zmin() const noexcept
{
    return this->zmin;
}
double box::get_zmax() const noexcept
{
    return this->zmax;
}

// mesh2
const std::vector<vec3d> &mesh2::get_vertices() const noexcept
{
    return this->vertices;
}
void mesh2::set_vertices(const std::vector<vec3d> &vertices) noexcept
{
    this->vertices = vertices;
}
void mesh2::add_vertex(const vec3d &vertex) noexcept
{
    this->vertices.push_back(vertex);
}

// mesh3
int mesh3::has_vertex(const vec3d &v) const noexcept
{
    auto
        find = std::find(this->vertices.begin(), this->vertices.end(), v);
    if (find == this->vertices.end())
        return -1;
    else
        return (int)(find - this->vertices.begin());
}
const std::vector<vec3d> &mesh3::get_vertices() const noexcept
{
    return this->vertices;
}
const std::vector<mesh3::index3> &mesh3::get_indices() const noexcept
{
    return this->indices;
}
void mesh3::set_vertices(const std::vector<vec3d> &vertices) noexcept
{
    this->vertices = vertices;
}
void mesh3::set_indices(const std::vector<index3> &indices) noexcept
{
    this->indices = indices;
}
void mesh3::add_vertex(const vec3d &v, bool multiplicity_check) noexcept
{
    if (!multiplicity_check || has_vertex(v) == -1)
        this->vertices.push_back(v);
}
void mesh3::add_triangle(const vec3d &a, const vec3d &b, const vec3d &c) noexcept
{
    int
        a_index = has_vertex(a),
        b_index = has_vertex(b),
        c_index = has_vertex(c);
    if (a_index == -1)
    {
        a_index = (int)this->vertices.size();
        add_vertex(a, false);
    }
    if (b_index == -1)
    {
        b_index = (int)this->vertices.size();
        add_vertex(b, false);
    }
    if (c_index == -1)
    {
        c_index = (int)this->vertices.size();
        add_vertex(c, false);
    }
    add_triangle(a_index, b_index, c_index);
}
void mesh3::add_triangle(int a_index, int b_index, int c_index) noexcept
{
    index3
        index{a_index, b_index, c_index};
    add_triangle(index);
}
void mesh3::add_triangle(const index3 &index) noexcept
{
    indices.push_back(index);
}

// mesh4
int mesh4::has_vertex(const vec3d &v) const noexcept
{
    auto
        find = std::find(this->vertices.begin(), this->vertices.end(), v);
    if (find == this->vertices.end())
        return -1;
    else
        return (int)(find - this->vertices.begin());
}
const std::vector<vec3d> &mesh4::get_vertices() const noexcept
{
    return this->vertices;
}
const std::vector<mesh4::index4> &mesh4::get_indices() const noexcept
{
    return this->indices;
}
void mesh4::set_vertices(const std::vector<vec3d> &vertices) noexcept
{
    this->vertices = vertices;
}
void mesh4::set_indices(const std::vector<index4> &indices) noexcept
{
    this->indices = indices;
}
void mesh4::add_vertex(const vec3d &v, bool multiplicity_check) noexcept
{
    if (!multiplicity_check || has_vertex(v) == -1)
        this->vertices.push_back(v);
}
void mesh4::add_quad(const vec3d &a, const vec3d &b, const vec3d &c, const vec3d &d) noexcept
{
    int
        a_index = has_vertex(a),
        b_index = has_vertex(b),
        c_index = has_vertex(c),
        d_index = has_vertex(d);
    if (a_index == -1)
    {
        a_index = (int)this->vertices.size();
        add_vertex(a, false);
    }
    if (b_index == -1)
    {
        b_index = (int)this->vertices.size();
        add_vertex(b, false);
    }
    if (c_index == -1)
    {
        c_index = (int)this->vertices.size();
        add_vertex(c, false);
    }
    if (d_index == -1)
    {
        d_index = (int)this->vertices.size();
        add_vertex(d, false);
    }
    add_quad(a_index, b_index, c_index, d_index);
}
void mesh4::add_quad(int a_index, int b_index, int c_index, int d_index) noexcept
{
    index4
        index{a_index, b_index, c_index, d_index};
    add_quad(index);
}
void mesh4::add_quad(const index4 &index) noexcept
{
    this->indices.push_back(index);
}

// mesh4normal
int mesh4normal::has_vertex_normal(const vertex_normal &vn) const noexcept
{
    auto
        find = std::find(this->vertex_normals.begin(), this->vertex_normals.end(), vn);
    if (find == this->vertex_normals.end())
        return -1;
    else
        return (int)(find - this->vertex_normals.begin());
}
const std::vector<mesh4normal::vertex_normal> &mesh4normal::get_vertex_normals() const noexcept
{
    return this->vertex_normals;
}
const std::vector<mesh4normal::index4> &mesh4normal::get_indices() const noexcept
{
    return this->indices;
}
void mesh4normal::set_vertex_normals(const std::vector<vertex_normal> &vertex_normals) noexcept
{
    this->vertex_normals = vertex_normals;
}
void mesh4normal::set_indices(const std::vector<index4> &indices) noexcept
{
    this->indices = indices;
}
void mesh4normal::add_vertex_normal(const vertex_normal &vn, bool multiplicity_check) noexcept
{
    if (!multiplicity_check || has_vertex_normal(vn) == -1)
        this->vertex_normals.push_back(vn);
}
void mesh4normal::add_quad(const vertex_normal &a, const vertex_normal &b, const vertex_normal &c, const vertex_normal &d) noexcept
{
    int
        a_index = has_vertex_normal(a),
        b_index = has_vertex_normal(b),
        c_index = has_vertex_normal(c),
        d_index = has_vertex_normal(d);
    if (a_index == -1)
    {
        a_index = (int)this->vertex_normals.size();
        add_vertex_normal(a, false);
    }
    if (b_index == -1)
    {
        b_index = (int)this->vertex_normals.size();
        add_vertex_normal(b, false);
    }
    if (c_index == -1)
    {
        c_index = (int)this->vertex_normals.size();
        add_vertex_normal(c, false);
    }
    if (d_index == -1)
    {
        d_index = (int)this->vertex_normals.size();
        add_vertex_normal(d, false);
    }
    add_quad(a_index, b_index, c_index, d_index);
}
void mesh4normal::add_quad(int a_index, int b_index, int c_index, int d_index) noexcept
{
    index4
        index{a_index, b_index, c_index, d_index};
    add_quad(index);
}
void mesh4normal::add_quad(const index4 &index) noexcept
{
    this->indices.push_back(index);
}