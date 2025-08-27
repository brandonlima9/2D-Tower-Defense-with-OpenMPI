#ifndef BASICPROJECTILE_H
#define BASICPROJECTILE_H
#include "../BaseNormalProjectile.h"
#include "../../Projectile.h"


class BasicProjectile final : public BaseNormalProjectile {
public:
    BasicProjectile(const float posX, const float posY, const float direction, const int pierce, const int damage)
        : BaseNormalProjectile(posX, posY, direction, pierce, damage, 20, 4.f, sf::Color(0, 0, 0)) {
    }

    BasicProjectile(const float posX, const float posY, const float direction, const int pierce)
        : BaseNormalProjectile(posX, posY, direction, pierce, 1, 20, 4.f, sf::Color(0, 0, 0)) {
    }

    BasicProjectile(const float posX, const float posY, const float direction)
        : BasicProjectile(posX, posY, direction, 1) {
    }
};

#endif //BASICPROJECTILE_H
