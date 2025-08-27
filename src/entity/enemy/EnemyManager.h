#ifndef ENEMYMANAGER_H
#define ENEMYMANAGER_H
#include <list>

#include "Enemy.h"
#include "../../helper/visual/ThickLine.h"
#include "../../display/DisplayConsts.h"
#include "SFML/System/Clock.hpp"
#include "SFML/System/Time.hpp"

struct EnemySpawn {
    std::string spawnGap;
    std::vector<std::shared_ptr<Enemy>> enemiesToSpawn;
};

class EnemyManager {
    std::list<std::shared_ptr<Enemy>> enemies;
    std::list<std::shared_ptr<Enemy>> undrawnEnemies;
    std::shared_ptr<sf::VertexArray> enemyPath;
    std::shared_ptr<sf::VertexArray> visualEnemyPath;
    std::list<std::shared_ptr<AnimatedSprite>> deathAnimations;
    sf::Clock animationClock;

    void addChildEnemy(const std::shared_ptr<Enemy> &parentEnemy,
                       const std::vector<std::shared_ptr<Enemy>> &childEnemy) {
        for (const std::shared_ptr<Enemy> &child: childEnemy) {
            child->setPosition(parentEnemy->getPosition().position);
            child->setTargetNode(parentEnemy->getTargetNode());
            child->setId(parentEnemy->getId());
            addEnemy(child);
        }
    }

    static float calculateSpawnGapSize(const std::string &spawnGapType, const std::shared_ptr<Enemy> &enemy) {
        const auto enemyHitBoxRadius = enemy->getHitTexture()->getCircleHitbox()->getRadius();
        if (spawnGapType == "FAR") {
            return enemyHitBoxRadius * 4 + 20;
        }
        if (spawnGapType == "SIDEBY") {
            return enemyHitBoxRadius * 4;
        }
        if (spawnGapType == "CLOSE") {
            return enemyHitBoxRadius;
        }
        return enemyHitBoxRadius;
    }

public:
    EnemyManager() {
        enemyPath = std::make_shared<sf::VertexArray>(sf::VertexArray());
        enemyPath->setPrimitiveType(sf::PrimitiveType::LineStrip);

        sf::Vertex vertices[]{
            {{3.000000f * DISPLAY_MAX_X_RATIO, 453.000000f * DISPLAY_MAX_Y_RATIO}, sf::Color::Red, {0.0f, 0.0f}},
            {{905.000000f * DISPLAY_MAX_X_RATIO, 451.000000f * DISPLAY_MAX_Y_RATIO}, sf::Color::Red, {0.0f, 0.0f}},
            {{952.000000f * DISPLAY_MAX_X_RATIO, 399.000000f * DISPLAY_MAX_Y_RATIO}, sf::Color::Red, {0.0f, 0.0f}},
            {{951.000000f * DISPLAY_MAX_X_RATIO, 247.000000f * DISPLAY_MAX_Y_RATIO}, sf::Color::Red, {0.0f, 0.0f}},
            {{891.000000f * DISPLAY_MAX_X_RATIO, 203.000000f * DISPLAY_MAX_Y_RATIO}, sf::Color::Red, {0.0f, 0.0f}},
            {{681.000000f * DISPLAY_MAX_X_RATIO, 203.000000f * DISPLAY_MAX_Y_RATIO}, sf::Color::Red, {0.0f, 0.0f}},
            {{626.000000f * DISPLAY_MAX_X_RATIO, 240.000000f * DISPLAY_MAX_Y_RATIO}, sf::Color::Red, {0.0f, 0.0f}},
            {{625.000000f * DISPLAY_MAX_X_RATIO, 805.000000f * DISPLAY_MAX_Y_RATIO}, sf::Color::Red, {0.0f, 0.0f}},
            {{559.000000f * DISPLAY_MAX_X_RATIO, 875.000000f * DISPLAY_MAX_Y_RATIO}, sf::Color::Red, {0.0f, 0.0f}},
            {{379.000000f * DISPLAY_MAX_X_RATIO, 877.000000f * DISPLAY_MAX_Y_RATIO}, sf::Color::Red, {0.0f, 0.0f}},
            {{312.000000f * DISPLAY_MAX_X_RATIO, 816.000000f * DISPLAY_MAX_Y_RATIO}, sf::Color::Red, {0.0f, 0.0f}},
            {{311.000000f * DISPLAY_MAX_X_RATIO, 677.000000f * DISPLAY_MAX_Y_RATIO}, sf::Color::Red, {0.0f, 0.0f}},
            {{358.000000f * DISPLAY_MAX_X_RATIO, 632.000000f * DISPLAY_MAX_Y_RATIO}, sf::Color::Red, {0.0f, 0.0f}},
            {{1129.000000f * DISPLAY_MAX_X_RATIO, 617.000000f * DISPLAY_MAX_Y_RATIO}, sf::Color::Red, {0.0f, 0.0f}},
            {{1210.000000f * DISPLAY_MAX_X_RATIO, 570.000000f * DISPLAY_MAX_Y_RATIO}, sf::Color::Red, {0.0f, 0.0f}},
            {{1211.000000f * DISPLAY_MAX_X_RATIO, 413.000000f * DISPLAY_MAX_Y_RATIO}, sf::Color::Red, {0.0f, 0.0f}},
            {{1265.000000f * DISPLAY_MAX_X_RATIO, 351.000000f * DISPLAY_MAX_Y_RATIO}, sf::Color::Red, {0.0f, 0.0f}},
            {{1400.000000f * DISPLAY_MAX_X_RATIO, 353.000000f * DISPLAY_MAX_Y_RATIO}, sf::Color::Red, {0.0f, 0.0f}},
            {{1453.000000f * DISPLAY_MAX_X_RATIO, 415.000000f * DISPLAY_MAX_Y_RATIO}, sf::Color::Red, {0.0f, 0.0f}},
            {{1448.000000f * DISPLAY_MAX_X_RATIO, 714.000000f * DISPLAY_MAX_Y_RATIO}, sf::Color::Red, {0.0f, 0.0f}},
            {{1403.000000f * DISPLAY_MAX_X_RATIO, 774.000000f * DISPLAY_MAX_Y_RATIO}, sf::Color::Red, {0.0f, 0.0f}},
            {{926.000000f * DISPLAY_MAX_X_RATIO, 788.000000f * DISPLAY_MAX_Y_RATIO}, sf::Color::Red, {0.0f, 0.0f}},
            {{856.000000f * DISPLAY_MAX_X_RATIO, 831.000000f * DISPLAY_MAX_Y_RATIO}, sf::Color::Red, {0.0f, 0.0f}},
            {{849.000000f * DISPLAY_MAX_X_RATIO, 1078.000000f * DISPLAY_MAX_Y_RATIO}, sf::Color::Red, {0.0f, 0.0f}}
        };

        std::vector<sf::Vector2f> points;

        for (const auto &vertice: vertices) {
            enemyPath->append(vertice);
            points.push_back(vertice.position);
        }
        ThickLine thickLine(points, 10.f, sf::Color(102, 102, 102));
        visualEnemyPath = std::make_shared<sf::VertexArray>(thickLine.getShape());
    }

