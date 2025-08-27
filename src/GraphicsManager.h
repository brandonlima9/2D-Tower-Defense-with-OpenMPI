#ifndef GRAPHICSMANAGER_H
#define GRAPHICSMANAGER_H
#include <SFML/Graphics.hpp>
#include <list>
#include <map>

#include "display/DisplayConsts.h"

class GraphicsManager {
    std::unique_ptr<sf::RenderWindow> window;
    std::map<int, std::list<std::shared_ptr<sf::Drawable>>> drawables;

public:
    GraphicsManager() {
        this->window = std::make_unique<sf::RenderWindow>(sf::RenderWindow(
            sf::VideoMode({DISPLAY_MAX_X, DISPLAY_MAX_Y}), "Capstone Tower Defence",
            sf::Style::Titlebar | sf::Style::Close));
        this->window->setFramerateLimit(MAX_FRAMERATE);
        drawables.insert(std::pair(0, std::list<std::shared_ptr<sf::Drawable>>()));
        drawables.insert(std::pair(1, std::list<std::shared_ptr<sf::Drawable>>()));
        drawables.insert(std::pair(2, std::list<std::shared_ptr<sf::Drawable>>()));
        drawables.insert(std::pair(3, std::list<std::shared_ptr<sf::Drawable>>()));
        drawables.insert(std::pair(4, std::list<std::shared_ptr<sf::Drawable>>()));
    }

    void addLowPriorityDrawable(const std::shared_ptr<sf::Drawable> &drawable) {
        drawables.find(0)->second.push_back(drawable);
    }

    void addLowPriorityDrawables(const std::vector<std::shared_ptr<sf::Drawable>> &newDrawables) {
        const auto drawableList = &drawables.find(0)->second;
        for (const auto &drawable: newDrawables) {
            drawableList->push_back(drawable);
        }
    }

    void addDrawable(const std::shared_ptr<sf::Drawable> &drawable) {
        drawables.find(2)->second.push_back(drawable);
    }

    void addDrawables(const std::vector<std::shared_ptr<sf::Drawable>> &newDrawables) {
        const auto drawableList = &drawables.find(2)->second;
        for (const auto &drawable: newDrawables) {
            drawableList->push_back(drawable);
        }
    }

    void addPriorityDrawable(const std::shared_ptr<sf::Drawable> &drawable) {
        drawables.find(4)->second.push_back(drawable);
    }

    void addPriorityDrawables(const std::vector<std::shared_ptr<sf::Drawable>> &newDrawables) {
        const auto drawableList = &drawables.find(4)->second;
        for (const auto &drawable: newDrawables) {
            drawableList->push_back(drawable);
        }
    }

    void removeDrawable(const std::shared_ptr<sf::Drawable> &toRemove) {
        for (auto &[priority, listOfDrawables]: drawables) {
            listOfDrawables.remove(toRemove);
        }
    }

    void removeDrawables(const std::vector<std::shared_ptr<sf::Drawable>> &toRemove) {
        for (const std::shared_ptr<sf::Drawable> &drawable: toRemove) {
            removeDrawable(drawable);
        }
    }

    [[nodiscard]] bool isActive() const {
        return window->isOpen();
    }

    void deactivate() const {
        window->close();
    }

    [[nodiscard]] std::optional<sf::Event> pollEvent() const {
        return window->pollEvent();
    }

    void draw() const {
        window->clear();
        for (auto &[priority, listOfDrawables]: drawables) {
            for (const auto &drawable: listOfDrawables) {
                window->draw(*drawable);
            }
        }
        window->display();
    }

    void setFramerateLimit(const int frames) const {
        window->setFramerateLimit(frames);
    }

    [[nodiscard]] sf::Vector2i getMousePosition() const {
        return sf::Mouse::getPosition(*window);
    }
};


#endif //GRAPHICSMANAGER_H
