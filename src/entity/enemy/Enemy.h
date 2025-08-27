#ifndef BALLOON_H
#define BALLOON_H
#include <algorithm>
#include <cmath>
#include <string>

#include "../../helper/UUID.h"
#include "../hit_texture/circle/animated_circle/AnimCircleHitTexture.h"
#include "SFML/Graphics/Vertex.hpp"


namespace sf {
    class VertexArray;
}

class Enemy {
    std::string id = get_uuid();
    sf::Vertex position;
    std::shared_ptr<sf::VertexArray> path;
    int currentNodeTarget = 0;
    std::shared_ptr<AnimCircleHitTexture> hitTexture;
    std::shared_ptr<AnimatedSprite> deathTexture;
    int health;
    double speed;
    int reward = 0;

    void initialize() {
        if (this->path != nullptr && this->path->getVertexCount() > 0) {
            this->position.position.x = path->operator[](0).position.x;
            this->position.position.y = path->operator[](0).position.y;
            this->hitTexture->setPosition(this->position.position);
        }
    }

    void setHitTexture(const std::shared_ptr<AnimCircleHitTexture>& hitTexture) {
        this->hitTexture = hitTexture;
    }

    void setDeathSprite(const std::shared_ptr<AnimatedSprite>& deathSprite) {
        this->deathTexture = deathSprite;
    }

    public:
        virtual ~Enemy() = default;

        Enemy(const std::shared_ptr<AnimCircleHitTexture>& hitTexture, const std::shared_ptr<AnimatedSprite>& deathTexture,
            const std::shared_ptr<sf::VertexArray>& pathToFollow, const float speed, const int health, const int reward) {
            this->hitTexture = hitTexture;
            this->deathTexture = deathTexture;
            this->path = pathToFollow;
            if (path != nullptr) {
                initialize();
            }
            this->speed = speed;
            this->health = health;
            this->reward = reward;
        }

        void initialize(const std::shared_ptr<sf::VertexArray>& pathToFollow) {
            this->path = pathToFollow;
            initialize();
        }

        void updatePosition() {
            if (path == nullptr) {
                return;
            }
            if (path->getVertexCount() > 0 && currentNodeTarget < path->getVertexCount()) {
                double distanceYetToTravel = speed;
                while (distanceYetToTravel > 0 && path->getVertexCount() - currentNodeTarget > 0) {
                    const auto nextNode = path->operator[](currentNodeTarget).position;
                    double distanceToNextNode = std::abs(sqrt(pow(nextNode.x - position.position.x, 2) + pow(nextNode.y - position.position.y, 2)));
                    distanceYetToTravel -= std::max(static_cast<double>(0), distanceToNextNode);
                    distanceYetToTravel = std::max(static_cast<double>(0), distanceYetToTravel);
                    const auto distanceToTravel = std::min(speed, distanceToNextNode);
                    if (std::abs(nextNode.y - position.position.y) + std::abs(nextNode.x - position.position.x) != 0) {
                        auto xRatio = (nextNode.x - position.position.x) / (std::abs(nextNode.y - position.position.y) + std::abs(nextNode.x - position.position.x));
                        auto yRatio = (nextNode.y - position.position.y) / (std::abs(nextNode.y - position.position.y) + std::abs(nextNode.x - position.position.x));
                        position.position.x += xRatio * static_cast<float>(distanceToTravel);
                        position.position.y += yRatio * static_cast<float>(distanceToTravel);
                        getHitTexture()->setPosition(position.position.x, position.position.y);
                    }
                    if (distanceToNextNode <= 0) {
                        currentNodeTarget++;
                    }
                }
            }
        }

        [[nodiscard]] sf::Vertex getPosition() const {
            return position;
        }

        void setPosition(const sf::Vector2f newPos) {
            position.position = newPos;
            hitTexture->setPosition(newPos);
        }

        [[nodiscard]] int getTargetNode() const {
            return currentNodeTarget;
        }

        void setTargetNode(const int newTarget) {
            currentNodeTarget = newTarget;
        }

        std::shared_ptr<AnimCircleHitTexture> getHitTexture() {
            return hitTexture;
        }

        void subtractHealth(const int toSubtract) {
            health -= toSubtract;
        }

        void kill() {
            health = 0;
        }

        [[nodiscard]] bool isAlive() const {
            return health > 0;
        }

        virtual std::vector<std::shared_ptr<Enemy>> getChildren() { return {}; }

        std::string getId() {
            return id;
        }

        void setId(const std::string &newId) {
            id = newId;
        }

        [[nodiscard]] int getReward() const {
            return reward;
        }

        std::shared_ptr<AnimatedSprite> getDeathAnimation() {
            return deathTexture;
        }

        bool operator == (const Enemy &other) const {
            if(this->id == other.id) {
                return true;
            }
            return false;
        }

        std::shared_ptr<Enemy> deep_copy() {
            const auto oldAnimDeathSprite = getDeathAnimation();
            const auto copiedDeathSprite = std::make_shared<sf::Sprite>(*oldAnimDeathSprite->getSprite());
            const auto copiedDeathAnimSprite = std::make_shared<AnimatedSprite>(
                AnimatedSprite(copiedDeathSprite, oldAnimDeathSprite->getFrameCount(), oldAnimDeathSprite->getFrameTime()));
            copiedDeathAnimSprite->setDoRepeat(false);

            const auto oldAnimSprite = getHitTexture()->getAnimDisplayEntity();
            const auto copiedSprite = std::make_shared<sf::Sprite>(*getHitTexture()->getAnimDisplayEntity()->getSprite());
            const auto copiedHitbox = std::make_shared<sf::CircleShape>(*getHitTexture()->getCircleHitbox());
            const auto copiedHitTexture = std::make_shared<AnimCircleHitTexture>(
                AnimCircleHitTexture(copiedSprite, oldAnimSprite->getFrameCount(), oldAnimSprite->getFrameTime()));

            auto copiedEnemy = copy();
            copiedHitTexture->setHitbox(copiedHitbox);
            copiedEnemy->setDeathSprite(copiedDeathAnimSprite);
            copiedEnemy->setHitTexture(copiedHitTexture);
            copiedEnemy->setId(get_uuid());
            return copiedEnemy;
        }

    protected:
        virtual std::shared_ptr<Enemy> copy() = 0;

        [[nodiscard]] std::shared_ptr<sf::VertexArray> getPathToFollow() const {
            return path;
        }

        [[nodiscard]] int getHealth() const {
            return health;
        }
};



#endif //BALLOON_H
