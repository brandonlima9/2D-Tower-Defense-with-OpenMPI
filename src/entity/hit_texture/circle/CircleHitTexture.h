#ifndef CIRCLEHITTEXTURE_H
#define CIRCLEHITTEXTURE_H
#include <memory>

#include "../HitTexture.h"
#include "SFML/Graphics/CircleShape.hpp"


class CircleHitTexture final : public HitTexture {
    std::shared_ptr<sf::CircleShape> displayEntity;
    std::shared_ptr<sf::CircleShape> hitbox;

public:
    CircleHitTexture() {
        this->displayEntity = std::make_shared<sf::CircleShape>(sf::CircleShape());
        this->hitbox = std::make_shared<sf::CircleShape>(sf::CircleShape());
    }

    CircleHitTexture(const sf::Texture &texture, int frameWidth, int frameHeight) {
        this->displayEntity = std::make_shared<sf::CircleShape>(sf::CircleShape());
        this->displayEntity->setTexture(&texture);
        this->displayEntity->setTextureRect(sf::IntRect({0, 0}, {frameWidth, frameHeight}));
        this->hitbox = std::make_shared<sf::CircleShape>(sf::CircleShape());
    }

    [[nodiscard]] std::shared_ptr<sf::Drawable> getHitbox() const override {
        return hitbox;
    }

    [[nodiscard]] std::shared_ptr<sf::CircleShape> getCircleHitbox() const {
        return hitbox;
    }

    void setHitboxColor(const sf::Color color) const {
        hitbox->setFillColor(color);
    }

    void setHitboxRadius(const float radius) const {
        hitbox->setRadius(radius);
    }

    [[nodiscard]] std::shared_ptr<sf::Drawable> getDisplayEntity() const override {
        return displayEntity;
    }

    [[nodiscard]] std::shared_ptr<sf::CircleShape> getCircleDisplayEntity() const {
        return displayEntity;
    }

    void setDisplayEntityColor(const sf::Color color) const {
        displayEntity->setFillColor(color);
    }

    void setDisplayEntityRadius(const float radius) const {
        displayEntity->setRadius(radius);
    }

    void setPosition(const float x, const float y) const override {
        displayEntity->setPosition(sf::Vector2f(x - displayEntity->getRadius(), y - displayEntity->getRadius()));
        hitbox->setPosition(sf::Vector2f(x - displayEntity->getRadius(), y - displayEntity->getRadius()));
    }

    void setPosition(const sf::Vector2f position) const override {
        setPosition(position.x, position.y);
    }

    void setHitbox(const std::shared_ptr<sf::CircleShape> &hitbox) {
        this->hitbox = hitbox;
    }

    void setDisplayEntity(const std::shared_ptr<sf::CircleShape> &displayEntity) {
        this->displayEntity = displayEntity;
    }
};


#endif //CIRCLEHITTEXTURE_H
