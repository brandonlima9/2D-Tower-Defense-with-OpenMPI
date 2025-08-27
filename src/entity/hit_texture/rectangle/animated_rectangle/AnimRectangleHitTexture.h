#ifndef ANIMRECTANGLEHITTEXTURE_H
#define ANIMRECTANGLEHITTEXTURE_H
#include <memory>

#include "../../HitTexture.h"
#include "../../animated_sprite/AnimatedSprite.h"
#include "SFML/Graphics/RectangleShape.hpp"


class AnimRectangleHitTexture final : public HitTexture {
    std::shared_ptr<AnimatedSprite> displayEntity;
    std::shared_ptr<sf::RectangleShape> hitbox;

public:
    AnimRectangleHitTexture(const sf::Texture &texture, const int frameWidth, const int frameHeight,
                            const int frameCount,
                            const float frameTime) {
        this->displayEntity = std::make_shared<AnimatedSprite>(
            AnimatedSprite(texture, frameWidth, frameHeight, frameCount, frameTime));
        this->hitbox = std::make_shared<sf::RectangleShape>(sf::RectangleShape());
    }

    AnimRectangleHitTexture(const std::shared_ptr<sf::Sprite> &sprite, const int frameCount, const float frameTime) {
        this->displayEntity = std::make_shared<AnimatedSprite>(AnimatedSprite(sprite, frameCount, frameTime));
        this->hitbox = std::make_shared<sf::RectangleShape>(sf::RectangleShape());
    }

    [[nodiscard]] std::shared_ptr<sf::Drawable> getHitbox() const override {
        return hitbox;
    }

    [[nodiscard]] std::shared_ptr<sf::RectangleShape> getRectHitbox() const {
        return hitbox;
    }

    void setHitboxColor(const sf::Color color) const {
        hitbox->setFillColor(color);
    }

    void setHitboxHeight(const float height) const {
        auto size = hitbox->getSize();
        size.y = height;
        hitbox->setSize(size);
    }

    void setHitboxWidth(const float width) const {
        auto size = hitbox->getSize();
        size.x = width;
        hitbox->setSize(size);
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

    void setDisplayEntityTransparency(const float transparency) const {
        if (transparency > 255 || transparency < 0) {
            return;
        }
        auto currentColor = displayEntity->getSprite()->getColor();
        currentColor.a = static_cast<int>(transparency * 255);
        displayEntity->getSprite()->setColor(currentColor);
    }

    void setDisplayEntityHeight(const float height) const {
        auto size = displayEntity->getSprite()->getScale();
        size.y = height;
        displayEntity->getSprite()->setScale(size);
    }

    void setDisplayEntityWidth(const float width) const {
        auto size = displayEntity->getSprite()->getScale();
        size.x = width;
        displayEntity->getSprite()->setScale(size);
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
        hitbox->setPosition(sf::Vector2f(
            x - hitbox->getSize().x / 2,
            y - hitbox->getSize().y / 2));
    }


    void setPositionVisualOffset(const sf::Vector2f position, const float xOffsetScale,
                                 const float yOffsetScale) const {
        setPositionVisualOffset(position.x, position.y, xOffsetScale, yOffsetScale);
    }

    void setHitbox(const std::shared_ptr<sf::RectangleShape> &hitbox) {
        this->hitbox = hitbox;
    }

    void setDisplayEntity(const std::shared_ptr<AnimatedSprite> &displayEntity) {
        this->displayEntity = displayEntity;
    }
};


#endif //ANIMRECTANGLEHITTEXTURE_H