    void update() {
        const float deltaTime = animationClock.restart().asSeconds();
        for (const auto &enemy: enemies) {
            if (enemy->isAlive()) {
                enemy->updatePosition();
                enemy->getHitTexture()->getAnimDisplayEntity()->update(deltaTime);
            }
        }
        for (const auto &deathAnimation: deathAnimations) {
            if (!deathAnimation->isFinishedAnimation()) {
                deathAnimation->update(deltaTime);
            }
        }
    }

    void addEnemy(const std::shared_ptr<Enemy> &newEnemy) {
        enemies.push_back(newEnemy);
        undrawnEnemies.push_back(newEnemy);
    }

    void addEnemies(const std::vector<std::shared_ptr<Enemy>> &newEnemies) {
        for (const auto &enemy: newEnemies) {
            enemy->initialize(enemyPath);
            enemies.push_back(enemy);
            undrawnEnemies.push_back(enemy);
        }
    }

    void addEnemies(const std::vector<EnemySpawn> &newSpawnGroup) {
        for (const auto &[spawnGap, enemiesToSpawn]: newSpawnGroup) {
            int counter = 0;
            for (const auto &enemy: enemiesToSpawn) {
                const auto spawnGapDistance = calculateSpawnGapSize(spawnGap, enemy);
                enemy->initialize(enemyPath);
                const sf::Vertex startPosition = enemy->getPosition();
                enemy->setPosition(sf::Vector2f(
                    startPosition.position.x - spawnGapDistance * static_cast<float>(counter),
                    startPosition.position.y));
                enemies.push_back(enemy);
                undrawnEnemies.push_back(enemy);
                counter++;
            }
        }
    }

