#ifndef BASENORMALPROJECTILE_H
#define BASENORMALPROJECTILE_H
#include <memory>

#include "../Projectile.h"
#include "SFML/Graphics/Color.hpp"

class BaseNormalProjectile : public Projectile {

public:
    BaseNormalProjectile(const float posX, const float posY, const float direction,
        const int pierce, const int damage, const int speed,
        const float size, const sf::Color color) : Projectile(pierce, damage, speed, posX, posY, direction) {
        getHitTexture()->setDisplayEntityColor(color);
        getHitTexture()->setDisplayEntityRadius(size);
        getHitTexture()->setHitboxColor(sf::Color::Transparent);
        getHitTexture()->setHitboxRadius(size);
        setAngle(direction);
    }

    bool hasActiveDisplayEffects() override {
        return false;
    }

    void updateDisplayEffects(float deltaTime) override {}

    std::vector<std::shared_ptr<sf::Drawable>> getDisplayEffects() override {
        return {};
    }
    std::vector<std::shared_ptr<sf::Drawable>> getCompletedDisplayEffects() override {
        return {};
    }

protected:
    void handleEnemies(const std::vector<std::shared_ptr<Enemy>> &enemies) override {
        for (const auto &enemy : enemies) {
            if (!isValid()) {
                break;
            }
            if (!enemy->isAlive()) {
                continue;
            }
            if (!alreadyCollided(enemy)) {
                const auto enemyCollidingProjectile = doCirclesOverlap(*getHitTexture()->getCircleHitbox(), *enemy->getHitTexture()->getCircleHitbox());
                if (enemyCollidingProjectile) {
                    enemy->subtractHealth(getDamage());
                    setPierce(getPierce() - 1);
                    collisions.push_back(enemy->getId());
                }
            }
        }
    }
};



#endif //BASENORMALPROJECTILE_H
