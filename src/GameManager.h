#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <queue>
#include <memory>
#include <string>

#include "display/DisplayTextManager.h"
#include "entity/defence/projectile/Projectile.h"
#include "entity/defence/tower/TowerManager.h"
#include "entity/enemy/Enemy.h"
#include "entity/enemy/EnemyManager.h"
#include "entity/enemy/waves/WaveLoader.h"
#include "selection/TowerSelector.h"

class Tower;


class GameManager {
    bool paused = false;
    std::string pauseState = "Pause";
    int towerLevel = 1;
    int towerUpgradeCost = 550;
    int playerHealth = 1000;
    int playerBalance = 4000;

    const DisplayTextManager displayTextManager;
    EnemyManager enemyManager;
    TowerManager towerManager;
    TowerSelector towerSelector;
    WaveLoader waveLoader = WaveLoader("../../src/resources/waves/dev-waves.json");
    std::chrono::steady_clock::time_point waveTimeStart = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point interWaveTimeStart = std::chrono::steady_clock::now();
    std::queue<std::pair<double, std::pair<double, std::vector<EnemySpawn>>>> enemySpawnTimeQueue;
    std::pair<double, std::pair<double, std::vector<EnemySpawn>>> nextEnemySummonSet;
    float queuedWaveReward = 0.f;
    bool waveLoadingPaused = true;

    static double pressureEconomyPenalize() {
        const int pressureLevel = static_cast<int>(toDecrypt.size()) + currentOperations;
        return static_cast<double>(pressureLevel) / 65 + 1;
    }

    void checkAndLoadNewEnemies() {
        const std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        const auto timeDiff = std::chrono::duration_cast<std::chrono::seconds>(end - waveTimeStart).count();
        if (enemySpawnTimeQueue.empty() && enemyManager.getNumberOfAliveEnemies() == 0 && waveLoadingPaused) {
            if (waveLoader.getCurrentWave() < waveLoader.getMaxWaves()) {
                const int rewardMoney = std::ceil(queuedWaveReward / pressureEconomyPenalize());
                playerBalance += rewardMoney;
                waveTimeStart = std::chrono::steady_clock::now();
                waveLoadingPaused = false;
            }
        }
        if (timeDiff >= 5 && !waveLoadingPaused) {
            loadNextEnemyWave();
            waveLoadingPaused = true;
        }
    }

    void loadNextEnemyWave() {
        std::map<double, std::pair<double, std::vector<EnemySpawn>>> tempSpawnMap;
        double previousSpawnTime = 0;
        const auto &[waveCoinReward, totalWaveTime, enemyInfos] = waveLoader.getNextWave();
        queuedWaveReward = static_cast<float>(waveCoinReward);
        const auto &total_time = totalWaveTime;
        for (const auto &[enemyMultiplier, spawnTimePercent, spawnGap, enemyToSpawn]: enemyInfos) {
            const auto spawnPercent = spawnTimePercent;
            auto spawnTime = total_time * (spawnPercent / 100.0);
            auto spawnInterval = spawnTime - previousSpawnTime;
            previousSpawnTime = spawnTime;
            auto enemy = enemyToSpawn;
            auto spawnPair = tempSpawnMap.find(spawnTime);
            if (spawnPair != tempSpawnMap.end()) {
                spawnPair->second.second.emplace_back(generateSpawnGroup(enemyMultiplier, enemy, spawnGap));
            } else {
                std::vector<EnemySpawn> enemies;
                enemies.push_back(generateSpawnGroup(enemyMultiplier, enemy, spawnGap));
                tempSpawnMap[spawnTime] = std::pair(spawnInterval, enemies);
            }
        }
        for (const auto& [spawnTime, enemyList] : tempSpawnMap) {
            enemySpawnTimeQueue.emplace(spawnTime, enemyList);
        }
        // pause after wave
        setPaused(true);
        // pause after wave
    }

    static EnemySpawn generateSpawnGroup(const int multiplier, const std::shared_ptr<Enemy> &enemy,
                                         const std::string &spawnGap) {
        EnemySpawn spawn;
        spawn.spawnGap = spawnGap;
        spawn.enemiesToSpawn = std::vector<std::shared_ptr<Enemy>>();
        spawn.enemiesToSpawn.push_back(enemy);
        for (int i = 0; i < multiplier - 1; i++) {
            spawn.enemiesToSpawn.push_back(enemy->deep_copy());
        }
        return spawn;
    }

