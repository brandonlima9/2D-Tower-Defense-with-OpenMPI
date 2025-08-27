#ifndef STATICGRAPHICSMANAGER_H
#define STATICGRAPHICSMANAGER_H
#include <memory>

#include "DisplayConsts.h"
#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/Texture.hpp"


class StaticGraphicsManager {
    const sf::Texture MAP_TEXTURE = sf::Texture("../../src/resources/textures/map.png");
    std::shared_ptr<sf::RectangleShape> selectionMenu;
    std::shared_ptr<sf::RectangleShape> menuStat;
    std::shared_ptr<sf::RectangleShape> backgroundMapTexture;

public:
    StaticGraphicsManager() {
        this->selectionMenu = std::make_shared<sf::RectangleShape>(sf::RectangleShape());
        selectionMenu->setPosition(sf::Vector2f(SEL_MENU_START_X, 0));
        selectionMenu->setSize(sf::Vector2f(DISPLAY_MAX_X, DISPLAY_MAX_Y));
        selectionMenu->setFillColor(sf::Color(33, 33, 33, 125));

        this->menuStat = std::make_shared<sf::RectangleShape>(sf::RectangleShape());
        menuStat->setPosition(sf::Vector2f(SEL_MENU_START_X, STAT_GAP_Y * 10));
        menuStat->setSize(sf::Vector2f(300, 275));
        menuStat->setFillColor(sf::Color(51, 77, 77, 125));

        this->backgroundMapTexture = std::make_shared<sf::RectangleShape>(sf::RectangleShape());
        backgroundMapTexture->setSize(sf::Vector2f(DISPLAY_MAX_X, DISPLAY_MAX_Y));
        this->backgroundMapTexture->setTexture(&MAP_TEXTURE);
        this->backgroundMapTexture->setTextureRect(sf::IntRect({0, 0}, {826, 532}));
    }

    [[nodiscard]] std::vector<std::shared_ptr<sf::Drawable>> getStaticDrawables() const {
        return {
            backgroundMapTexture,
            selectionMenu,
            menuStat
        };
    }
};


#endif //STATICGRAPHICSMANAGER_H
