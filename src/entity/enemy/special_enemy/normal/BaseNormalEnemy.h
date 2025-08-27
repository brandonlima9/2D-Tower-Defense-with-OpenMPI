#ifndef BASENORMALENEMY_H
#define BASENORMALENEMY_H
#include <memory>

#include "../../Enemy.h"
#include "../../../../texture/EnemySpriteInjector.h"
#include "SFML/Graphics/VertexArray.hpp"


class DevEnemy;

class BaseNormalEnemy : public Enemy {
public:
    ~BaseNormalEnemy() override = default;

    BaseNormalEnemy(const std::shared_ptr<AnimCircleHitTexture> &animHitTexture,
                    const std::shared_ptr<AnimatedSprite> &animDeathTexture,
                    const std::shared_ptr<sf::VertexArray> &pathToFollow, const float speed, const sf::Color color,
                    const float size)
        : Enemy(animHitTexture, animDeathTexture, pathToFollow, speed, 1, 1) {
        const auto scaleIncrease = 2 * size / 15;

        getDeathAnimation()->getSprite()->setColor(color);
        getDeathAnimation()->getSprite()->setScale(sf::Vector2f(scaleIncrease, scaleIncrease));
        getDeathAnimation()->setDoRepeat(false);

        getHitTexture()->getAnimDisplayEntity()->getSprite()->setScale(sf::Vector2f(scaleIncrease, scaleIncrease));
        getHitTexture()->setDisplayEntityColor(color);
        getHitTexture()->setHitboxColor(sf::Color::Transparent);
        getHitTexture()->setHitboxRadius(size);
    }
};


#endif //BASENORMALENEMY_H
