#ifndef THICKLINE_H
#define THICKLINE_H
#include <cmath>
#include <stdexcept>
#include <vector>

#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/VertexArray.hpp"
#include "SFML/System/Vector2.hpp"


using point_set = std::vector<sf::Vector2f>;

class ThickLine {
    float THICKNESS = 2.5;
    float DOT_SIZE = 5.f;
    sf::Color COLOR = sf::Color::White;

    float _thickness;
    sf::Color _color;
    sf::VertexArray _shape;
    int _last_dot;

public:
    ThickLine(const point_set &pts, const float thickness, const sf::Color color)
        : _thickness{thickness} , _color{color} , _shape{sf::PrimitiveType::TriangleStrip} , _last_dot{-2} {
        renew_shape(pts);
    }

    explicit ThickLine(const point_set &pts) : ThickLine(pts, 7.5, sf::Color::White) {}

    ThickLine() : ThickLine(std::vector<sf::Vector2f>()) {}

    void renew_shape(const point_set &pts) {
        _last_dot = -2;
        const auto pts_count = pts.size();
        _shape = sf::VertexArray(sf::PrimitiveType::TriangleStrip, 0);
        for (auto i = 0; i < static_cast<int>(pts_count); ++i) {
            add_point(pts[i]);
        }
    }

    static sf::Vector2f unit_vector(const sf::Vector2f v) {
        return v / std::sqrt(v.x * v.x + v.y * v.y);
    }

    static sf::Vector2f mid_point(const sf::Vector2f u, const sf::Vector2f v) {
        return (u + v) / 2.f;
    }

    static sf::Vector2f normal_vector(sf::Vector2f u) {
        return {-u.y, u.x};
    }

    static bool are_colinear(const sf::Vector2f u, const sf::Vector2f v) {
        return u.x * v.y - u.y * v.x == 0.f;
    }

    static sf::Vector2f intersection_point(const sf::Vector2f dir1, sf::Vector2f pt1, const sf::Vector2f dir2, sf::Vector2f pt2) {
        // the two if-statements handle the case of one (or two) vector(s) being normal to the abscissa
        if (dir1.x == 0.f) {
            if (dir2.x == 0.f)
                throw std::logic_error("Error: trying to compute intersection between two parallel lines.");

            float ysol{dir2.y / dir2.x * pt1.x + pt2.y - dir2.y / dir2.x * pt2.x};
            return {pt1.x, ysol};
        }
        if (dir2.x == 0.f) {
            float ysol{dir1.y / dir1.x * pt2.x + pt1.y - dir1.y / dir1.x * pt1.x};
            return {pt2.x, ysol};
        }

        const float a1{dir1.y / dir1.x};
        const float b1{pt1.y - a1 * pt1.x};

        const float a2{dir2.y / dir2.x};
        const float b2{pt2.y - a2 * pt2.x};

        float xsol{(b2 - b1) / (a1 - a2)};
        return {xsol, a2 * xsol + b2};
    }

    void add_point(const sf::Vector2f f_pt) {
        // case empty _shape
        if (_last_dot < 0) {
            add_point_with_offset(f_pt, sf::Vector2f(0, _thickness / 2.f));
            return;
        }
        // case one dot in thick line
        if (_last_dot == 0) {
            const sf::Vector2f previous_point{mid_point(_shape[1].position, _shape[0].position)};
            const sf::Vector2f offset = _thickness * normal_vector(unit_vector(f_pt - previous_point)) / 2.f;
            _shape[0].position = previous_point - offset;
            _shape[1].position = previous_point + offset;
            add_point_with_offset(f_pt, offset);
            return;
        }
        /* The previous point is the mid-point between the two previous vertices.
        *  The previous direction is the direction of the previous line. */
        const sf::Vector2f prev_point1{mid_point(_shape[_last_dot - 1].position, _shape[_last_dot - 2].position)};
        const sf::Vector2f prev_point2{mid_point(_shape[_last_dot + 1].position, _shape[_last_dot].position)};

        auto [fst, snd] = make_offset(prev_point1, prev_point2, f_pt);

        if (fst != sf::Vector2f(-1, -1)) {
            _shape[_last_dot].position = fst;
            _shape[_last_dot + 1].position = snd;
        }
        //add a new pair of vertices to form a rectangle with the previous couple of vertices
        add_point_with_offset(f_pt, prev_point2 - fst);
    }

