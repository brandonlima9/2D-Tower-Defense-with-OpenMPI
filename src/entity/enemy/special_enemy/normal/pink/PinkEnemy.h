#ifndef PINKENEMY_H
#define PINKENEMY_H
#include <memory>

#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/VertexArray.hpp"
#include "../BaseNormalEnemy.h"
#include "../yellow/YellowEnemy.h"

class PinkEnemy final : public BaseNormalEnemy {
    std::shared_ptr<EnemySpriteInjector> spriteInjector;

public:
    explicit PinkEnemy(const std::shared_ptr<EnemySpriteInjector> &spriteInjector,
                       const std::shared_ptr<sf::VertexArray> &pathToFollow) : BaseNormalEnemy(
        spriteInjector->createAnimHitTexture(), spriteInjector->createAnimDeathTexture(), pathToFollow, 5,
        sf::Color(205, 43, 115), 25.f) {
        this->spriteInjector = spriteInjector;
    }

    std::vector<std::shared_ptr<Enemy>> getChildren() override {
        std::vector<std::shared_ptr<Enemy>> children;
        if (getHealth() == 0) {
            const auto enemy = std::make_shared<YellowEnemy>(YellowEnemy(spriteInjector, getPathToFollow()));
            children.push_back(enemy);
        } else if (getHealth() == -1) {
            const auto enemy = std::make_shared<GreenEnemy>(GreenEnemy(spriteInjector, getPathToFollow()));
            children.push_back(enemy);
        } else if (getHealth() == -2) {
            const auto enemy = std::make_shared<BlueEnemy>(BlueEnemy(spriteInjector, getPathToFollow()));
            children.push_back(enemy);
        } else if (getHealth() == -3) {
            const auto enemy = std::make_shared<RedEnemy>(RedEnemy(spriteInjector, getPathToFollow()));
            children.push_back(enemy);
        }
        return children;
    }

protected:
    std::shared_ptr<Enemy> copy() override {
        return std::make_shared<PinkEnemy>(*this);
    }
};


#endif //PINKENEMY_H
