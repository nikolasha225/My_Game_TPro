#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <functional>
#include <cmath>
#include "game.h"
#include <chrono>

class MenuItem;
class AdTimer;


//свой таймер для рекламы -_-
class AdTimer {
private:
    std::chrono::steady_clock::time_point lastAdTime;
    unsigned int adDelaySeconds;

public:
    AdTimer(unsigned int delaySeconds = 300); //5 минут ждать, эх(
    bool canShowAd() const; //5 минут прошло?
    void recordAdShown(); //считаааем
    unsigned int getRemainingSeconds() const; //снова считаеем
    void setDelay(unsigned int delaySeconds); //выставляем тайм
};

class MenuItem {
public:
    MenuItem(const sf::String& label, sf::Font& font, unsigned int size,
        const sf::Vector2f& pos, std::function<void()> callback,
        bool title = false,
        const sf::Color& normalColor = sf::Color(180, 255, 180),
        const sf::Color& hoverColor = sf::Color(0, 255, 0)); // constructoor

    bool isMouseOver(const sf::RenderWindow& window) const; // по базе себя чувствует
    void update(float time); //навели
    bool gettitle() const; //использовал ли вообще?:

    sf::Text text;
    bool hovered = false;
    std::function<void()> onClick;

private:
    bool title;
    sf::Color normalColor;
    sf::Color hoverColor;
};

//ну тут очевидно вроде кто и где
void renderPause(sf::RenderWindow* window, EnumGameState& gameState, std::function<void(sf::RenderWindow*)> drawStack, AdTimer& adTimer);
void renderWin(sf::RenderWindow* window, EnumGameState& gameState, uint8_t Level, std::function<void(sf::RenderWindow*)> drawStack);
void renderLose(sf::RenderWindow* window, EnumGameState& gameState, uint8_t Level, std::function<void(sf::RenderWindow*)> drawStack);
void renderOver(sf::RenderWindow* window, EnumGameState& gameState, std::function<void(sf::RenderWindow*)> drawStack);
void renderAd(EnumGameState& GAME_STATE, AdTimer& adTimer, VideoPlayer& VIDEO_PLAYER);