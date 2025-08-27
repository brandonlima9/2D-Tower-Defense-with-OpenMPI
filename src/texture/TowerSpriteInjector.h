#ifndef TOWERSPRITEINJECTOR_H
#define TOWERSPRITEINJECTOR_H
#include <memory>

#include "../entity/hit_texture/rectangle/animated_rectangle/AnimRectangleHitTexture.h"
#include "SFML/Graphics/Texture.hpp"


class TowerSpriteInjector {
    std::shared_ptr<sf::Texture> archerMinTexture;
    std::shared_ptr<sf::Texture> archerMidTexture;
    std::shared_ptr<sf::Texture> archerMaxTexture;

    std::shared_ptr<sf::Texture> radialMinTexture;
    std::shared_ptr<sf::Texture> radialMidTexture;
    std::shared_ptr<sf::Texture> radialMaxTexture;

    std::shared_ptr<sf::Texture> cannonMinTexture;
    std::shared_ptr<sf::Texture> cannonMidTexture;
    std::shared_ptr<sf::Texture> cannonMaxTexture;
    std::shared_ptr<sf::Texture> depressorTexture;

public:
    TowerSpriteInjector() {
        archerMinTexture = std::make_shared<sf::Texture>(sf::Texture());
        radialMinTexture = std::make_shared<sf::Texture>(sf::Texture());
        cannonMinTexture = std::make_shared<sf::Texture>(sf::Texture());

        archerMidTexture = std::make_shared<sf::Texture>(sf::Texture());
        radialMidTexture = std::make_shared<sf::Texture>(sf::Texture());
        cannonMidTexture = std::make_shared<sf::Texture>(sf::Texture());

        archerMaxTexture = std::make_shared<sf::Texture>(sf::Texture());
        radialMaxTexture = std::make_shared<sf::Texture>(sf::Texture());
        cannonMaxTexture = std::make_shared<sf::Texture>(sf::Texture());

        depressorTexture = std::make_shared<sf::Texture>(sf::Texture());

        if (!archerMinTexture->loadFromFile("../../src/resources/textures/towers/normal/ARCHER_MIN.png")) {
            return;
        }
        if (!radialMinTexture->loadFromFile("../../src/resources/textures/towers/normal/RADIAL_MIN.png")) {
            return;
        }
        if (!cannonMinTexture->loadFromFile("../../src/resources/textures/towers/normal/CANNON_MAGE_MIN.png")) {
            return;
        }
        if (!archerMidTexture->loadFromFile("../../src/resources/textures/towers/normal/ARCHER_MID.png")) {
            return;
        }
        if (!radialMidTexture->loadFromFile("../../src/resources/textures/towers/normal/RADIAL_MID.png")) {
            return;
        }
        if (!cannonMidTexture->loadFromFile("../../src/resources/textures/towers/normal/CANNON_MAGE_MID.png")) {
            return;
        }
        if (!archerMaxTexture->loadFromFile("../../src/resources/textures/towers/normal/ARCHER_MAX.png")) {
            return;
        }
        if (!radialMaxTexture->loadFromFile("../../src/resources/textures/towers/normal/RADIAL_MAX.png")) {
            return;
        }
        if (!cannonMaxTexture->loadFromFile("../../src/resources/textures/towers/normal/CANNON_MAGE_MAX.png")) {
            return;
        }
        if (!depressorTexture->loadFromFile("../../src/resources/textures/towers/normal/DEPRESS_TOWER.png")) {
            return;
        }
    }

    [[nodiscard]] std::shared_ptr<AnimRectangleHitTexture> createArcherMinIdleHitTexture() const {
        return std::make_shared<AnimRectangleHitTexture>(AnimRectangleHitTexture(*archerMinTexture, 70, 130, 4, 0.1f));
    }

    [[nodiscard]] std::shared_ptr<AnimRectangleHitTexture> createRadialMinIdleHitTexture() const {
        return std::make_shared<AnimRectangleHitTexture>(AnimRectangleHitTexture(*radialMinTexture, 70, 130, 4, 0.1f));
    }

    [[nodiscard]] std::shared_ptr<AnimRectangleHitTexture> createCannonMinIdleHitTexture() const {
        return std::make_shared<AnimRectangleHitTexture>(AnimRectangleHitTexture(*cannonMinTexture, 30, 44, 6, 0.1f));
    }

    [[nodiscard]] std::shared_ptr<AnimRectangleHitTexture> createDepressIdleHitTexture() const {
        return std::make_shared<AnimRectangleHitTexture>(AnimRectangleHitTexture(*depressorTexture, 64, 64, 25, 0.2f));
    }

    [[nodiscard]] std::shared_ptr<AnimatedSprite> createArcherMidIdleSprite() const {
        return std::make_shared<AnimatedSprite>(AnimatedSprite(*archerMidTexture, 70, 130, 6, 0.1f));
    }

    [[nodiscard]] std::shared_ptr<AnimatedSprite> createRadialMidIdleSprite() const {
        return std::make_shared<AnimatedSprite>(AnimatedSprite(*radialMidTexture, 70, 130, 6, 0.1f));
    }

    [[nodiscard]] std::shared_ptr<AnimatedSprite> createCannonMidIdleSprite() const {
        return std::make_shared<AnimatedSprite>(AnimatedSprite(*cannonMidTexture, 30, 44, 6, 0.1f));
    }

    [[nodiscard]] std::shared_ptr<AnimatedSprite> createArcherMaxIdleSprite() const {
        return std::make_shared<AnimatedSprite>(AnimatedSprite(*archerMaxTexture, 70, 130, 6, 0.1f));
    }

    [[nodiscard]] std::shared_ptr<AnimatedSprite> createRadialMaxIdleSprite() const {
        return std::make_shared<AnimatedSprite>(AnimatedSprite(*radialMaxTexture, 70, 130, 6, 0.1f));
    }

    [[nodiscard]] std::shared_ptr<AnimatedSprite> createCannonMaxIdleSprite() const {
        return std::make_shared<AnimatedSprite>(AnimatedSprite(*cannonMaxTexture, 30, 44, 6, 0.1f));
    }
};



#endif //TOWERSPRITEINJECTOR_H
