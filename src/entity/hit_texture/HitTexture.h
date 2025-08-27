#ifndef HITTEXTURE_H
#define HITTEXTURE_H

#include <memory>
#include "SFML/Graphics/Shape.hpp"

class HitTexture {
    public:
        virtual ~HitTexture() = default;

        [[nodiscard]] virtual std::shared_ptr<sf::Drawable> getHitbox() const = 0;

        [[nodiscard]] virtual std::shared_ptr<sf::Drawable> getDisplayEntity() const = 0;

        virtual void setPosition(float x, float y) const = 0;

        virtual void setPosition(sf::Vector2f position) const = 0;
};



#endif //HITTEXTURE_H