    void pop_point() {
        if (_last_dot < 0)
            return;
        if (_last_dot >= 2) {
            _shape[_last_dot + 0].position = _shape[_last_dot - 2].position;
            _shape[_last_dot + 1].position = _shape[_last_dot - 1].position;
        }
        _shape[_last_dot + 0].color = sf::Color::Transparent;
        _shape[_last_dot + 1].color = sf::Color::Transparent;

        _last_dot -= 2;
    }

    void close_line() {
        if (_last_dot < 4)
            return;

        const sf::Vector2f before_last{mid_point(_shape[_last_dot].position, _shape[_last_dot + 1].position)};
        const sf::Vector2f closing_point{mid_point(_shape[0].position, _shape[1].position)};
        const sf::Vector2f second_point{mid_point(_shape[2].position, _shape[3].position)};
        add_point(closing_point);

        auto [fst, snd]{make_offset(before_last, closing_point, second_point)};
        _shape[0].position = fst;
        _shape[1].position = snd;
        _shape[_last_dot].position = fst;
        _shape[_last_dot + 1].position = snd;
    }

    void set_color(const sf::Color c) {
        _color = c;
        for (auto i = 0; i < static_cast<int>(_shape.getVertexCount()); ++i)
            _shape[i].color = c;
    }

    void set_thickness(const float new_t) {
        const float ratio{new_t / _thickness};
        for (auto i = 0; i < static_cast<int>(_shape.getVertexCount()) - 1; i += 2) {
            const sf::Vector2f mid{mid_point(_shape[i + 1].position, _shape[i].position)};
            sf::Vector2f sep{_shape[i + 1].position - _shape[i].position};
            sep *= ratio / 2.f;
            _shape[i].position = mid - sep;
            _shape[i + 1].position = mid + sep;
        }
        _thickness = new_t;
    }

    [[nodiscard]] float get_thickness() const && {
        return _thickness;
    }

    [[nodiscard]] const float &get_thickness() const & {
        return _thickness;
    }

    void add_point_with_offset(const sf::Vector2f pt, const sf::Vector2f offset) {
        sf::Vertex v1{pt, _color, { 0.0f,  0.0f}};
        sf::Vertex v2{pt, _color, { 0.0f,  0.0f}};
        v1.position -= offset;
        v2.position += offset;

        //if some spaces is available in _shape, we use it, otherwise we append new vertices
        if (static_cast<int>(_shape.getVertexCount()) > _last_dot + 3) {
            _shape[_last_dot + 2] = v1;
            _shape[_last_dot + 3] = v2;
            for (auto i = _last_dot + 4; i < static_cast<int>(_shape.getVertexCount()) - 1; ++i) {
                _shape[i].position = _shape[i - 2].position;
                _shape[i + 1].position = _shape[i - 1].position;
            }
        } else {
            _shape.append(v1);
            _shape.append(v2);
        }
        _last_dot += 2;
    }

    [[nodiscard]] std::pair<sf::Vector2f, sf::Vector2f> make_offset(const sf::Vector2f p1, const sf::Vector2f p2,
                                                                    const sf::Vector2f p3) const {
        const sf::Vector2f prev_direction{unit_vector(p2 - p1)};
        const sf::Vector2f prev_normal{normal_vector(prev_direction)};

        const sf::Vector2f current_direction{unit_vector(p3 - p2)};
        const sf::Vector2f current_normal{normal_vector(current_direction)};

        // if the two consecutive lines are aligned, there's no need to compute the offset (as there isn't any)
        if (are_colinear(prev_direction, current_direction))
            return {sf::Vector2f(-1, -1), sf::Vector2f(-1, -1)};
        sf::Vector2f ref_pt_1{p2 - _thickness / 2.f * prev_normal};
        sf::Vector2f ref_pt_2{p2 - _thickness / 2.f * current_normal};
        sf::Vector2f offset1{intersection_point(prev_direction, ref_pt_1, current_direction, ref_pt_2)};

        ref_pt_1 = p2 + _thickness / 2.f * prev_normal;
        ref_pt_2 = p2 + _thickness / 2.f * current_normal;
        sf::Vector2f offset2{intersection_point(prev_direction, ref_pt_1, current_direction, ref_pt_2)};

        return {offset1, offset2};
    }

    sf::VertexArray getShape() {
        return _shape;
    }
};


#endif //THICKLINE_H