    [[nodiscard]] std::vector<std::shared_ptr<Enemy>> getAliveEnemies() const {
        std::vector<std::shared_ptr<Enemy>> alive;
        for (auto &enemy: enemies) {
            if (enemy->isAlive()) {
                alive.push_back(enemy);
            }
        }
        return alive;
    }

    [[nodiscard]] std::vector<std::shared_ptr<Enemy>> getUndrawnEnemies() {
        std::vector<std::shared_ptr<Enemy>> alive;
        for (auto &enemy: undrawnEnemies) {
            if (enemy->isAlive()) {
                alive.push_back(enemy);
            }
        }
        undrawnEnemies.clear();
        return alive;
    }

    [[nodiscard]] std::vector<std::shared_ptr<sf::Drawable>> getDisplayEffects() {
        std::vector<std::shared_ptr<sf::Drawable>> drawables;
        for (const auto &deadEnemy: getDeadEnemies()) {
            const auto xOffset = static_cast<float>(deadEnemy->getDeathAnimation()->getFrameSize().x) * deadEnemy->
                                 getDeathAnimation()->getSprite()->getScale().x;
            const auto yOffset = static_cast<float>(deadEnemy->getDeathAnimation()->getFrameSize().y) * deadEnemy->
                                 getDeathAnimation()->getSprite()->getScale().y;

            deadEnemy->getDeathAnimation()->setPosition(deadEnemy->getPosition().position.x - xOffset / 2.f,
                                                        deadEnemy->getPosition().position.y - yOffset / 1.5f);
            deathAnimations.push_back(deadEnemy->getDeathAnimation());
            drawables.push_back(deadEnemy->getDeathAnimation()->getSprite());
        }
        return drawables;
    }

    [[nodiscard]] std::vector<std::shared_ptr<sf::Drawable>> getCompletedDisplayEffects() const {
        std::vector<std::shared_ptr<sf::Drawable>> drawables;
        for (const auto &deathAnimation: deathAnimations) {
            if (deathAnimation->isFinishedAnimation()) {
                drawables.push_back(deathAnimation->getSprite());
            }
        }
        return drawables;
    }

    [[nodiscard]] std::vector<std::shared_ptr<Enemy>> getDeadEnemies() const {
        std::vector<std::shared_ptr<Enemy>> dead;
        for (auto &enemy: enemies) {
            if (!enemy->isAlive()) {
                dead.push_back(enemy);
            }
        }
        return dead;
    }

    [[nodiscard]] std::vector<std::shared_ptr<Enemy>> getEnemiesAtEndOfPath() const {
        std::vector<std::shared_ptr<Enemy>> dead;
        for (auto &enemy: enemies) {
            if (enemy->getTargetNode() >= enemyPath->getVertexCount()) {
                enemy->kill();
                dead.push_back(enemy);
            }
        }
        return dead;
    }

    void replaceDeadEnemiesWithChildren() {
        auto it = enemies.begin();
        for (int i = 0; i < enemies.size(); i++) {
            if (!it.operator*()->isAlive()) {
                addChildEnemy(it.operator*(), it.operator*()->getChildren());
                //enemies.remove(it.operator*());
            }
            ++it;
        }
    }

    void cleanup() {
        enemies.remove_if([](const std::shared_ptr<Enemy> &enemy) {
            return !enemy->isAlive();
        });
        undrawnEnemies.remove_if([](const std::shared_ptr<Enemy> &enemy) {
            return !enemy->isAlive();
        });
        deathAnimations.remove_if([](const std::shared_ptr<AnimatedSprite> &sprite) {
            return sprite->isFinishedAnimation();
        });
    }

    [[nodiscard]] std::shared_ptr<sf::VertexArray> getEnemyPath() const {
        return enemyPath;
    }

    [[nodiscard]] std::shared_ptr<sf::VertexArray> getVisualEnemyPath() const {
        return visualEnemyPath;
    }

    [[nodiscard]] unsigned long getNumberOfAliveEnemies() const {
        return enemies.size();
    }
};


#endif //ENEMYMANAGER_H
