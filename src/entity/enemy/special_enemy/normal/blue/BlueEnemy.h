#ifndef BLUEENEMY_H
#define BLUEENEMY_H
#include <memory>

#include "../BaseNormalEnemy.h"
#include "../red/RedEnemy.h"
#include "SFML/Graphics/VertexArray.hpp"


class BaseNormalEnemy;

class BlueEnemy final : public BaseNormalEnemy {
    std::shared_ptr<EnemySpriteInjector> spriteInjector;

public:
    explicit BlueEnemy(const std::shared_ptr<EnemySpriteInjector> &spriteInjector,
                       const std::shared_ptr<sf::VertexArray> &pathToFollow) : BaseNormalEnemy(
        spriteInjector->createAnimHitTexture(), spriteInjector->createAnimDeathTexture(), pathToFollow, 2,
        sf::Color(0, 93, 209), 17.5f) {
        this->spriteInjector = spriteInjector;
    }

    std::vector<std::shared_ptr<Enemy>> getChildren() override {
        std::vector<std::shared_ptr<Enemy>> children;
        if (getHealth() == 0) {
            const auto enemy = std::make_shared<RedEnemy>(RedEnemy(spriteInjector, getPathToFollow()));
            children.push_back(enemy);
        }
        return children;
    }

protected:
    std::shared_ptr<Enemy> copy() override {
        return std::make_shared<BlueEnemy>(*this);
    }
};


#endif //BLUEENEMY_H