    void summonNewEnemies() {
        const std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        if (enemySpawnTimeQueue.empty()) {
            return;
        }
        auto [spawnTime, enemySpawnInfo] = enemySpawnTimeQueue.front();
        const auto timeDifference = static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(
                                        end - interWaveTimeStart).count()) / 1000;
        if (timeDifference >= enemySpawnInfo.first && !enemySpawnInfo.second.empty()) {
            enemyManager.addEnemies(enemySpawnInfo.second);
            enemySpawnTimeQueue.pop();
            interWaveTimeStart = std::chrono::steady_clock::now();
        }
    }

    void penalizeForFinishedEnemies() {
        for ([[maybe_unused]] const auto &enemy: enemyManager.getEnemiesAtEndOfPath()) {
            playerHealth--;
        }
    }

    void updateMoneyForKilledEnemies() {
        const std::vector<std::shared_ptr<Enemy>> enemies = enemyManager.getDeadEnemies();
        for (const auto &enemy: enemies) {
            if (!enemy->isAlive() && enemy->getTargetNode() < enemyManager.getEnemyPath()->getVertexCount()) {
                playerBalance += std::ceil(enemy->getReward() / pressureEconomyPenalize());
            }
        }
    }

public:
    GameManager() = default;

    //pause methods
    void togglePause() {
        paused = !paused;
        pauseState = paused ? "Play" : "Pause";
    }

    void setPaused(const bool value) {
        paused = value;
        pauseState = paused ? "Play" : "Pause";
    }

    bool isPaused() const { return paused; }
    std::string getPauseState() const { return pauseState; }
    // pause methods

    //upgrade methods
    int getTowerLevel() const {
        return towerLevel;
    }

    int getTowerUpgradeCost() const {
        return towerUpgradeCost;
    }

    void upgradeTowerLevel(const std::shared_ptr<Tower> &selectedTower) {
        if (playerBalance >= towerUpgradeCost && selectedTower != nullptr) {
            for (const auto &placedTowers: towerManager.getTowers()) {
                if (selectedTower->getId() == placedTowers->getId()) {
                    playerBalance -= towerUpgradeCost;
                    selectedTower->upgradeTower();
                    return;
                }
            }
        }
    }


    void update() {
        enemyManager.update();
        towerManager.update();
        towerManager.enemyInteractions(enemyManager.getAliveEnemies());
        updateMoneyForKilledEnemies();
        enemyManager.replaceDeadEnemiesWithChildren();
        checkAndLoadNewEnemies();
        summonNewEnemies();
        penalizeForFinishedEnemies();
    }

    bool attemptSelectingTower(const sf::Vector2i& mousePosition){
        return towerSelector.attemptSelectingTower(mousePosition);
    }

    [[nodiscard]] std::shared_ptr<Tower> attemptSelectingPlacedTower(const sf::Vector2i &mousePosition) const {
        return TowerSelector::attemptSelectingPlacedTower(mousePosition, towerManager.getTowers());
    }

    void dragSelectedTower(const sf::Vector2i& mousePosition) {
        towerSelector.dragSelectedTower(mousePosition);
        const auto towerOverlap = towerSelector.doesSelectedTowerOverlap(towerManager.getTowers());
        const auto towerPosX = towerSelector.getSelectedTower()->getPosition().x;
        const auto menuStartX = SEL_MENU_START_X - towerSelector.getSelectedTower()->getHitTexture()->getRectHitbox()->getSize().x / 2;
        const auto outOfBounds = towerPosX > menuStartX;
        if (towerOverlap || outOfBounds) {
            towerSelector.getSelectedTower()->invalidateRangeIndicator();
        } else {
            towerSelector.getSelectedTower()->validateRangeIndicator();
        }
    }

    void deselectTower() {
        towerSelector.deselectTower();
    }

    [[nodiscard]] bool isTowerAlreadySelected() const {
        return towerSelector.isStillSelected();
    }

    bool addTower(const std::shared_ptr<Tower> &tower) {
        if (playerBalance >= tower->getCost()) {
            const auto overlap = towerSelector.doesSelectedTowerOverlap(towerManager.getTowers());
            const auto towerPosX = towerSelector.getSelectedTower()->getPosition().x;
            const auto menuStartX = SEL_MENU_START_X - towerSelector.getSelectedTower()->getHitTexture()->
                                    getRectHitbox()->getSize().x / 2;
            const auto outOfBounds = towerPosX > menuStartX;
            if (!(overlap || outOfBounds)) {
                towerManager.addTower(tower);
                playerBalance -= tower->getCost();
                return true;
            }
        }
        return false;
    }

    void removeTower(const std::shared_ptr<Tower> &tower) {
        towerManager.removeTower(tower);
    }

    void sellTower(const std::shared_ptr<Tower> &tower) {
        const int sellPrice = getSellPrice(tower); // Refund half of the tower's cost
        playerBalance += sellPrice;
        removeTower(tower);
    }

    std::shared_ptr<Tower> getHoveredTower() {
        return towerSelector.getSelectedTower();
    }

    std::shared_ptr<sf::Drawable> getHoveredTowerDrawable() {
        return towerSelector.getSelectedTower()->getHitTexture()->getDisplayEntity();
    }

    [[nodiscard]] std::vector<std::shared_ptr<sf::Drawable>> getAvailTowerDrawables() const {
        std::vector<std::shared_ptr<sf::Drawable>> drawables;
        const std::vector<std::shared_ptr<Tower>> towers = towerSelector.getAvailableTowers();
        for (const auto &tower: towers) {
            drawables.push_back(tower->getHitTexture()->getDisplayEntity());
        }

        return drawables;
    }

    [[nodiscard]] std::vector<std::shared_ptr<sf::Drawable>> getDrawables() const {
        std::vector<std::shared_ptr<sf::Drawable>> drawables;

        const std::vector<std::shared_ptr<Enemy>> enemies = enemyManager.getAliveEnemies();
        const std::vector<Projectile *> projectiles = towerManager.getActiveProjectiles();
        const std::vector<std::shared_ptr<Tower>> towers = towerManager.getTowers();

        for (const auto &enemy: enemies) {
            drawables.push_back(enemy->getHitTexture()->getDisplayEntity());
        }
        for (const auto projectile: projectiles) {
            drawables.push_back(projectile->getHitTexture()->getDisplayEntity());
        }
        for (const auto &tower: towers) {
            drawables.push_back(tower->getHitTexture()->getDisplayEntity());
        }

        return drawables;
    }

    [[nodiscard]] std::vector<std::shared_ptr<sf::Drawable>> getNewDrawables() {
        const std::vector<std::shared_ptr<Enemy>> enemies = enemyManager.getUndrawnEnemies();
        const std::vector<std::shared_ptr<Projectile>> projectiles = towerManager.getUndrawnProjectiles();
        std::vector<std::shared_ptr<sf::Drawable>> drawables;
        for (const auto &enemy: enemies) {
            drawables.push_back(enemy->getHitTexture()->getDisplayEntity());
        }
        for (const auto &projectile: projectiles) {
            drawables.push_back(projectile->getHitTexture()->getDisplayEntity());
        }
        for (const auto &projectileDisplayEffect: towerManager.getDisplayEffects()) {
            drawables.push_back(projectileDisplayEffect);
        }
        for (const auto &deathAnimationEffect: enemyManager.getDisplayEffects()) {
            drawables.push_back(deathAnimationEffect);
        }
        return drawables;
    }

    [[nodiscard]] std::vector<std::shared_ptr<sf::Drawable>> getRemovableDrawables() const {
        const std::vector<std::shared_ptr<Enemy>> enemies = enemyManager.getDeadEnemies();
        const std::vector<std::shared_ptr<Enemy>> enemies2 = enemyManager.getEnemiesAtEndOfPath();
        const std::vector<Projectile *> projectiles = towerManager.getInactiveProjectiles();
        std::vector<std::shared_ptr<sf::Drawable>> drawables;
        for (const auto &enemy: enemies) {
            drawables.push_back(enemy->getHitTexture()->getDisplayEntity());
        }
        for (const auto &enemy: enemies2) {
            drawables.push_back(enemy->getHitTexture()->getDisplayEntity());
        }
        for (const auto projectile: projectiles) {
            drawables.push_back(projectile->getHitTexture()->getDisplayEntity());
        }
        for (const auto &projectileDisplayEffect: towerManager.getCompletedDisplayEffects()) {
            drawables.push_back(projectileDisplayEffect);
        }
        for (const auto &deathAnimationEffect: enemyManager.getCompletedDisplayEffects()) {
            drawables.push_back(deathAnimationEffect);
        }
        return drawables;
    }

    void cleanup() {
        enemyManager.cleanup();
        towerManager.removeInactiveProjectiles();
    }

    [[nodiscard]] int getMaxWaveNumber() const {
        return waveLoader.getMaxWaves();
    }

    [[nodiscard]] int getCurrentWaveNumber() const {
        return waveLoader.getCurrentWave();
    }

    [[nodiscard]] int getPlayerHealth() const {
        return playerHealth;
    }

    [[nodiscard]] int getPlayerBalance() const {
        return playerBalance;
    }

    [[nodiscard]] int getSellPrice(const std::shared_ptr<Tower> &tower) const {
        const int upgradeEditions = towerUpgradeCost * (tower->getUpgradeValue() - 1);
        const int sellPrice = tower->getCost() / 2 + upgradeEditions / 2;
        return sellPrice;
    }
};

#endif //GAMEMANAGER_H
