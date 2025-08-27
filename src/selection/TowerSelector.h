#ifndef TOWERSELECTOR_H
#define TOWERSELECTOR_H
#include <memory>
#include <vector>

#include "../display/DisplayConsts.h"
#include "../entity/defence/tower/special_tower/bomb_tower/BombTower.h"
#include "../entity/defence/tower/special_tower/Jo_tower/JoTower.h"
#include "../entity/defence/tower/special_tower/radial_spreader/RadialShooterTower.h"
#include "../entity/defence/tower/special_tower/depressure_tower/DepressureTower.h"
#include "SFML/System/Vector2.hpp"


class Tower;

class TowerSelector {
    std::shared_ptr<TowerSpriteInjector> towerSpriteInjector;
    std::shared_ptr<ProjectileSpriteInjector> projectileSpriteInjector;
    const sf::Vector2f availIconStart = sf::Vector2f(ICON_START_X, ICON_START_Y);
    std::vector<std::shared_ptr<Tower>> availableTowers;
    std::shared_ptr<Tower> selectedTower;
    bool isTowerSelected;

    public:
        TowerSelector() {
            towerSpriteInjector = std::make_shared<TowerSpriteInjector>(TowerSpriteInjector());
            projectileSpriteInjector = std::make_shared<ProjectileSpriteInjector>(ProjectileSpriteInjector());
            std::shared_ptr<Tower> towersToMakeAvail[] {
                std::make_shared<JoTower>(JoTower(towerSpriteInjector, sf::Vector2f(0, 0))),
                std::make_shared<RadialShooterTower>(RadialShooterTower(towerSpriteInjector, sf::Vector2f(0, 0))),
                std::make_shared<BombTower>(BombTower(towerSpriteInjector, projectileSpriteInjector, sf::Vector2f(0, 0))),
                std::make_shared<DepressureTower>(DepressureTower(towerSpriteInjector, sf::Vector2f(0, 0)))
            };

            int counter = 0;
            float switch_column = 0;
            float counter_y = 0;
            for (const auto &available : towersToMakeAvail) {
                const auto& towerDisplaySize = available->getHitTexture()->getAnimDisplayEntity()->getFrameSize();
                const auto& towerDisplayScale = available->getHitTexture()->getAnimDisplayEntity()->getSprite()->getScale();
                available->setPosition(sf::Vector2f(
                    availIconStart.x + switch_column * (static_cast<float>(towerDisplaySize.x) * towerDisplayScale.x + ICON_GAP),
                    availIconStart.y + counter_y * (static_cast<float>(towerDisplaySize.y) * towerDisplayScale.y + ICON_GAP + 50)));
                availableTowers.push_back(available);
                // check if even or odd
                if (counter % 2 == 0) { // even
                    switch_column = 1;
                } else { // odd
                    switch_column = 0;
                    counter_y++;
                }
                counter++;
            }

            isTowerSelected = false;
        }

        bool attemptSelectingTower(const sf::Vector2i& mousePosition) {
            if (!isTowerSelected) {
                for (const auto& tower : availableTowers) {
                    if (tower->getHitTexture()->getRectHitbox()->getGlobalBounds().contains(sf::Vector2f(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y)))) {
                        isTowerSelected = true;
                        selectedTower = tower->deep_copy(); // make a copy of the available tower
                        selectedTower->setId(get_uuid());
                        selectedTower->getHitTexture()->setDisplayEntityTransparency(0.5);
                        return true;
                    }
                }
            }
            return false;
        }

        static std::shared_ptr<Tower> attemptSelectingPlacedTower(const sf::Vector2i& mousePosition, const std::vector<std::shared_ptr<Tower>>& towers) {
            for (const auto& tower : towers) {
                if (tower->getHitTexture()->getRectHitbox()->getGlobalBounds().contains(sf::Vector2f(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y)))) {
                    return tower;
                }
            }
            return nullptr;
        }

        void dragSelectedTower(const sf::Vector2i& mousePosition) const {
            if (isTowerSelected) {
                selectedTower->setPosition(
                    sf::Vector2f(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y)));
            }
        }

        void deselectTower() {
            isTowerSelected = false;
            selectedTower->getHitTexture()->setDisplayEntityTransparency(1);
        }

        [[nodiscard]] bool doesSelectedTowerOverlap(const std::vector<std::shared_ptr<Tower>>& towers) const {
            const bool overlapping = std::any_of(towers.begin(), towers.end(), [&](const auto& tower) {
                return doRectanglesOverlap(
                    *tower->getHitTexture()->getRectHitbox(), *selectedTower->getHitTexture()->getRectHitbox());
            });
            return overlapping;
        }

        [[nodiscard]] std::vector<std::shared_ptr<Tower>> getAvailableTowers() const {
            return availableTowers;
        }

        [[nodiscard]] bool isStillSelected() const {
            return isTowerSelected;
        }

        std::shared_ptr<Tower> getSelectedTower() {
            return selectedTower;
        }
};



#endif //TOWERSELECTOR_H
