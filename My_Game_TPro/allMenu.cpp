#include "allMenu.h"
#include "game.h"

    AdTimer::AdTimer(unsigned int delaySeconds) : adDelaySeconds(delaySeconds) {}

    bool AdTimer::canShowAd() const {
        if (lastAdTime.time_since_epoch().count() == 0) {
            return true;
        }

        auto now = std::chrono::steady_clock::now();
        auto timeSinceLastAd = std::chrono::duration_cast<std::chrono::seconds>(now - lastAdTime).count();
        return timeSinceLastAd >= adDelaySeconds;
    }

    void AdTimer::recordAdShown() {
        lastAdTime = std::chrono::steady_clock::now();
    }

    unsigned int AdTimer::getRemainingSeconds() const {
        if (lastAdTime.time_since_epoch().count() == 0) {
            return 0;
        }

        auto now = std::chrono::steady_clock::now();
        auto timeSinceLastAd = std::chrono::duration_cast<std::chrono::seconds>(now - lastAdTime).count();

        if (timeSinceLastAd >= adDelaySeconds) {
            return 0;
        }

        return adDelaySeconds - timeSinceLastAd;
    }

    void AdTimer::setDelay(unsigned int delaySeconds) {
        adDelaySeconds = delaySeconds;
    }

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

    // Автоматическое центрирование
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin(textBounds.left + textBounds.width / 2.0f,
        textBounds.top + textBounds.height / 2.0f);
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

