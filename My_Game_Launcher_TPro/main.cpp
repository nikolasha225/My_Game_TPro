#include "mainMenu.h"

int main() {

    sf::RenderWindow window(sf::VideoMode(1020, 640), "Хакерское меню");
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(1);
    sf::Font font;
    if (!font.loadFromFile("assets/fonts/PressStart2P-Regular.ttf")) {
        std::cerr << "Не удалось загрузить шрифт!\n";
        return 1;
    }

    // === Функции пунктов меню ===
    auto startGame = []() { std::cout << "Запуск игры!\n"; };
    auto openSettings = []() { std::cout << "Открытие настроек...\n"; };
    auto records = []() { std::cout << "Открытие настроек...\n"; };
    auto owners = []() { std::cout << "Открытие настроек...\n"; };
    auto exitGame = [&window]() { std::cout << "Выход из игры\n"; window.close(); };

    std::vector<MenuItem> menu = {
        MenuItem(L"Старт", font, 36, { 100.f, 200.f }, startGame),
        MenuItem(L"Настройки", font, 36, { 100.f, 270.f }, openSettings),
        MenuItem(L"Таблица лидеров", font, 36, { 100.f, 340.f }, records),
        MenuItem(L"О создателях", font, 36, { 100.f, 410.f }, owners),
        MenuItem(L"Выход", font, 36, { 100.f, 480.f }, exitGame)
    };

    sf::Clock clock;

    while (window.isOpen()) {
        float time = clock.getElapsedTime().asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                for (auto& item : menu) {
                    if (item.isMouseOver(window)) {
                        item.onClick();
                    }
                }
            }
        }

        // Проверяем наведение
        for (auto& item : menu) {
            item.hovered = item.isMouseOver(window);
            item.update(time);
        }

        // === Рендер ===
        window.clear(sf::Color(30, 30, 30)); //фон

        for (auto& item : menu)
            window.draw(item.text);

        window.display();
    }

    return 0;
}