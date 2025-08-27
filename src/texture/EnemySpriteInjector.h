#ifndef ENEMYSPRITEINJECTOR_H
#define ENEMYSPRITEINJECTOR_H
#include "../entity/hit_texture/circle/animated_circle/AnimCircleHitTexture.h"
#include "SFML/Graphics/Texture.hpp"


class EnemySpriteInjector {
    std::shared_ptr<sf::Texture> enemyWalkTexture;
    std::shared_ptr<sf::Texture> enemyDeathTexture;

public:
    EnemySpriteInjector() {
        enemyWalkTexture = std::make_shared<sf::Texture>(sf::Texture());
        enemyDeathTexture = std::make_shared<sf::Texture>(sf::Texture());

        if (!enemyWalkTexture->loadFromFile("../../src/resources/textures/slime/D_Walk_F.png")) {
            return;
        }
        if (!enemyDeathTexture->loadFromFile("../../src/resources/textures/slime/S_Death_F.png")) {
            return;
        }
    }

    [[nodiscard]] std::shared_ptr<AnimCircleHitTexture> createAnimHitTexture() const {
        return std::make_shared<AnimCircleHitTexture>(AnimCircleHitTexture(*enemyWalkTexture, 48, 48, 6, 0.1f));
    }

    [[nodiscard]] std::shared_ptr<AnimatedSprite> createAnimDeathTexture() const {
        return std::make_shared<AnimatedSprite>(AnimatedSprite(*enemyDeathTexture, 48, 48, 6, 0.1f));
    }
};



#endif //ENEMYSPRITEINJECTOR_H
