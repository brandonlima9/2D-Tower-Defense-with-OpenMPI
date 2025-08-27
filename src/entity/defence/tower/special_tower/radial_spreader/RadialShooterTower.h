#ifndef RADIALSHOOTERTOWER_H
#define RADIALSHOOTERTOWER_H

#include "../BaseTower.h"
#include "../../Tower.h"
#include "../../../projectile/tower_projectile/basic/BasicProjectile.h"
#include "../../../../../texture/TowerSpriteInjector.h"

class RadialShooterTower final : public BaseTower {
    std::shared_ptr<TowerSpriteInjector> towerSpriteInjector;
    double bulletSpreadAngleDeg = 60;

public:
    explicit RadialShooterTower(const std::shared_ptr<TowerSpriteInjector> &towerSpriteInjector,
                                const sf::Vector2f &position)
        : BaseTower(towerSpriteInjector->createRadialMinIdleHitTexture(), position, 100, 370, 1.5f, 100, "Radial",
                    sf::Color(252, 152, 3)) {
        this->towerSpriteInjector = towerSpriteInjector;
    }

protected:
    std::shared_ptr<Tower> copy() override {
        return std::make_shared<RadialShooterTower>(*this);
    }

    std::pair<double, bool> calculateAimAngle(std::vector<std::shared_ptr<Enemy>> &enemies) const override {
        bool enemyFound = false;

        for (const auto &enemy: enemies) {
            if (doCirclesOverlap(*getRangeIndicator(), *enemy->getHitTexture()->getCircleHitbox())) {
                enemyFound = true;
                break;
            }
        }
        return {0, enemyFound};
    }

    std::vector<std::shared_ptr<Projectile>> generateProjectiles(const double angle) override {
        const int shots = static_cast<int>(360 / bulletSpreadAngleDeg);
        std::vector<std::shared_ptr<Projectile>> projectiles;
        const float towerCenterX = getPosition().x;
        const float towerCenterY = getPosition().y;

        for (int i = 0; i < shots; i++) {
            constexpr double PI = 3.1415926535897932384626433;
            const double bulletAngleRads = angle + i * (bulletSpreadAngleDeg * PI / 180);
            projectiles.push_back(
                std::make_shared<BasicProjectile>(
                    BasicProjectile(towerCenterX, towerCenterY, static_cast<float>(bulletAngleRads))));
        }

        return projectiles;
    }

    void upgradeTower(const int upgradeValue) override {
        if (upgradeValue == 3) {
            //lvl 3 bullet spread tightest
            bulletSpreadAngleDeg = 20;
            setDamage(300);
            setAttackSpeed(1.f);
            const auto &maxTowerSprite = towerSpriteInjector->createRadialMaxIdleSprite();
            const auto &maxTowerSpriteSize = maxTowerSprite->getFrameSize();
            getHitTexture()->getAnimDisplayEntity()->setTexture(maxTowerSprite->getSprite()->getTexture(),
                                                                maxTowerSpriteSize.x, maxTowerSpriteSize.y,
                                                                maxTowerSprite->getFrameCount(),
                                                                maxTowerSprite->getFrameTime());
        } else if (upgradeValue == 2) {
            //lvl 2 bullet spread is tighter than 1
            bulletSpreadAngleDeg = 30;
            setDamage(200);
            const auto &midTowerSprite = towerSpriteInjector->createRadialMidIdleSprite();
            const auto &midTowerSpriteSize = midTowerSprite->getFrameSize();
            getHitTexture()->getAnimDisplayEntity()->setTexture(midTowerSprite->getSprite()->getTexture(),
                                                                midTowerSpriteSize.x, midTowerSpriteSize.y,
                                                                midTowerSprite->getFrameCount(),
                                                                midTowerSprite->getFrameTime());
        }
    }

    std::string getPressurePattern() override {
        return R"~(0)~";
    }
};


#endif //RADIALSHOOTERTOWER_H
