#ifndef PROJECTILE_H
#define PROJECTILE_H
#include <algorithm>
#include <cmath>
#include <complex>
#include <vector>

#include "SFML/System/Vector2.hpp"

#include "../../enemy/Enemy.h"
#include "../../../helper/UUID.h"
#include "../../../helper/CollisionDetect.h"
#include "../../hit_texture/circle/CircleHitTexture.h"

class Projectile {
    std::string id = get_uuid();
    int pierce;
    int damage;
    int speed;
    float angle;
    sf::Vector2f pos;
    CircleHitTexture hitTexture;

    Projectile(const int pierce, const int damage, const int speed): angle(0) {
        this->pierce = pierce;
        this->damage = damage;
        this->speed = speed;
    }

public:
    virtual ~Projectile() = default;

    Projectile(const int pierce, const int damage, const int speed, const float posX, const float posY, const float direction) : Projectile(pierce, damage, speed) {
        this->pos.x = posX;
        this->pos.y = posY;
        this->angle = direction;
        this->hitTexture.setPosition(pos);
    }

    void updatePosition() {
        pos.x += static_cast<float>(this->speed) * std::cos(angle);
        pos.y += static_cast<float>(this->speed) * std::sin(angle);
        hitTexture.setPosition(pos);
    }

    virtual void handleEnemies(const std::vector<std::shared_ptr<Enemy>> &collided) = 0;

    [[nodiscard]] bool isColliding(const std::shared_ptr<Enemy>& toCheck) const {
        return doCirclesOverlap(*hitTexture.getCircleHitbox(), *toCheck->getHitTexture()->getCircleHitbox());
    }

    virtual bool hasActiveDisplayEffects() = 0;
    virtual void updateDisplayEffects(float deltaTime) = 0;
    virtual std::vector<std::shared_ptr<sf::Drawable>> getDisplayEffects() = 0;
    virtual std::vector<std::shared_ptr<sf::Drawable>> getCompletedDisplayEffects() = 0;

    CircleHitTexture *getHitTexture() {
        return &hitTexture;
    }

    [[nodiscard]] bool isValid() const {
        return pierce > 0 && (pos.x <= 1920 && pos.x >= 0 && pos.y <= 1080 && pos.y >= 0);
    }

    bool operator == (const Projectile &other) const {
        if(this->id == other.id) {
            return true;
        }
        return false;
    }

protected:
    std::vector<std::string> collisions;

    bool alreadyCollided(const std::shared_ptr<Enemy>& collided) {
        return std::find(collisions.begin(), collisions.end(), collided->getId()) != collisions.end();
    }

    void setPos(const sf::Vector2f& pos) {
        this->pos = pos;
    }

    [[nodiscard]] sf::Vector2f getPos() const {
        return pos;
    }

    [[nodiscard]] int getDamage() const {
        return damage;
    }

    void setPierce(const int pierce) {
        this->pierce = pierce;
    }

    [[nodiscard]] int getPierce() const {
        return pierce;
    }

    void setAngle(const float angle) {
        this->angle = angle;
    }

    [[nodiscard]] float getAngle() const {
        return angle;
    }
};



#endif //PROJECTILE_H
