#ifndef DISPLAYTEXTMANAGER_H
#define DISPLAYTEXTMANAGER_H
#include <memory>
#include <iomanip>

#include "DisplayConsts.h"
#include "../helper/Digits.h"
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics/RectangleShape.hpp"

class GameManager; // Forward declaration


class DisplayTextManager {
    sf::Font font;
    const std::shared_ptr<sf::Text> fpsCounter = std::make_shared<sf::Text>(sf::Text(font));
    const std::shared_ptr<sf::Text> lifeCounter = std::make_shared<sf::Text>(sf::Text(font));
    const std::shared_ptr<sf::Text> waveCounter = std::make_shared<sf::Text>(sf::Text(font));
    const std::shared_ptr<sf::Text> pressureRemaining = std::make_shared<sf::Text>(sf::Text(font));
    const std::shared_ptr<sf::Text> jobCounter = std::make_shared<sf::Text>(sf::Text(font));
    const std::shared_ptr<sf::Text> pressureAdditionRate = std::make_shared<sf::Text>(sf::Text(font));
    const std::shared_ptr<sf::Text> pressureCompletionRate = std::make_shared<sf::Text>(sf::Text(font));
    const std::shared_ptr<sf::Text> balanceCounter = std::make_shared<sf::Text>(sf::Text(font));
    const std::shared_ptr<sf::Text> displayDamage = std::make_shared<sf::Text>(sf::Text(font));
    const std::shared_ptr<sf::Text> displaySpeed = std::make_shared<sf::Text>(sf::Text(font));
    const std::shared_ptr<sf::Text> displayType = std::make_shared<sf::Text>(sf::Text(font));
    const std::shared_ptr<sf::Text> displaySellOption = std::make_shared<sf::Text>(sf::Text(font));
    const std::shared_ptr<sf::Text> displayCostOption = std::make_shared<sf::Text>(sf::Text(font));
    const std::shared_ptr<sf::Text> displayPauseOption = std::make_shared<sf::Text>(sf::Text(font));
    const std::shared_ptr<sf::Text> displayUpgradeOption = std::make_shared<sf::Text>(sf::Text(font));
    sf::RectangleShape sellButtonRectangle;
    sf::RectangleShape pauseButtonRectangle;
    sf::RectangleShape upgradeButtonRectangle;

public:
    DisplayTextManager() {
        if (font.openFromFile("../../src/resources/fonts/RasterForgeRegular-JpBgm.ttf")) {
            fpsCounter->setCharacterSize(24);
            fpsCounter->setFillColor(sf::Color::Red);
            fpsCounter->setStyle(sf::Text::Bold);
            fpsCounter->setPosition(sf::Vector2f(FPS_START_RIGHT, TEXT_START_Y));

            lifeCounter->setCharacterSize(24);
            lifeCounter->setFillColor(sf::Color::Red);
            lifeCounter->setStyle(sf::Text::Bold);
            lifeCounter->setPosition(sf::Vector2f(LIFE_START_RIGHT, TEXT_START_Y));

            pressureRemaining->setCharacterSize(24);
            pressureRemaining->setFillColor(sf::Color(150, 80, 80));
            pressureRemaining->setStyle(sf::Text::Bold);
            pressureRemaining->setPosition(sf::Vector2f(PRESSURE_START_RIGHT, TEXT_START_Y));

            jobCounter->setCharacterSize(24);
            jobCounter->setFillColor(sf::Color(80, 150, 150));
            jobCounter->setStyle(sf::Text::Bold);
            jobCounter->setPosition(sf::Vector2f(PRESSURE_START_RIGHT, PRESSURE_GAP_Y * 1));

            pressureCompletionRate->setCharacterSize(24);
            pressureCompletionRate->setFillColor(sf::Color(17, 124, 19));
            pressureCompletionRate->setStyle(sf::Text::Bold);
            pressureCompletionRate->setPosition(sf::Vector2f(PRESSURE_START_RIGHT, PRESSURE_GAP_Y * 2));

            pressureAdditionRate->setCharacterSize(24);
            pressureAdditionRate->setFillColor(sf::Color(255, 99, 71));
            pressureAdditionRate->setStyle(sf::Text::Bold);
            pressureAdditionRate->setPosition(sf::Vector2f(PRESSURE_START_RIGHT, PRESSURE_GAP_Y * 3));

            waveCounter->setCharacterSize(24);
            waveCounter->setFillColor(sf::Color(20, 20, 40));
            waveCounter->setStyle(sf::Text::Bold);
            waveCounter->setPosition(sf::Vector2f(WAVE_START_RIGHT, TEXT_START_Y));

            balanceCounter->setCharacterSize(24);
            balanceCounter->setFillColor(sf::Color::Yellow);
            balanceCounter->setStyle(sf::Text::Bold);
            balanceCounter->setPosition(sf::Vector2f(BALANCE_START_RIGHT, TEXT_START_Y));

            displayType->setCharacterSize(24);
            displayType->setFillColor(sf::Color(51, 204, 255));
            displayType->setStyle(sf::Text::Bold);
            displayType->setPosition(sf::Vector2f(STAT_START_RIGHT, STAT_GAP_Y * 11));

            displayDamage->setCharacterSize(24);
            displayDamage->setFillColor(sf::Color(255, 92, 51));
            displayDamage->setStyle(sf::Text::Bold);
            displayDamage->setPosition(sf::Vector2f(STAT_START_RIGHT, STAT_GAP_Y * 12));

            displaySpeed->setCharacterSize(24);
            displaySpeed->setFillColor(sf::Color(102, 255, 102));
            displaySpeed->setStyle(sf::Text::Bold);
            displaySpeed->setPosition(sf::Vector2f(STAT_START_RIGHT, STAT_GAP_Y * 13));

            displayCostOption->setCharacterSize(24);
            displayCostOption->setFillColor(sf::Color(0, 255, 0));
            displayCostOption->setStyle(sf::Text::Bold);
            displayCostOption->setPosition(sf::Vector2f(STAT_START_RIGHT, STAT_GAP_Y * 14));

            displaySellOption->setCharacterSize(24);
            displaySellOption->setFillColor(sf::Color(255, 255, 0));
            displaySellOption->setStyle(sf::Text::Bold);
            displaySellOption->setPosition(sf::Vector2f(STAT_START_RIGHT, STAT_GAP_Y * 15));

            displayPauseOption->setCharacterSize(24);
            displayPauseOption->setFillColor(sf::Color::Red);
            displayPauseOption->setStyle(sf::Text::Bold);
            displayPauseOption->setOutlineColor(sf::Color::Black);
            displayPauseOption->setOutlineThickness(1);
            displayPauseOption->setPosition(sf::Vector2f(PLAY_START_RIGHT, DISPLAY_MAX_Y - static_cast<float>(100) * DISPLAY_MAX_Y_RATIO));

            displayUpgradeOption->setCharacterSize(14);
            displayUpgradeOption->setFillColor(sf::Color::Cyan);
            displayUpgradeOption->setStyle(sf::Text::Bold);
            displayUpgradeOption->setOutlineColor(sf::Color::Black);
            displayUpgradeOption->setOutlineThickness(1);
            displayUpgradeOption->setPosition(sf::Vector2f(PLAY_START_RIGHT - 40 * DISPLAY_MAX_X_RATIO, STAT_GAP_Y * 17.5f));

            sellButtonRectangle.setPosition(displaySellOption->getPosition());
            sellButtonRectangle.setSize(sf::Vector2f(100.f, 40.f));
            sellButtonRectangle.setFillColor(sf::Color(255, 255, 0, 150));

            pauseButtonRectangle.setPosition(displayPauseOption->getPosition());
            pauseButtonRectangle.setSize(sf::Vector2f(100.f, 40.f));

            upgradeButtonRectangle.setPosition(displayUpgradeOption->getPosition());
            upgradeButtonRectangle.setSize(sf::Vector2f(170.f, 40.f));
        }
    }

