#pragma once
//то что видно на паузе
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <functional>
#include <cmath> // для sin
#include "game.h"

class MenuItem;

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

void renderPause(sf::RenderWindow* window, EnumGameState& gameState);