void renderPause(sf::RenderWindow* window, EnumGameState& gameState, std::function<void(sf::RenderWindow*)> drawStack, AdTimer& adTimer) {
    sf::Font font;
    if (!font.loadFromFile("textures/font/PressStart2P-Regular.ttf")) {
        std::cerr << "Не удалось загрузить шрифт!\n";
        return;
    }

    // Затемнение
    sf::RectangleShape overlay(sf::Vector2f(window->getSize()));
    overlay.setFillColor(sf::Color(0, 0, 0, 150));

    // Для центрирования
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

    // Центр для текста
    float centerX = menuX + menuWidth / 2.f;
    float startY = menuY + 100.f;
    float itemSpacing = 80.f;

    // Создаем элементы меню
    sf::String adText = L"";
    std::vector<MenuItem> pauseMenu = {
        MenuItem(L"-->Продолжить<--", font, 36, {centerX, startY}, [&gameState]() {gameState = GAME;}, false,
                sf::Color(100, 255, 100), sf::Color(0, 255, 0)),

        MenuItem(L"Реклама(50 монет)", font, 36, {centerX, startY + itemSpacing}, [&gameState]() {gameState = AD;}, false,
                sf::Color(255, 255, 100), sf::Color(255, 255, 0)),

        MenuItem(adText, font, 20, {centerX, startY + 2 * itemSpacing}, []() {}, true,
                sf::Color(200, 200, 100), sf::Color(200, 200, 100)),

        MenuItem(L"Выйти из игры", font, 36, {centerX, startY + 4 * itemSpacing}, [&window]() {window->close();}, false,
                sf::Color(255, 100, 100), sf::Color(255, 0, 0))
    };

    // Центрируем весь текст
    for (auto& item : pauseMenu) {
        sf::FloatRect textBounds = item.text.getLocalBounds();
        item.text.setOrigin(textBounds.width / 2.f, textBounds.height / 2.f);
    }

    bool menuActive = true;
    sf::Clock animationClock;
    sf::Clock adUpdateClock;

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
                        // Проверка для кнопки рекламы
                        if (item.text.getString() == L"Реклама(50 монет)" && !adTimer.canShowAd()) {
                            // Не переходим в AD, если реклама недоступна
                            continue;
                        }
                        item.onClick();
                        if (gameState == GAME || gameState == AD) {
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

        // Обновляем анимации
        for (auto& item : pauseMenu) {
            item.update(time);
        }

        // Обновляем текст рекламы каждые 0.5 секунды
        if (adUpdateClock.getElapsedTime().asSeconds() > 0.5f) {
            if (adTimer.canShowAd()) {
                pauseMenu[2].text.setString(L"Доступно сейчас!");
                pauseMenu[2].text.setFillColor(sf::Color(100, 255, 100)); // Зеленый
            }
            else {
                unsigned int remaining = adTimer.getRemainingSeconds();
                pauseMenu[2].text.setString(L"Доступно через: " + std::to_wstring(remaining) + L" сек.");
                pauseMenu[2].text.setFillColor(sf::Color(255, 150, 100)); // Оранжевый
            }

            // Пере-центрируем текст после изменения
            sf::FloatRect textBounds = pauseMenu[2].text.getLocalBounds();
            pauseMenu[2].text.setOrigin(textBounds.width / 2.f, textBounds.height / 2.f);

            adUpdateClock.restart();
        }

        // Отрисовка
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

    float centerX = menuX + menuWidth / 2.f;
    float startY = menuY + 100.f;  // Начинаем ниже от верха
    float itemSpacing = 60.f;

    // Создаем текст с номером уровня
    sf::String levelText = L"Поздравляем, вы прошли Level " + std::to_wstring(Level) + L"!";
    sf::String currentLevel = L"Уровень " + std::to_wstring(Level) + L" из 3 пройден!";

    // Определяем следующие действия в зависимости от уровня
    std::function<void()> nextAction;
    sf::String nextButtonText;
    sf::String currentAttack;
    sf::String nextAttack;
    sf::String tryAgain;

    if (Level < 3) {
        nextButtonText = L"-->Следующий рубеж<--";
        currentAttack = L"Вы успешно отразили " + std::to_wstring(Level) + L" атаку!";
        nextAttack = L"Теперь враг рвётся с другого направления";
        tryAgain = L"-->Попробовать снова<--";
        nextAction = [&gameState]() { gameState = GAME;};
    }
    else {
        nextButtonText = L"-->Завершить игру<--";
        currentAttack = L"Вы успешно отразили все атаки!";
        nextAttack = L"Враг полностью повержен";
        tryAgain = L"Начать заново";
        nextAction = [&window]() { window->close(); };
    }

    std::vector<MenuItem> winMenu = {
        MenuItem(currentAttack, font, 24, {centerX, startY - 50.f}, []() {}, true,
        sf::Color(100, 255, 100), sf::Color(100, 255, 100)),

        MenuItem(nextAttack, font, 20, {centerX, startY + itemSpacing}, []() {}, true,
        sf::Color(100, 255, 100), sf::Color(100, 255, 100)),

        MenuItem(nextButtonText, font, 32, {centerX, startY + 3 * itemSpacing}, nextAction, false,
        sf::Color(100, 255, 100), sf::Color(0, 255, 0)),

        MenuItem(tryAgain, font, 32, {centerX, startY + 4 * itemSpacing}, [&gameState]() { gameState = GAME; }, false,
        sf::Color(255, 255, 100), sf::Color(255, 255, 0)),

        MenuItem(L"Техническая информация:", font, 16, {centerX, startY + 5 * itemSpacing}, []() {}, true,
        sf::Color(200, 200, 200), sf::Color(200, 200, 200)),

        MenuItem(L"Оператор: Alex, Убито мобов: 42", font, 14, {centerX, startY + 6 * itemSpacing}, []() {}, true,
        sf::Color(150, 150, 150), sf::Color(150, 150, 150)),

        MenuItem(L"-->Оставить на произвол судьбы<--", font, 24, {centerX, startY + 8 * itemSpacing}, [&window]() { window->close(); }, false,
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

    sf::String player = L"Unknown";
    sf::String Killed = L"0";

    //#######################jSON#######################
    try {
        std::ifstream scoreFile("config/score.json");
        if (scoreFile.is_open()) {
            nlohmann::json scoreData;
            scoreFile >> scoreData;
            scoreFile.close();

            if (scoreData.contains("players") && !scoreData["players"].empty()) {
                auto players = scoreData["players"];
                auto firstPlayer = players.begin();

                std::string nameStr = firstPlayer.key();
                player = sf::String::fromUtf8(nameStr.begin(), nameStr.end());

                if (!firstPlayer.value().empty()) {
                    auto firstSession = firstPlayer.value().begin();
                    if (firstSession.value().contains("enemies") &&
                        firstSession.value()["enemies"].contains("total_killed")) {
                        int killed = firstSession.value()["enemies"]["total_killed"];
                        Killed = std::to_wstring(killed);
                    }
                }
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка загрузки score.json: " << e.what() << std::endl;
        player = L"Ошибка";
        Killed = L"0";
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

    float centerX = menuX + menuWidth / 2.f;
    float startY = menuY + 100.f;
    float itemSpacing = 60.f;

    sf::String levelMessage = L"Вы не выдержали " + std::to_wstring(Level) + L" волну вирусов";

    std::vector<MenuItem> loseMenu = {
        MenuItem(L"FATAL ERROR", font, 36, {centerX, startY - 50.f}, []() {}, true,
        sf::Color(255, 100, 100), sf::Color(255, 100, 100)),

        MenuItem(levelMessage, font, 24, {centerX, startY + itemSpacing}, []() {}, true,
        sf::Color(255, 200, 100), sf::Color(255, 200, 100)),

        MenuItem(L"-->Попробовать снова<--", font, 32, {centerX, startY + 3 * itemSpacing}, [&gameState]() { gameState = GAME; }, false,
        sf::Color(255, 255, 100), sf::Color(255, 255, 0)),

        MenuItem(L"Техническая информация:", font, 16, {centerX, startY + 6 * itemSpacing}, []() {}, true,
        sf::Color(200, 200, 200), sf::Color(200, 200, 200)),

        MenuItem(L"Оператор:", font, 14, {centerX - 250.f, startY + 7 * itemSpacing - 30.f}, []() {}, true,
        sf::Color(150, 150, 150), sf::Color(150, 150, 150)),

        MenuItem(player, font, 14, {centerX + 250.f, startY + 7 * itemSpacing - 30.f}, []() {}, true,
        sf::Color(150, 150, 150), sf::Color(150, 150, 150)),

        MenuItem(L"Убито мобов:", font, 14, {centerX - 250.f, startY + 7 * itemSpacing}, []() {}, true,
        sf::Color(150, 150, 150), sf::Color(150, 150, 150)),

        MenuItem(Killed, font, 14, {centerX + 250.f, startY + 7 * itemSpacing}, []() {}, true,
        sf::Color(150, 150, 150), sf::Color(150, 150, 150)),

        MenuItem(L"-->Оставить на произвол судьбы<--", font, 24, {centerX, startY + 8 * itemSpacing}, [&window]() { window->close(); }, false,
        sf::Color(255, 100, 100), sf::Color(255, 0, 0))
    };

    //#######################ДЛЯ ТЕХ ИНФЫ#######################
    for (auto& item : loseMenu) {
        sf::FloatRect textBounds = item.text.getLocalBounds();
        if (item.text.getString() == L"Оператор:" || item.text.getString() == L"Убито мобов:") {
            item.text.setOrigin(0, textBounds.height / 2.f); //левишь
        }
        else if (item.text.getString() == L"Alex" || item.text.getString() == L"23") {
            item.text.setOrigin(textBounds.width, textBounds.height / 2.f); //правишь
        }
        else {
            //по базе
            item.text.setOrigin(textBounds.width / 2.f, textBounds.height / 2.f);
        }
    }

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