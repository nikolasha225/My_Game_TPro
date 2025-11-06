#include "pauseMenu.h"
#include "game.h"
MenuItem::MenuItem(const sf::String& label, sf::Font& font, unsigned int size, const sf::Vector2f& pos, std::function<void()> callback, bool title)
    : onClick(callback), title(title)
{
    text.setFont(font);
    text.setString(label);
    text.setCharacterSize(size);
    text.setFillColor(sf::Color(180, 255, 180)); // мягкий зелёный
    text.setPosition(pos);
}

bool MenuItem::isMouseOver(const sf::RenderWindow& window) const {
    auto mouse = sf::Mouse::getPosition(window);
    auto bounds = text.getGlobalBounds();
    return bounds.contains(static_cast<float>(mouse.x), static_cast<float>(mouse.y));
}

void MenuItem::update(float time) {
    if (hovered && !title) {
        // Переливающийся зелёный (плавно пульсирует)
        float pulse = (std::sin(time * 3.0f) + 1.f) / 2.f; // 0..1
        sf::Uint8 green = static_cast<sf::Uint8>(180 + 75 * pulse); // 180..255
        text.setFillColor(sf::Color(0, green, 0));

        // Обводка тоже переливается, но слабее
        sf::Uint8 glow = static_cast<sf::Uint8>(80 + 100 * pulse);
        text.setOutlineColor(sf::Color(0, glow, 0));
    }
    else {
        // Цвет, когда не наведено
        text.setFillColor(sf::Color::Yellow);
        text.setOutlineColor(sf::Color(0, 60, 0));
    }
}

bool MenuItem::gettitle() const {
    return this->title;
}


void renderPause(sf::RenderWindow* window, EnumGameState& gameState) {
    sf::Font font;
    if (!font.loadFromFile("textures/font/PressStart2P-Regular.ttf")) {
        std::cerr << "Не удалось загрузить шрифт!\n";
        return;
    }

    //для заднего фона
    sf::Texture gameScreenshot;
    gameScreenshot.create(window->getSize().x, window->getSize().y);
    gameScreenshot.update(*window);
    sf::Sprite backgroundSprite(gameScreenshot);

    //просто по красоте
    sf::RectangleShape overlay(sf::Vector2f(window->getSize()));
    overlay.setFillColor(sf::Color(0, 0, 0, 150));

    sf::RectangleShape menuBackground(sf::Vector2f(600.f, 400.f));
    menuBackground.setFillColor(sf::Color(20, 20, 30, 200));
    menuBackground.setOutlineColor(sf::Color(100, 150, 100, 150));
    menuBackground.setOutlineThickness(3.f);
    menuBackground.setPosition(700.f, 400.f);

    std::vector<MenuItem> pauseMenu = {
        MenuItem(L"МЕНЮ", font, 50, {750.f, 450.f}, []() {}, true),
        MenuItem(L"Продолжить", font, 36, {710.f, 500.f}, [&gameState]() {gameState = GAME;}, false),
        MenuItem(L"Выйти", font, 36, {725.f, 600.f}, [&window]() {window->close();}, false) // выйти в лаунчер
    };
    bool menuActive = true;

    while (window->isOpen() && menuActive) {
        sf::Event event;
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window->close();
                menuActive = false;
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                gameState = GAME;
                menuActive = false;
            }
            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left) {
                for (auto& item : pauseMenu) {
                    if (item.isMouseOver(*window)) {
                        item.onClick();
                        if (gameState == GAME) {
                            menuActive = false;
                        }
                        break;
                    }
                }
            }
        }
        sf::Clock clock;
        float time = clock.getElapsedTime().asSeconds();
        for (auto& item : pauseMenu) {
            item.hovered = item.isMouseOver(*window);
            item.update(time);
        }

        window->draw(backgroundSprite);
        window->draw(overlay);
        window->draw(menuBackground);

        for (const auto& item : pauseMenu) {
            window->draw(item.text);
        }

        window->display();
    }
}