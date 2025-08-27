#ifndef JOTOWER_H
#define JOTOWER_H

#include <memory>

#include "../BaseTower.h"
#include "../../Tower.h"
#include "../../../projectile/tower_projectile/basic/BasicProjectile.h"
#include "../../../../../texture/TowerSpriteInjector.h"

class Projectile;


class JoTower final : public BaseTower {
    std::shared_ptr<TowerSpriteInjector> towerSpriteInjector;

public:
    explicit JoTower(const std::shared_ptr<TowerSpriteInjector> &towerSpriteInjector, const sf::Vector2f &position)
        : BaseTower(towerSpriteInjector->createArcherMinIdleHitTexture(), position, 250, 270, 3.f, 150, "Dart",
                    sf::Color::White) {
        this->towerSpriteInjector = towerSpriteInjector;
    }

protected:
    std::shared_ptr<Tower> copy() override {
        return std::make_shared<JoTower>(*this);
    }

    std::vector<std::shared_ptr<Projectile> > generateProjectiles(const double angle) override {
        std::vector<std::shared_ptr<Projectile> > projectiles;
        if (getUpgradeValue() == 1) {
            const float towerCenterX = getPosition().x;
            const float towerCenterY = getPosition().y;
            projectiles.push_back(
                std::make_shared<BasicProjectile>(
                    BasicProjectile(towerCenterX, towerCenterY, static_cast<float>(angle), 1, 1)));
        } else if (getUpgradeValue() == 2) {
            const float towerCenterX = getPosition().x;
            const float towerCenterY = getPosition().y;
            projectiles.push_back(
                std::make_shared<BasicProjectile>(
                    BasicProjectile(towerCenterX, towerCenterY, static_cast<float>(angle), 2, 2)));
        } else if (getUpgradeValue() == 3) {
            const float towerCenterX = getPosition().x;
            const float towerCenterY = getPosition().y;
            projectiles.push_back(
                std::make_shared<BasicProjectile>(
                    BasicProjectile(towerCenterX, towerCenterY, static_cast<float>(angle), 2, 3)));
        }

        return projectiles;
    }

    std::string getPressurePattern() override {
        return R"~(^(000))~";
    }

    void upgradeTower(const int upgradeValue) override {
        if (upgradeValue == 3) {
            setAttackSpeed(1);
            setDamage(600);
            const auto &maxTowerSprite = towerSpriteInjector->createArcherMaxIdleSprite();
            const auto &maxTowerSpriteSize = maxTowerSprite->getFrameSize();
            getHitTexture()->getAnimDisplayEntity()->setTexture(maxTowerSprite->getSprite()->getTexture(),
                                                                maxTowerSpriteSize.x, maxTowerSpriteSize.y,
                                                                maxTowerSprite->getFrameCount(),
                                                                maxTowerSprite->getFrameTime());
        } else if (upgradeValue == 2) {
            setDamage(300);
            const auto &midTowerSprite = towerSpriteInjector->createArcherMidIdleSprite();
            const auto &midTowerSpriteSize = midTowerSprite->getFrameSize();
            getHitTexture()->getAnimDisplayEntity()->setTexture(midTowerSprite->getSprite()->getTexture(),
                                                                midTowerSpriteSize.x, midTowerSpriteSize.y,
                                                                midTowerSprite->getFrameCount(),
                                                                midTowerSprite->getFrameTime());
        }
    }
};

#endif //JOTOWER_H
