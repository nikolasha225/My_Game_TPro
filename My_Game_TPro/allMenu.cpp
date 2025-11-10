#include "allMenu.h"
#include "game.h"

MenuItem::MenuItem(const sf::String& label, sf::Font& font, unsigned int size,
    const sf::Vector2f& pos, std::function<void()> callback,
    bool title, const sf::Color& normalColor, const sf::Color& hoverColor)
    : onClick(callback), title(title), normalColor(normalColor), hoverColor(hoverColor)
{
    text.setFont(font);
    text.setString(label);
    text.setCharacterSize(size);
    text.setFillColor(normalColor);
    text.setPosition(pos);
    text.setOutlineThickness(2.f);
    text.setOutlineColor(sf::Color(0, 60, 0));
}

bool MenuItem::isMouseOver(const sf::RenderWindow& window) const {
    auto mouse = sf::Mouse::getPosition(window);
    auto bounds = text.getGlobalBounds();
    return bounds.contains(static_cast<float>(mouse.x), static_cast<float>(mouse.y));
}

void MenuItem::update(float time) {
    if (hovered && !title) {
        float pulse = (std::sin(time * 6.0f) + 1.f) / 2.f; // Быстрая пульсация

        // Яркое переливание между цветами
        sf::Uint8 r = static_cast<sf::Uint8>(normalColor.r + (hoverColor.r - normalColor.r) * pulse);
        sf::Uint8 g = static_cast<sf::Uint8>(normalColor.g + (hoverColor.g - normalColor.g) * pulse);
        sf::Uint8 b = static_cast<sf::Uint8>(normalColor.b + (hoverColor.b - normalColor.b) * pulse);

        text.setFillColor(sf::Color(r, g, b));

        // Пульсирующая обводка
        sf::Uint8 glow = static_cast<sf::Uint8>(100 + 155 * pulse);
        text.setOutlineColor(sf::Color(0, glow, 0)); // Зеленая пульсирующая обводка

    }
    else {
        // Возвращаем к нормальному состоянию
        text.setFillColor(normalColor);
        text.setOutlineColor(sf::Color(0, 60, 0));
    }
}

bool MenuItem::gettitle() const {
    return this->title;
}

void renderPause(sf::RenderWindow* window, EnumGameState& gameState, std::function<void(sf::RenderWindow*)> drawStack) {
    sf::Font font;
    if (!font.loadFromFile("textures/font/PressStart2P-Regular.ttf")) {
        std::cerr << "Не удалось загрузить шрифт!\n";
        return;
    }

    // zatmenue
    sf::RectangleShape overlay(sf::Vector2f(window->getSize()));
    overlay.setFillColor(sf::Color(0, 0, 0, 150));

    // for center
    sf::Vector2u windowSize = window->getSize();
    float menuWidth = 800.f;
    float menuHeight = 600.f;
    float menuX = (windowSize.x - menuWidth) / 2.f;
    float menuY = (windowSize.y - menuHeight) / 2.f;

    sf::RectangleShape menuBorder(sf::Vector2f(800.f, 600.f));
    menuBorder.setFillColor(sf::Color(30, 30, 30));
    menuBorder.setOutlineColor(sf::Color(100, 150, 100, 150));
    menuBorder.setOutlineThickness(3.f);
    menuBorder.setPosition(menuX, menuY);

    // Центральные координаты для текста
    float centerX = menuX + menuWidth / 2.f;
    float startY = menuY + 100.f;
    float itemSpacing = 80.f;

    // Контрастные цвета для пульсации
    std::vector<MenuItem> pauseMenu = {
        MenuItem(L"Продолжить", font, 36, {centerX, startY},
                [&gameState]() {gameState = GAME;}, false,
                sf::Color(100, 255, 100),    // Ярко-зеленый
                sf::Color(0, 255, 0)),       // Очень яркий зеленый

        MenuItem(L"Выйти из игры", font, 36, {1000.f, 800.f},
                [&window]() {window->close();}, false,
                sf::Color(255, 100, 100),    // Красный
                sf::Color(255, 0, 0))        // Ярко-красный
    };

    bool menuActive = true;
    sf::Clock animationClock;

    while (window->isOpen() && menuActive) {
        float time = animationClock.getElapsedTime().asSeconds();

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

            if (event.type == sf::Event::MouseMoved) {
                for (auto& item : pauseMenu) {
                    item.hovered = item.isMouseOver(*window);
                }
            }
        }

        for (auto& item : pauseMenu) {
            item.update(time);
        }

        drawStack(window);
        window->draw(overlay);
        window->draw(menuBorder);

        for (const auto& item : pauseMenu) {
            window->draw(item.text);
        }

        window->display();
    }
}

