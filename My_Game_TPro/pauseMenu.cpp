#include "pauseMenu.h"
#include "game.h"
MenuItem::MenuItem(const sf::String& label, sf::Font& font, unsigned int size,
    const sf::Vector2f& pos, std::function<void()> callback,
    bool title, const sf::Color& normalColor, const sf::Color& hoverColor)
    : onClick(callback), title(title), normalColor(normalColor), hoverColor(hoverColor)
{
    text.setFont(font);
    text.setString(label);
    text.setCharacterSize(size);
    text.setFillColor(normalColor); // используем переданный цвет
    text.setPosition(pos);
}

bool MenuItem::isMouseOver(const sf::RenderWindow& window) const {
    auto mouse = sf::Mouse::getPosition(window);
    auto bounds = text.getGlobalBounds();
    return bounds.contains(static_cast<float>(mouse.x), static_cast<float>(mouse.y));
}

void MenuItem::update(float time) {
    if (hovered && !title) {
        // Используем hoverColor для анимации при наведении
        float pulse = (std::sin(time * 3.0f) + 1.f) / 2.f; // 0..1

        // Интерполируем между normalColor и hoverColor
        sf::Uint8 r = static_cast<sf::Uint8>(normalColor.r + (hoverColor.r - normalColor.r) * pulse);
        sf::Uint8 g = static_cast<sf::Uint8>(normalColor.g + (hoverColor.g - normalColor.g) * pulse);
        sf::Uint8 b = static_cast<sf::Uint8>(normalColor.b + (hoverColor.b - normalColor.b) * pulse);

        text.setFillColor(sf::Color(r, g, b));

        // Обводка тоже переливается
        sf::Uint8 glow = static_cast<sf::Uint8>(80 + 100 * pulse);
        text.setOutlineColor(sf::Color(0, glow, 0));
    }
    else {
        // Цвет, когда не наведено - используем normalColor
        text.setFillColor(normalColor);
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

    sf::RectangleShape menuBorder(sf::Vector2f(800.f, 600.f));
    menuBorder.setFillColor(sf::Color(20, 20, 30, 200));
    menuBorder.setOutlineColor(sf::Color(100, 150, 100, 150));
    menuBorder.setOutlineThickness(3.f);
    menuBorder.setPosition(600.f, 300.f);

    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("textures/img/firsttwowithprop.png")) {
        return;
    }
    sf::Sprite menuBackground(backgroundTexture);
    menuBackground.setPosition(600.f, 300.f);

    std::vector<MenuItem> pauseMenu = {
        MenuItem(L"Продолжить", font, 36, {800.f, 400.f}, [&gameState]() {gameState = GAME;}, false,   sf::Color(100, 255, 100),    // нормальный - светлый зеленый
        sf::Color(0, 255, 0)),
        MenuItem(L"Выйти из игры", font, 36, {750.f, 700.f}, [&window]() {window->close();}, false) // выйти в лаунчер
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
        window->draw(menuBorder);
        window->draw(menuBackground);

        for (const auto& item : pauseMenu) {
            window->draw(item.text);
        }

        window->display();
    }
}