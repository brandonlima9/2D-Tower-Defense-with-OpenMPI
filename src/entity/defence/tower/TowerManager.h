#ifndef TOWERMANAGER_H
#define TOWERMANAGER_H
#include <list>
#include <vector>
#include <memory>
#include "Tower.h"
#include "../projectile/ProjectileManager.h"
#include <SFML/Graphics.hpp>

#include "../../../pressure/TowerPressureDecrypt.h"


class TowerManager {
    ProjectileManager projectileManager;
    std::list<std::shared_ptr<Tower>> towers;
    int accumulatedPressure = 0;
    std::chrono::steady_clock::time_point accumulationStart = std::chrono::steady_clock::now();
    sf::Clock animationClock;

public:
    void update() {
        projectileManager.update();
        const float deltaTime = animationClock.restart().asSeconds();
        for (const auto &tower: towers) {
            tower->getHitTexture()->getAnimDisplayEntity()->update(deltaTime);
        }
    }

    void addTower(const std::shared_ptr<Tower> &newTower) {
        towers.push_back(newTower);
    }

    void addTowers(const std::vector<std::shared_ptr<Tower>> &newTowers) {
        for (const auto &tower: newTowers) {
            towers.push_back(tower);
        }
    }

    void removeTower(const std::shared_ptr<Tower> &tower) {
        towers.remove(tower);
    }

    [[nodiscard]] std::vector<std::shared_ptr<Tower>> getTowers() const {
        return {towers.begin(), towers.end()};
    }

    void enemyInteractions(std::vector<std::shared_ptr<Enemy>> enemies) {
        for (const auto &tower: towers) {
            const auto &newProjectiles = tower->shootProjectile(enemies);
            if (!newProjectiles.empty()) {
                accumulatedPressure += static_cast<int>(newProjectiles.size());
                for (auto &projectile: newProjectiles) {
                    projectileManager.addProjectile(projectile);
                }
            }
        }
        const std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        const auto timeDiff = std::chrono::duration_cast<std::chrono::seconds>(end - accumulationStart).count();
        if (timeDiff >= 1) {
            addToAccumRate(accumulatedPressure);
            accumulatedPressure = 0;
            accumulationStart = std::chrono::steady_clock::now();
        }
        projectileManager.enemyInteractions(enemies);
    }

    [[nodiscard]] std::vector<Projectile *> getActiveProjectiles() const {
        return projectileManager.getActiveProjectiles();
    }

    [[nodiscard]] std::vector<Projectile *> getInactiveProjectiles() const {
        return projectileManager.getInactiveProjectiles();
    }

    [[nodiscard]] std::vector<std::shared_ptr<Projectile>> getUndrawnProjectiles() {
        return projectileManager.getUndrawnProjectiles();
    }

    [[nodiscard]] std::vector<std::shared_ptr<sf::Drawable>> getDisplayEffects() const {
        return projectileManager.getDisplayEffects();
    }

    [[nodiscard]] std::vector<std::shared_ptr<sf::Drawable>> getCompletedDisplayEffects() const {
        return projectileManager.getCompletedDisplayEffects();
    }

    void removeInactiveProjectiles() {
        return projectileManager.removeInactiveProjectiles();
    }
};

#endif // TOWERMANAGER_H