void renderWin(sf::RenderWindow* window, EnumGameState& gameState, uint8_t Level, std::function<void(sf::RenderWindow*)> drawStack) {
    sf::Font font;
    if (!font.loadFromFile("textures/font/PressStart2P-Regular.ttf")) {
        std::cerr << "Не удалось загрузить шрифт!\n";
        return;
    }

    // zatmenie
    sf::RectangleShape overlay(sf::Vector2f(window->getSize()));
    overlay.setFillColor(sf::Color(0, 50, 0, 150)); // green

    // for center
    sf::Vector2u windowSize = window->getSize();
    float menuWidth = 900.f;
    float menuHeight = 700.f;
    float menuX = (windowSize.x - menuWidth) / 2.f;
    float menuY = (windowSize.y - menuHeight) / 2.f;

    sf::RectangleShape menuBorder(sf::Vector2f(menuWidth, menuHeight));
    menuBorder.setFillColor(sf::Color(30, 30, 30));
    menuBorder.setOutlineColor(sf::Color(100, 255, 100, 200));
    menuBorder.setOutlineThickness(5.f);
    menuBorder.setPosition(menuX, menuY);


    // Создаем текст с номером уровня
    sf::String levelText = L"Поздравляем, вы прошли Level " + std::to_wstring(Level) + L"!";
    sf::String currentLevel = L"Уровень " + std::to_wstring(Level) + L" из 3 пройден!";

    // Определяем следующие действия в зависимости от уровня
    std::function<void()> nextAction;
    sf::String nextButtonText;

    if (Level < 3) {
        nextButtonText = L"Следующий уровень";
        nextAction = [&gameState]() { gameState = GAME;};
    }
    else {
        nextButtonText = L"Завершить игру";
        nextAction = [&window]() { window->close(); };
    }

    std::vector<MenuItem> winMenu = {
        MenuItem(L"Вы успешно отразили первую атаку", font, 24, {1000.f, 350.f}, []() {}, true,
        sf::Color(100, 255, 100), sf::Color(100, 255, 100)),

        MenuItem(L"Теперь враг рвётся с другого направления", font, 24, {1000.f, 400.f}, []() {}, true,
        sf::Color(100, 255, 100), sf::Color(100, 255, 100)),

        MenuItem(L"--->Следующий рубеж<---", font, 32, {1000.f, 500.f}, nextAction, false,
        sf::Color(100, 255, 100), sf::Color(0, 255, 0)),

        MenuItem(L"--->Попытаться снова<---", font, 32, {1000.f, 570.f}, [&gameState]() { gameState = GAME; }, false,
        sf::Color(255, 255, 100), sf::Color(255, 255, 0)),

        MenuItem(L"Техническая информация:", font, 16, {1000.f, 650.f}, []() {}, true,
        sf::Color(200, 200, 200), sf::Color(200, 200, 200)),

        MenuItem(L"Оператор: Alex, Убито мобов: 42", font, 14, {1000.f, 680.f}, []() {}, true,
        sf::Color(150, 150, 150), sf::Color(150, 150, 150)),

        MenuItem(L"--->Оставить на произвол судьбы<---", font, 32, {1000.f, 750.f}, [&window]() { window->close(); }, false,
        sf::Color(255, 100, 100), sf::Color(255, 0, 0))
    };

    bool menuActive = true;
    sf::Clock animationClock;

    while (window->isOpen() && menuActive) {
        float time = animationClock.getElapsedTime().asSeconds();

        sf::Event event;
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window->close();
                menuActive = false;
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                menuActive = false;
            }
            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left) {
                for (auto& item : winMenu) {
                    if (!item.gettitle() && item.isMouseOver(*window)) {
                        item.onClick();
                        menuActive = false;
                        break;
                    }
                }
            }

            if (event.type == sf::Event::MouseMoved) {
                for (auto& item : winMenu) {
                    item.hovered = item.isMouseOver(*window);
                }
            }
        }

        for (auto& item : winMenu) {
            item.update(time);
        }

        drawStack(window);
        window->draw(overlay);
        window->draw(menuBorder);

        for (const auto& item : winMenu) {
            window->draw(item.text);
        }

        window->display();
    }
}

