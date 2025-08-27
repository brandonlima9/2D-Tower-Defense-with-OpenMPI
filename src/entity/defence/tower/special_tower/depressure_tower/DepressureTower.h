#ifndef DEPRESSURETOWER_H
#define DEPRESSURETOWER_H
#include <memory>
#include <vector>

#include "SFML/Graphics/Color.hpp"
#include "SFML/System/Vector2.hpp"
#include "../BaseTower.h"
#include "../../Tower.h"
#include "../../../../../texture/TowerSpriteInjector.h"

class Projectile;

class DepressureTower final : public BaseTower {
    bool alreadyIncreasedCore = false;

public:
    explicit DepressureTower(const std::shared_ptr<TowerSpriteInjector> &towerSpriteInjector,
                             const sf::Vector2f &position)
        : BaseTower(towerSpriteInjector->createDepressIdleHitTexture(), position, 0, 50, 0.f, 0,
                    "Depressure", sf::Color::White) {
    }

        std::vector<std::shared_ptr<Projectile>> shootProjectile(std::vector<std::shared_ptr<Enemy>>& enemies) override {
            if (!alreadyIncreasedCore) {
                setActiveCoresTo(activeCores + 1, currentNodes);
                alreadyIncreasedCore = true;
            }
            return {};
        }

    protected:
        std::shared_ptr<Tower> copy() override {
            return std::make_shared<DepressureTower>(*this);
        }

        std::vector<std::shared_ptr<Projectile>> generateProjectiles(const double angle) override {
            return {};
        }

    std::string getPressurePattern() override {
        return R"~(0)~";
    }
};


#endif //DEPRESSURETOWER_H
