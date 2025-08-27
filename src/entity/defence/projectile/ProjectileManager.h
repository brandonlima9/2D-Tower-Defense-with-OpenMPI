#ifndef PROJECTILEMANAGER_H
#define PROJECTILEMANAGER_H
#include <list>
#include <memory>
#include <vector>

#include "Projectile.h"
#include "../../enemy/Enemy.h"
#include "SFML/System/Clock.hpp"
#include "SFML/System/Time.hpp"

class ProjectileManager {
    std::list<std::shared_ptr<Projectile>> projectiles;
    std::list<std::shared_ptr<Projectile>> undrawnProjectiles;
    sf::Clock animationClock;

    public:
        void update() {
            const float deltaTime = animationClock.restart().asSeconds();
            for (const auto &projectile : projectiles) {
                if (projectile->isValid()) {
                    projectile->updatePosition();
                }
                if (projectile->hasActiveDisplayEffects()) {
                    projectile->updateDisplayEffects(deltaTime);
                }
            }
        }

        void enemyInteractions(const std::vector<std::shared_ptr<Enemy>>& enemies) const {
            for (const auto &projectile : projectiles) {
                if (!projectile->isValid()) {
                    continue;
                }
                projectile->handleEnemies(enemies);
            }
        }

        void addProjectile(const std::shared_ptr<Projectile>& projectile) {
            projectiles.push_back(projectile);
            undrawnProjectiles.push_back(projectile);
        }

        [[nodiscard]] std::vector<Projectile*> getActiveProjectiles() const {
            std::vector<Projectile*> active;
            for (const auto& projectile : projectiles) {
                if (projectile->isValid()) {
                    active.push_back(projectile.get());
                }
            }
            return active;
        }

        [[nodiscard]] std::vector<Projectile*> getInactiveProjectiles() const {
            std::vector<Projectile*> inactive;
            for (auto& projectile : projectiles) {
                if (!projectile->isValid()) {
                    inactive.push_back(projectile.get());
                }
            }
            return inactive;
        }

        [[nodiscard]] std::vector<std::shared_ptr<Projectile>> getUndrawnProjectiles() {
            std::vector<std::shared_ptr<Projectile>> valid;
            for (auto& projectile : undrawnProjectiles) {
                if (projectile->isValid()) {
                    valid.push_back(projectile);
                }
            }
            undrawnProjectiles.clear();
            return valid;
        }

        [[nodiscard]] std::vector<std::shared_ptr<sf::Drawable>> getDisplayEffects() const {
            std::vector<std::shared_ptr<sf::Drawable>> drawables;
            for (const auto& projectile : projectiles) {
                for (const auto& drawable : projectile->getDisplayEffects()) {
                    drawables.push_back(drawable);
                }
            }
            return drawables;
        }

        [[nodiscard]] std::vector<std::shared_ptr<sf::Drawable>> getCompletedDisplayEffects() const {
            std::vector<std::shared_ptr<sf::Drawable>> drawables;
            for (const auto& projectile : projectiles) {
                for (const auto& drawable : projectile->getCompletedDisplayEffects()) {
                    drawables.push_back(drawable);
                }
            }
            return drawables;
        }

        void removeInactiveProjectiles() {
            projectiles.remove_if([](const std::shared_ptr<Projectile> &projectile) {
                return !projectile->hasActiveDisplayEffects() && !projectile->isValid();
            });
            undrawnProjectiles.remove_if([](const std::shared_ptr<Projectile> &projectile) {
                return !projectile->hasActiveDisplayEffects() && !projectile->isValid();
            });
        }
};



#endif //PROJECTILEMANAGER_H