void renderLose(sf::RenderWindow* window, EnumGameState& gameState, uint8_t Level, std::function<void(sf::RenderWindow*)> drawStack) {
    sf::Font font;
    if (!font.loadFromFile("textures/font/PressStart2P-Regular.ttf")) {
        std::cerr << "Не удалось загрузить шрифт!\n";
        return;
    }

    // zatmenie
    sf::RectangleShape overlay(sf::Vector2f(window->getSize()));
    overlay.setFillColor(sf::Color(50, 0, 0, 150)); //red

    // for center
    sf::Vector2u windowSize = window->getSize();
    float menuWidth = 900.f;
    float menuHeight = 700.f;
    float menuX = (windowSize.x - menuWidth) / 2.f;
    float menuY = (windowSize.y - menuHeight) / 2.f;

    sf::RectangleShape menuBorder(sf::Vector2f(menuWidth, menuHeight));
    menuBorder.setFillColor(sf::Color(30, 30, 30));
    menuBorder.setOutlineColor(sf::Color(255, 100, 100, 200));
    menuBorder.setOutlineThickness(5.f);
    menuBorder.setPosition(menuX, menuY);

    sf::String levelMessage = L"Уровень " + std::to_wstring(Level) + L" не пройден";

    std::vector<MenuItem> loseMenu = {
        MenuItem(L"Вы проиграли!", font, 36, {1000.f, 350.f}, []() {}, true,
        sf::Color(255, 100, 100), sf::Color(255, 100, 100)),

        MenuItem(levelMessage, font, 24, {1000.f, 420.f}, []() {}, true,
        sf::Color(255, 200, 100), sf::Color(255, 200, 100)),

        MenuItem(L"--->Попробовать снова<---", font, 32, {1000.f, 500.f}, [&gameState]() { gameState = GAME; }, false,
        sf::Color(255, 255, 100), sf::Color(255, 255, 0)),

        MenuItem(L"--->Выбрать уровень<---", font, 32, {1000.f, 570.f}, [&gameState]() { gameState = GAME; }, false,
        sf::Color(100, 200, 255), sf::Color(0, 150, 255)),

        MenuItem(L"Техническая информация:", font, 16, {1000.f, 650.f}, []() {}, true,
        sf::Color(200, 200, 200), sf::Color(200, 200, 200)),

        MenuItem(L"Оператор: Alex, Убито мобов: 23", font, 14, {1000.f, 680.f}, []() {}, true,
        sf::Color(150, 150, 150), sf::Color(150, 150, 150)),

        MenuItem(L"--->Оставить на произвол судьбы<---", font, 32, {1000.f, 750.f}, [&window]() { window->close(); }, false,
        sf::Color(255, 100, 100), sf::Color(255, 0, 0))
    };

    bool menuActive = true;
    sf::Clock animationClock;

    while (window->isOpen() && menuActive) {
        float time = animationClock.getElapsedTime().asSeconds();

        sf::Event event;
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window->close();
                menuActive = false;
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                menuActive = false;
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
                gameState = GAME;
                menuActive = false;
            }
            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left) {
                for (auto& item : loseMenu) {
                    if (!item.gettitle() && item.isMouseOver(*window)) {
                        item.onClick();
                        menuActive = false;
                        break;
                    }
                }
            }

            if (event.type == sf::Event::MouseMoved) {
                for (auto& item : loseMenu) {
                    item.hovered = item.isMouseOver(*window);
                }
            }
        }

        for (auto& item : loseMenu) {
            item.update(time);
        }

        drawStack(window);
        window->draw(overlay);
        window->draw(menuBorder);

        for (const auto& item : loseMenu) {
            window->draw(item.text);
        }

        window->display();
    }
}