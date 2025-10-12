#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <functional>
#include <cmath> // для sin

class MenuItem;


class MenuItem {
public:
    sf::Text text;
    std::function<void()> onClick;
    bool title;
    bool hovered = false;
    MenuItem(const sf::String& label, sf::Font& font, unsigned int size, const sf::Vector2f& pos, std::function<void()> callback, bool title);
    bool isMouseOver(const sf::RenderWindow& window) const;
    void update(float time);
};