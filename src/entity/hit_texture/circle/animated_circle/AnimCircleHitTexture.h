#ifndef ANIMCIRCLEHITTEXTURE_H
#define ANIMCIRCLEHITTEXTURE_H
#include <memory>

#include "../../HitTexture.h"
#include "../../animated_sprite/AnimatedSprite.h"
#include "SFML/Graphics/CircleShape.hpp"


class AnimCircleHitTexture final : public HitTexture {
    std::shared_ptr<AnimatedSprite> displayEntity;
    std::shared_ptr<sf::CircleShape> hitbox;

public:
    AnimCircleHitTexture(const sf::Texture &texture, const int frameWidth, const int frameHeight, const int frameCount,
                         const float frameTime) {
        this->displayEntity = std::make_shared<AnimatedSprite>(
            AnimatedSprite(texture, frameWidth, frameHeight, frameCount, frameTime));
        this->hitbox = std::make_shared<sf::CircleShape>(sf::CircleShape());
    }

    AnimCircleHitTexture(const std::shared_ptr<sf::Sprite> &sprite, const int frameCount, const float frameTime) {
        this->displayEntity = std::make_shared<AnimatedSprite>(AnimatedSprite(sprite, frameCount, frameTime));
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
        return displayEntity->getSprite();
    }

    [[nodiscard]] std::shared_ptr<AnimatedSprite> getAnimDisplayEntity() const {
        return displayEntity;
    }

    void setDisplayEntityColor(const sf::Color color) const {
        displayEntity->getSprite()->setColor(color);
    }

    void setPosition(const float x, const float y) const override {
        setPositionVisualOffset(x, y, 2.f, 1.5f);
    }

    void setPosition(const sf::Vector2f position) const override {
        setPosition(position.x, position.y);
    }

    void setPositionVisualOffset(const float x, const float y, const float xOffsetScale,
                                 const float yOffsetScale) const {
        const auto xOffset =
            static_cast<float>(displayEntity->getFrameSize().x) * displayEntity->getSprite()->getScale().x;
        const auto yOffset =
            static_cast<float>(displayEntity->getFrameSize().y) * displayEntity->getSprite()->getScale().y;
        displayEntity->setPosition(x - xOffset / xOffsetScale, y - yOffset / yOffsetScale);
        hitbox->setPosition(sf::Vector2f(x - hitbox->getRadius(), y - hitbox->getRadius()));
    }


    void setPositionVisualOffset(const sf::Vector2f position, const float xOffsetScale,
                                 const float yOffsetScale) const {
        setPositionVisualOffset(position.x, position.y, xOffsetScale, yOffsetScale);
    }

    void setHitbox(const std::shared_ptr<sf::CircleShape> &hitbox) {
        this->hitbox = hitbox;
    }

    void setDisplayEntity(const std::shared_ptr<AnimatedSprite> &displayEntity) {
        this->displayEntity = displayEntity;
    }
};


#endif //ANIMCIRCLEHITTEXTURE_H
