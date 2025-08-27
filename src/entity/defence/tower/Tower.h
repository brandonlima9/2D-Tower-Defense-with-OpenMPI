#ifndef TOWER_H
#define TOWER_H
#include <string>
#include <memory>
#include <utility>

#include "../../../helper/UUID.h"
#include "../../hit_texture/rectangle/animated_rectangle/AnimRectangleHitTexture.h"
#include "SFML/Graphics.hpp"
#include "../projectile/Projectile.h"


class Tower {
    std::string id = get_uuid();
    sf::Vector2f position;
    float range;
    int cost;
    float attackSpeed;
    int damage;
    int upgradeValue = 1;
    std::string type;
    std::shared_ptr<sf::CircleShape> rangeIndicator;
    std::shared_ptr<AnimRectangleHitTexture> hitTexture;

    void setHitTexture(const std::shared_ptr<AnimRectangleHitTexture> &hitTexture) {
        this->hitTexture = hitTexture;
    }

    void setRangeIndicator(const std::shared_ptr<sf::CircleShape> &rangeIndicator) {
        this->rangeIndicator = rangeIndicator;
    }

public:
    virtual ~Tower() = default;

    Tower(const std::shared_ptr<AnimRectangleHitTexture> &hitTexture, const sf::Vector2f &position, const float range,
          const int towerCost, const float attackSpeed, const int damage, std::string type)
        : position(position), range(range), cost(towerCost), attackSpeed(attackSpeed), damage(damage),
          type(std::move(type)) {
        this->hitTexture = hitTexture;
        rangeIndicator = std::make_shared<sf::CircleShape>(sf::CircleShape());
        rangeIndicator->setRadius(range);
        rangeIndicator->setPosition(position);
        rangeIndicator->setFillColor(sf::Color(137, 137, 137, 50)); // Semi-transparent
    }

    virtual std::vector<std::shared_ptr<Projectile>> shootProjectile(std::vector<std::shared_ptr<Enemy>> &enemies) {
        return {};
    }

    [[nodiscard]] sf::Vector2f getPosition() const {
        return position;
    }

    void setPosition(const sf::Vector2f &newPosition) {
        position = newPosition;
        hitTexture->setPosition(newPosition);
        rangeIndicator->setPosition(sf::Vector2f(newPosition.x - rangeIndicator->getRadius(),
                                                 newPosition.y - rangeIndicator->getRadius()));
    }

    [[nodiscard]] float getRange() const {
        return range;
    }

    [[nodiscard]] int getCost() const {
        return cost;
    }

    [[nodiscard]] float getAttackSpeed() const {
        return attackSpeed;
    }

    void upgradeTower() {
        upgradeValue++;
        upgradeTower(upgradeValue);
    }

    [[nodiscard]] int getUpgradeValue() const {
        return upgradeValue;
    }

    void setAttackSpeed(const float value) {
        attackSpeed = value;
    }

    void setDamage(const int value) {
        damage = value;
    }

    [[nodiscard]] int getDamage() const {
        return damage;
    }

    [[nodiscard]] std::string getType() const {
        return type;
    }

    [[nodiscard]] std::string getId() const {
        return id;
    }

    void setId(const std::string &newId) {
        id = newId;
    }

    [[nodiscard]] std::shared_ptr<AnimRectangleHitTexture> getHitTexture() const {
        return hitTexture;
    }

    [[nodiscard]] std::shared_ptr<sf::CircleShape> getRangeIndicator() const {
        return rangeIndicator; // Return the range indicator as a drawable
    }

    void validateRangeIndicator() const {
        auto currentColor = rangeIndicator->getFillColor();
        currentColor.r = 137;
        currentColor.g = 137;
        currentColor.b = 137;
        rangeIndicator->setFillColor(currentColor);
    }

    void invalidateRangeIndicator() const {
        auto currentColor = rangeIndicator->getFillColor();
        currentColor.r = 255;
        currentColor.g = 137;
        currentColor.b = 137;
        rangeIndicator->setFillColor(currentColor);
    }

    std::shared_ptr<Tower> deep_copy() {
        const auto oldAnimSprite = getHitTexture()->getAnimDisplayEntity();
        const auto copiedSprite = std::make_shared<sf::Sprite>(*getHitTexture()->getAnimDisplayEntity()->getSprite());
        const auto copiedHitbox = std::make_shared<sf::RectangleShape>(*getHitTexture()->getRectHitbox());
        const auto copiedHitTexture = std::make_shared<AnimRectangleHitTexture>(
            AnimRectangleHitTexture(copiedSprite, oldAnimSprite->getFrameCount(), oldAnimSprite->getFrameTime()));
        const auto copiedRangeIndicator = std::make_shared<sf::CircleShape>(*getRangeIndicator());

        auto copiedTower = copy();
        copiedHitTexture->setHitbox(copiedHitbox);
        copiedTower->setHitTexture(copiedHitTexture);
        copiedTower->setRangeIndicator(copiedRangeIndicator);
        copiedTower->setId(get_uuid());
        return copiedTower;
    }

protected:
    virtual void upgradeTower(int level) =0;

    virtual std::shared_ptr<Tower> copy() = 0;
};

#endif //TOWER_H
