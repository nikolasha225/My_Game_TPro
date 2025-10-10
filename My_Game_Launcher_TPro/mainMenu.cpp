#include "mainMenu.h"


    MenuItem::MenuItem(const sf::String& label, sf::Font& font, unsigned int size, const sf::Vector2f& pos, std::function<void()> callback)
        : onClick(callback)
    {
        text.setFont(font);
        text.setString(label);
        text.setCharacterSize(size);
        text.setFillColor(sf::Color(180, 255, 180)); // м€гкий зелЄный
        text.setPosition(pos);
    }

    bool MenuItem::isMouseOver(const sf::RenderWindow& window) const {
        auto mouse = sf::Mouse::getPosition(window);
        auto bounds = text.getGlobalBounds();
        return bounds.contains(static_cast<float>(mouse.x), static_cast<float>(mouse.y));
    }

    void MenuItem::update(float time) {
        if (hovered) {
            // ѕереливающийс€ зелЄный (плавно пульсирует)
            float pulse = (std::sin(time * 3.0f) + 1.f) / 2.f; // 0..1
            sf::Uint8 green = static_cast<sf::Uint8>(180 + 75 * pulse); // 180..255
            text.setFillColor(sf::Color(0, green, 0));

            // ќбводка тоже переливаетс€, но слабее
            sf::Uint8 glow = static_cast<sf::Uint8>(80 + 100 * pulse);
            text.setOutlineColor(sf::Color(0, glow, 0));
        }
        else {
            // ÷вет, когда не наведено
            text.setFillColor(sf::Color::Yellow);
            text.setOutlineColor(sf::Color(0, 60, 0));
        }
    }