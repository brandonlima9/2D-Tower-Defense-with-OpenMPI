#ifndef BASETOWER_H
#define BASETOWER_H
#include "../Tower.h"
#include "../../../../pressure/TowerPressureDecrypt.h"


class BaseTower : public Tower {
public:
    BaseTower(const std::shared_ptr<AnimRectangleHitTexture> &animHitTexture, const sf::Vector2f &position,
              const float sizeScale, const float range, const int towerCost, const float attackSpeed, const int damage,
              const std::string &type, const sf::Color color)
        : Tower(animHitTexture, position, range, towerCost, attackSpeed, damage, type) {
        const float sizeScaler = 1.2f * sizeScale;
        getHitTexture()->setDisplayEntityColor(color);
        getHitTexture()->setDisplayEntityHeight(sizeScaler);
        getHitTexture()->setDisplayEntityWidth(sizeScaler);
        getHitTexture()->setHitboxColor(sf::Color::Transparent);
        getHitTexture()->setHitboxWidth(
            static_cast<float>(animHitTexture->getAnimDisplayEntity()->getFrameSize().x) * sizeScaler);
        getHitTexture()->setHitboxHeight(
            static_cast<float>(animHitTexture->getAnimDisplayEntity()->getFrameSize().y) * sizeScaler);
    }

    BaseTower(const std::shared_ptr<AnimRectangleHitTexture> &animHitTexture, const sf::Vector2f &position,
              const float range, const int towerCost, const float attackSpeed, const int damage,
              const std::string &type, const sf::Color color)
        : BaseTower(animHitTexture, position, 1.f, range, towerCost, attackSpeed, damage, type, color) {
    }

    std::vector<std::shared_ptr<Projectile>> shootProjectile(std::vector<std::shared_ptr<Enemy>> &enemies) override {
        const std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        const auto timeDiff = std::chrono::duration_cast<std::chrono::seconds>(end - shootStart).count();
        std::vector<std::shared_ptr<Projectile>> projectiles;
        if (static_cast<float>(timeDiff) >= getAttackSpeed()) {
            auto [aimAngle, enemyFound] = calculateAimAngle(enemies);

            if (enemyFound) {
                for (const auto &projectile: generateProjectiles(aimAngle)) {
                    projectiles.push_back(projectile);
                }

                addToDecrypt(getId(), getPressurePattern());
                shootStart = std::chrono::steady_clock::now();
            }
        }
        return projectiles;
    }

protected:
    std::chrono::steady_clock::time_point shootStart = std::chrono::steady_clock::now();

    virtual std::pair<double, bool> calculateAimAngle(std::vector<std::shared_ptr<Enemy>> &enemies) const {
        double aimAngle = 0;
        double closestDistance = 10000;
        bool enemyFound = false;
        const float towerCenterX = getPosition().x;
        const float towerCenterY = getPosition().y;

        for (const auto &enemy: enemies) {
            const double distance = std::sqrt(std::pow(towerCenterX - enemy->getPosition().position.x, 2)
                                              + std::pow(towerCenterY - enemy->getPosition().position.y, 2));
            if (distance < closestDistance && doCirclesOverlap(*getRangeIndicator(),
                                                               *enemy->getHitTexture()->getCircleHitbox())) {
                closestDistance = distance;
                aimAngle = std::atan2(enemy->getPosition().position.y - towerCenterY,
                                      enemy->getPosition().position.x - towerCenterX);
                enemyFound = true;
            }
        }
        return {aimAngle, enemyFound};
    }

    void upgradeTower(int upgradeValue) override {
    }

    virtual std::vector<std::shared_ptr<Projectile>> generateProjectiles(double angle) = 0;

    virtual std::string getPressurePattern() = 0;
};


#endif //BASETOWER_H
