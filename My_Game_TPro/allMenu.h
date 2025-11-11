#pragma once
//то что видно на паузе
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <functional>
#include <cmath> // для sin
#include "game.h"
#include <chrono>

class MenuItem;
class AdTimer;


class AdTimer {
private:
    std::chrono::steady_clock::time_point lastAdTime;
    unsigned int adDelaySeconds;

public:
    AdTimer(unsigned int delaySeconds = 300);
    bool canShowAd() const;
    void recordAdShown();
    unsigned int getRemainingSeconds() const;
    void setDelay(unsigned int delaySeconds);
};

class MenuItem {
public:
    MenuItem(const sf::String& label, sf::Font& font, unsigned int size,
        const sf::Vector2f& pos, std::function<void()> callback,
        bool title = false,
        const sf::Color& normalColor = sf::Color(180, 255, 180),
        const sf::Color& hoverColor = sf::Color(0, 255, 0));

    bool isMouseOver(const sf::RenderWindow& window) const;
    void update(float time);
    bool gettitle() const;

    sf::Text text;
    bool hovered = false;
    std::function<void()> onClick;

private:
    bool title;
    sf::Color normalColor;
    sf::Color hoverColor;
};

void renderPause(sf::RenderWindow* window, EnumGameState& gameState, std::function<void(sf::RenderWindow*)> drawStack, AdTimer& adTimer);
void renderWin(sf::RenderWindow* window, EnumGameState& gameState, uint8_t Level, std::function<void(sf::RenderWindow*)> drawStack);
void renderLose(sf::RenderWindow* window, EnumGameState& gameState, uint8_t Level, std::function<void(sf::RenderWindow*)> drawStack);
void renderAd(EnumGameState& GAME_STATE, AdTimer& adTimer, VideoPlayer& VIDEO_PLAYER);