#ifndef PROJECTILESPRITEINJECTOR_H
#define PROJECTILESPRITEINJECTOR_H
#include <memory>

#include "../entity/hit_texture/circle/animated_circle/AnimCircleHitTexture.h"
#include "SFML/Graphics/Texture.hpp"


class ProjectileSpriteInjector {
    std::shared_ptr<sf::Texture> explosionTexture;

public:
    ProjectileSpriteInjector() {
        explosionTexture = std::make_shared<sf::Texture>(sf::Texture());

        if (!explosionTexture->loadFromFile("../../src/resources/textures/explosion/BombExplosion.png")) {
            return;
        }
    }

    [[nodiscard]] std::shared_ptr<AnimCircleHitTexture> createExplosionHitTexture() const {
        return std::make_shared<AnimCircleHitTexture>(AnimCircleHitTexture(*explosionTexture, 64, 64, 25, 0.05f));
    }
};



#endif //PROJECTILESPRITEINJECTOR_H
