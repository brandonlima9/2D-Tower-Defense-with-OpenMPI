#ifndef REDENEMY_H
#define REDENEMY_H
#include "../BaseNormalEnemy.h"


class RedEnemy final : public BaseNormalEnemy {
public:
    explicit RedEnemy(const std::shared_ptr<EnemySpriteInjector> &spriteInjector,
                      const std::shared_ptr<sf::VertexArray> &pathToFollow) : BaseNormalEnemy(
        spriteInjector->createAnimHitTexture(), spriteInjector->createAnimDeathTexture(), pathToFollow, 1,
        sf::Color(209, 10, 10), 15.f) {
    }

protected:
    std::shared_ptr<Enemy> copy() override {
        return std::make_shared<RedEnemy>(*this);
    }
};


#endif //REDENEMY_H
