#ifndef ANIMATEDSPRITE_H
#define ANIMATEDSPRITE_H
#include <memory>

#include "SFML/Graphics/Sprite.hpp"


namespace sf {
    class RenderWindow;
    class Texture;
}

class AnimatedSprite {
    std::shared_ptr<sf::Sprite> sprite;
    int frameWidth;
    int frameHeight;
    int frameCount;
    float frameTime;
    float elapsedTime;
    int currentFrame;
    bool isDone = false;
    bool doRepeat = true;

public:
    AnimatedSprite(const sf::Texture &texture, int frameWidth, int frameHeight, const int frameCount, const float frameTime)
        : frameWidth(frameWidth), frameHeight(frameHeight), frameCount(frameCount),
          frameTime(frameTime), elapsedTime(0), currentFrame(0) {
        sprite = std::make_shared<sf::Sprite>(sf::Sprite(texture, sf::IntRect({0, 0}, {frameWidth, frameHeight})));
    }

    AnimatedSprite(const std::shared_ptr<sf::Sprite> &sprite, const int frameCount, const float frameTime)
        : frameWidth(sprite->getTextureRect().size.x), frameHeight(sprite->getTextureRect().size.y),
          frameCount(frameCount),
          frameTime(frameTime), elapsedTime(0), currentFrame(0) {
        this->sprite = sprite;
    }

    void update(const float deltaTime) {
        elapsedTime += deltaTime;
        const bool isOnFinalFrame = currentFrame == frameCount - 1;
        if (elapsedTime >= frameTime && !isDone) {
            elapsedTime = 0;
            currentFrame = (currentFrame + 1) % frameCount;
            sprite->setTextureRect(sf::IntRect({currentFrame * frameWidth, 0}, {frameWidth, frameHeight}));
            if (!doRepeat && isOnFinalFrame && currentFrame == 0) {
                isDone = true;
            }
        }
    }

    void setPosition(const float x, const float y) const {
        sprite->setPosition(sf::Vector2f(x, y));
    }

    void setDoRepeat(const bool doRepeat) {
        this->doRepeat = doRepeat;
        if (doRepeat == true) {
            this->isDone = false;
        }
    }

    void setTexture(const sf::Texture &texture, int frameWidth, int frameHeight, const int frameCount, const float frameTime) {
        sprite->setTexture(texture);
        sprite->setTextureRect(sf::IntRect({0, 0}, {frameWidth, frameHeight}));
        this->frameWidth = frameWidth;
        this->frameHeight = frameHeight;
        this->frameTime = frameTime;
        if (this->frameCount != frameCount) {
            this->frameCount = frameCount;
            this->currentFrame = 0;
            this->elapsedTime = 0;
        }

    }

    [[nodiscard]] std::shared_ptr<sf::Sprite> getSprite() const {
        return sprite;
    }

    [[nodiscard]] sf::Vector2i getFrameSize() const {
        return {frameWidth, frameHeight};
    }

    [[nodiscard]] int getFrameCount() const {
        return frameCount;
    }

    [[nodiscard]] float getFrameTime() const {
        return frameTime;
    }

    [[nodiscard]] bool isFinishedAnimation() const {
        return isDone;
    }
};


#endif //ANIMATEDSPRITE_H
