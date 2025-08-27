#ifndef BASEEXPLOSIVEPROJECTILE_H
#define BASEEXPLOSIVEPROJECTILE_H

#include "BaseNormalProjectile.h"


class BaseExplosiveProjectile : public BaseNormalProjectile {
    std::shared_ptr<AnimCircleHitTexture> explosion;
    bool exploded = false;

public:
    BaseExplosiveProjectile(const std::shared_ptr<AnimCircleHitTexture> &explosionTexture,
                            const float posX, const float posY, const float direction,
                            const int damage, const int speed, const float explosionSize,
                            const sf::Color explosionColor,
                            const float size, const sf::Color color)
        : BaseNormalProjectile(posX, posY, direction, 1, damage, speed, size, color) {
        const auto scaleIncrease = explosionSize / 30;
        explosion = explosionTexture;
        explosion->getAnimDisplayEntity()->setDoRepeat(false);
        explosion->getAnimDisplayEntity()->getSprite()->setScale(sf::Vector2f(scaleIncrease, scaleIncrease));
        explosion->setDisplayEntityColor(explosionColor);
        explosion->setHitboxColor(sf::Color::Transparent);
        explosion->setHitboxRadius(explosionSize);
    }

    BaseExplosiveProjectile(const std::shared_ptr<AnimCircleHitTexture> &explosionTexture, const float posX,
                            const float posY, const float direction,
                            const int speed, const float explosionSize, const float size)
        : BaseExplosiveProjectile(explosionTexture, posX, posY, direction, 1, speed, explosionSize,
                                  sf::Color(255, 84, 0, 100), size, sf::Color(70, 70, 70)) {
    }

    BaseExplosiveProjectile(const std::shared_ptr<AnimCircleHitTexture> &explosionTexture, const float posX,
                            const float posY, const float direction, const int damage,
                            const int speed, const float explosionSize, const float size)
        : BaseExplosiveProjectile(explosionTexture, posX, posY, direction, damage, speed, explosionSize,
                                  sf::Color(255, 84, 0, 100), size, sf::Color(70, 70, 70)) {
    }

    BaseExplosiveProjectile(const std::shared_ptr<AnimCircleHitTexture> &explosionTexture, const float posX,
                            const float posY, const float direction)
        : BaseExplosiveProjectile(explosionTexture, posX, posY, direction, 1, 10, 50.f,
                                  sf::Color(255, 84, 0, 100), 8.f, sf::Color(70, 70, 70)) {
    }

    void handleEnemies(const std::vector<std::shared_ptr<Enemy>> &enemies) override {
        bool projectileExploded = false;
        for (const auto &enemy: enemies) {
            if (!isValid()) {
                break;
            }
            if (!enemy->isAlive()) {
                continue;
            }
            if (!alreadyCollided(enemy)) {
                const auto enemyCollidingProjectile = doCirclesOverlap(*getHitTexture()->getCircleHitbox(),
                                                                       *enemy->getHitTexture()->getCircleHitbox());
                if (enemyCollidingProjectile) {
                    projectileExploded = true;
                    enemy->subtractHealth(getDamage());
                    collisions.push_back(enemy->getId());
                    explosion->setPositionVisualOffset(getPos(), 2.f, 2.5f);
                    exploded = true;
                    break;
                }
            }
        }
        if (projectileExploded) {
            for (const auto &enemy: enemies) {
                if (!enemy->isAlive()) {
                    continue;
                }
                const auto enemyCollidingExplosion = doCirclesOverlap(*explosion->getCircleHitbox(),
                                                                      *enemy->getHitTexture()->getCircleHitbox());
                if (enemyCollidingExplosion) {
                    enemy->subtractHealth(getDamage());
                }
            }
            setPierce(0);
        }
    }

    bool hasActiveDisplayEffects() override {
        return exploded && !explosion->getAnimDisplayEntity()->isFinishedAnimation();
    }

    void updateDisplayEffects(const float deltaTime) override {
        if (exploded && !explosion->getAnimDisplayEntity()->isFinishedAnimation()) {
            explosion->getAnimDisplayEntity()->update(deltaTime);
        }
    }

    std::vector<std::shared_ptr<sf::Drawable>> getDisplayEffects() override {
        std::vector<std::shared_ptr<sf::Drawable>> drawables;
        if (exploded && !explosion->getAnimDisplayEntity()->isFinishedAnimation()) {
            drawables.push_back(explosion->getDisplayEntity());
        }
        return drawables;
    }

    std::vector<std::shared_ptr<sf::Drawable>> getCompletedDisplayEffects() override {
        std::vector<std::shared_ptr<sf::Drawable>> drawables;
        if (explosion->getAnimDisplayEntity()->isFinishedAnimation()) {
            drawables.push_back(explosion->getDisplayEntity());
        }
        return drawables;
    }

    void setExplosionColour(const sf::Color newColor) const {
        explosion->setDisplayEntityColor(newColor);
    }
};


#endif //BASEEXPLOSIVEPROJECTILE_H