    void setLifeCounterValue(const int newHealth) const {
        lifeCounter->setString("Lives: " + std::to_string(newHealth));
    }

    void setWaveCounterValue(const int newWave, const int maxWaves) const {
        waveCounter->setString(std::to_string(newWave) + " / " + std::to_string(maxWaves));
    }

    void setPlayerBalanceValue(const int newBalance) const {
        balanceCounter->setString("Balance: " + std::to_string(newBalance));
    }

    void setFPSCounterValue(const unsigned int newFPS) const {
        fpsCounter->setString(std::to_string(newFPS));
    }

    void setRemainingPressureValue(const int newRemaining) const {
        pressureRemaining->setString("Remaining Pressure Jobs: " + std::to_string(newRemaining));
    }

    void setActivePressureJobsValue(const int newActive) const {
        jobCounter->setString("Active Pressure Jobs: " + std::to_string(newActive));
    }

    void setPressureCompletionRateValue(const double newRate) const {
        pressureCompletionRate->setString(
            "Completion Rate: " + std::to_string(newRate).substr(0, countDigit(static_cast<int>(newRate)) + 3));
    }

    void setPressureProductionRateValue(const double newRate) const {
        pressureAdditionRate->setString(
            "Production Rate: " + std::to_string(newRate).substr(0, countDigit(static_cast<int>(newRate)) + 3));
    }

