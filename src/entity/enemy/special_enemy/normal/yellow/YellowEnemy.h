#ifndef YELLOWENEMY_H
#define YELLOWENEMY_H
#include <memory>

#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/VertexArray.hpp"
#include "../BaseNormalEnemy.h"
#include "../green/GreenEnemy.h"


class YellowEnemy final : public BaseNormalEnemy {
    std::shared_ptr<EnemySpriteInjector> spriteInjector;

public:
    explicit YellowEnemy(const std::shared_ptr<EnemySpriteInjector> &spriteInjector,
                         const std::shared_ptr<sf::VertexArray> &pathToFollow) : BaseNormalEnemy(
        spriteInjector->createAnimHitTexture(), spriteInjector->createAnimDeathTexture(), pathToFollow, 4,
        sf::Color(227, 203, 0), 22.5f) {
        this->spriteInjector = spriteInjector;
    }

    std::vector<std::shared_ptr<Enemy>> getChildren() override {
        std::vector<std::shared_ptr<Enemy>> children;
        if (getHealth() == 0) {
            const auto enemy = std::make_shared<GreenEnemy>(GreenEnemy(spriteInjector, getPathToFollow()));
            children.push_back(enemy);
        } else if (getHealth() == -1) {
            const auto enemy = std::make_shared<BlueEnemy>(BlueEnemy(spriteInjector, getPathToFollow()));
            children.push_back(enemy);
        } else if (getHealth() == -2) {
            const auto enemy = std::make_shared<RedEnemy>(RedEnemy(spriteInjector, getPathToFollow()));
            children.push_back(enemy);
        }
        return children;
    }

protected:
    std::shared_ptr<Enemy> copy() override {
        return std::make_shared<YellowEnemy>(*this);
    }
};


#endif //YELLOWENEMY_H
