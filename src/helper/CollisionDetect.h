#ifndef COLLISIONDETECT_H
#define COLLISIONDETECT_H

#include <cmath>
#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/RectangleShape.hpp"

inline bool doCirclesOverlap(const sf::CircleShape& circle1, const sf::CircleShape& circle2) {
    const double r1 = circle1.getRadius();
    const double x1 = circle1.getPosition().x + r1;
    const double y1 = circle1.getPosition().y + r1;
    const double r2 = circle2.getRadius();
    const double x2 = circle2.getPosition().x + r2;
    const double y2 = circle2.getPosition().y + r2;

    // get distance between the circle's centers
    // use the Pythagorean Theorem to compute the distance
    const double distX = x1 - x2;
    const double distY = y1 - y2;

    // if the distance is less than the sum of the circle's
    // radii, the circles are touching!
    if (const double distance = std::sqrt(distX * distX + distY * distY); distance <= r1 + r2) {
        return true;
    }
    return false;
}

inline bool doRectanglesOverlap(const sf::RectangleShape& rect1, const sf::RectangleShape& rect2) {
    const sf::FloatRect bounds1 = rect1.getGlobalBounds();
    const sf::FloatRect bounds2 = rect2.getGlobalBounds();
    return bounds1.findIntersection(bounds2).has_value();
}

#endif //COLLISIONDETECT_H