    void setTowerDamageValue(const int newDamage) const {
        displayDamage->setString("Damage: " + std::to_string(newDamage));
    }

    void setTowerSpeedValue(const float newSpeed) const {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2) << newSpeed; // Set precision to 2 decimal places
        displaySpeed->setString("Speed: " + oss.str());
    }

    void setTowerType(const std::string &newType) const {
        displayType->setString("Type: " + newType);
    }

    void setCostOption(const double newCost) const {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(0) << newCost;
        displayCostOption->setString("Cost: " + oss.str());
    }

    void setSellOption(const double newSell) const {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(0) << newSell;
        displaySellOption->setString("Sell: " + oss.str());
    }

    bool isSellButtonClicked(const sf::Vector2i &mousePosition) const {
        return sellButtonRectangle.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition));
    }

    void setSellColorChange(const sf::Color &newColor) const {
        displaySellOption->setFillColor(newColor);
    }

    void setPauseOption(const std::string &isPaused) const {
        displayPauseOption->setString(isPaused);
    }

    bool isPauseButtonClicked(const sf::Vector2i &mousePosition) const {
        return pauseButtonRectangle.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition));
    }

    void setUpgradeOption(const int upgradeLevel, const int cost) const {
        std::ostringstream oss;
        oss << std::fixed << upgradeLevel + 1 << "\n        for " << std::setprecision(0) << cost;
        displayUpgradeOption->setString("Upgrade to LVL " + oss.str());
    }

    void setUpgradeMaxOption() const {
        displayUpgradeOption->setString("Maximum Tower Level                          ");
    }

    bool isUpgradeButtonClicked(const sf::Vector2i &mousePosition) const {
        return upgradeButtonRectangle.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition));
    }

    void removeTowerStats() const {
        // Remove or hide the text for damage and speed here.
        // For example:
        displayDamage->setString("          ");
        displaySpeed->setString("           ");
        displayType->setString("          ");
        displayCostOption->setString("          ");
        displaySellOption->setString("          ");
        displayUpgradeOption->setString("          ");
    }


    std::vector<std::shared_ptr<sf::Drawable>> getTextDrawables() const {
        return {
            fpsCounter,
            lifeCounter,
            pressureRemaining,
            jobCounter,
            pressureCompletionRate,
            pressureAdditionRate,
            waveCounter,
            balanceCounter,
            displayDamage,
            displaySpeed,
            displayType,
            displayCostOption,
            displaySellOption,
            displayPauseOption,
            displayUpgradeOption
        };
    }
};


#endif //DISPLAYTEXTMANAGER_H
