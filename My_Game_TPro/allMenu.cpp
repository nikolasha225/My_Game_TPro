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
        // Интенсивная пульсация цвета
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

void renderPause(sf::RenderWindow* window, EnumGameState& gameState) {
    sf::Font font;
    if (!font.loadFromFile("textures/font/PressStart2P-Regular.ttf")) {
        std::cerr << "Не удалось загрузить шрифт!\n";
        return;
    }

    // Для заднего фона
    sf::Texture gameScreenshot;
    gameScreenshot.create(window->getSize().x, window->getSize().y);
    gameScreenshot.update(*window);
    sf::Sprite backgroundSprite(gameScreenshot);

    // Затемнение
    sf::RectangleShape overlay(sf::Vector2f(window->getSize()));
    overlay.setFillColor(sf::Color(0, 0, 0, 150));

    sf::RectangleShape menuBorder(sf::Vector2f(800.f, 600.f));
    menuBorder.setFillColor(sf::Color(20, 20, 30, 200));
    menuBorder.setOutlineColor(sf::Color(100, 150, 100, 150));
    menuBorder.setOutlineThickness(3.f);
    menuBorder.setPosition(600.f, 300.f);

    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("textures/img/menupause.png")) {
        return;
    }
    sf::Sprite menuBackground(backgroundTexture);
    menuBackground.setPosition(600.f, 300.f);

    // Контрастные цвета для пульсации
    std::vector<MenuItem> pauseMenu = {
        MenuItem(L"Продолжить", font, 36, {830.f, 400.f},
                [&gameState]() {gameState = GAME;}, false,
                sf::Color(100, 255, 100),    // Ярко-зеленый
                sf::Color(0, 255, 0)),       // Очень яркий зеленый

        MenuItem(L"Выйти из игры", font, 36, {780.f, 700.f},
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

            // Обработка движения мыши для обновления наведения
            if (event.type == sf::Event::MouseMoved) {
                for (auto& item : pauseMenu) {
                    item.hovered = item.isMouseOver(*window);
                }
            }
        }

        // Обновляем анимации всех элементов
        for (auto& item : pauseMenu) {
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

void renderWin(sf::RenderWindow* window, EnumGameState& gameState, uint8_t Level) {
    sf::Font font;
    if (!font.loadFromFile("textures/font/PressStart2P-Regular.ttf")) {
        std::cerr << "Не удалось загрузить шрифт!\n";
        return;
    }

    // Затемнение с зеленым оттенком для победы
    sf::RectangleShape overlay(sf::Vector2f(window->getSize()));
    overlay.setFillColor(sf::Color(0, 50, 0, 150));

    sf::RectangleShape menuBorder(sf::Vector2f(800.f, 600.f));
    menuBorder.setFillColor(sf::Color(20, 40, 20, 200));
    menuBorder.setOutlineColor(sf::Color(100, 255, 100, 200));
    menuBorder.setOutlineThickness(5.f);
    menuBorder.setPosition(600.f, 300.f);

    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("textures/img/menupause.png")) {
        return;
    }
    sf::Sprite menuBackground(backgroundTexture);
    menuBackground.setPosition(600.f, 300.f);

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
              MenuItem(levelText, font, 28, {650.f, 350.f}, []() {}, true,
              sf::Color(100, 255, 100), sf::Color(100, 255, 100)),

              MenuItem(nextButtonText, font, 32, {700.f, 450.f}, nextAction, false,
              sf::Color(100, 255, 100), sf::Color(0, 255, 0)),
       
              MenuItem(L"Перезапустить уровень", font, 32, {680.f, 520.f}, [&gameState]() { gameState = GAME; }, false,
              sf::Color(255, 255, 100), sf::Color(255, 255, 0)),
              
              MenuItem(L"Выйти в меню", font, 32, {750.f, 590.f}, [&window]() { window->close(); }, false,
              sf::Color(255, 100, 100), sf::Color(255, 0, 0)),
                     
              MenuItem(currentLevel, font, 20, {700.f, 670.f}, []() {}, true,
              sf::Color(200, 200, 200), sf::Color(200, 200, 200))
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

        window->draw(overlay);
        window->draw(menuBorder);
        window->draw(menuBackground);

        for (const auto& item : winMenu) {
            window->draw(item.text);
        }

        window->display();
    }
}

void renderLose(sf::RenderWindow* window, EnumGameState& gameState, uint8_t Level) {
    sf::Font font;
    if (!font.loadFromFile("textures/font/PressStart2P-Regular.ttf")) {
        std::cerr << "Не удалось загрузить шрифт!\n";
        return;
    }

    // Затемнение с красным оттенком для проигрыша
    sf::RectangleShape overlay(sf::Vector2f(window->getSize()));
    overlay.setFillColor(sf::Color(50, 0, 0, 150));

    sf::RectangleShape menuBorder(sf::Vector2f(800.f, 600.f));
    menuBorder.setFillColor(sf::Color(40, 20, 20, 200));
    menuBorder.setOutlineColor(sf::Color(255, 100, 100, 200));
    menuBorder.setOutlineThickness(5.f);
    menuBorder.setPosition(600.f, 300.f);

    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("textures/img/menupause.png")) {
        return;
    }
    sf::Sprite menuBackground(backgroundTexture);
    menuBackground.setPosition(600.f, 300.f);

    sf::String levelMessage = L"Уровень " + std::to_wstring(Level) + L" не пройден";

    std::vector<MenuItem> loseMenu = {

        MenuItem(L"Вы проиграли!", font, 36, {750.f, 350.f}, []() {}, true,
        sf::Color(255, 100, 100), sf::Color(255, 100, 100)),

        MenuItem(levelMessage, font, 24, {700.f, 420.f}, []() {}, true,
        sf::Color(255, 200, 100), sf::Color(255, 200, 100)),

        MenuItem(L"Попробовать снова", font, 32, {700.f, 480.f}, [&gameState]() { gameState = GAME; }, false,
        sf::Color(255, 255, 100), sf::Color(255, 255, 0)),

        MenuItem(L"Выбрать уровень", font, 32, {710.f, 550.f}, [&gameState]() { gameState = GAME; }, false,
        sf::Color(100, 200, 255), sf::Color(0, 150, 255)),

        MenuItem(L"Выйти в меню", font, 32, {750.f, 620.f}, [&window]() { window->close(); }, false,
        sf::Color(255, 100, 100), sf::Color(255, 0, 0)),
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

        window->draw(overlay);
        window->draw(menuBorder);
        window->draw(menuBackground);

        for (const auto& item : loseMenu) {
            window->draw(item.text);
        }

        window->display();
    }
}