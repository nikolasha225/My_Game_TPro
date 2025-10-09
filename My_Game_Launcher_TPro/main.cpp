#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <functional>
#include <cmath> // для sin

//=============================ЗАМЕЧАНИЯ=======================================
//надо оформить в отдельный файл
//причём в .h в самом верху просто class MainMenu; а после уже class MainMenu {...}
//а в срр определение каждой функции
//и все инклюды тоже туда

class MenuItem {
public:
    sf::Text text;
    std::function<void()> onClick;
    bool hovered = false;

    MenuItem(const sf::String& label, sf::Font& font, unsigned int size, const sf::Vector2f& pos, std::function<void()> callback)
        : onClick(callback)
    {
        text.setFont(font);
        text.setString(label);
        text.setCharacterSize(size);
        text.setFillColor(sf::Color(180, 255, 180)); // мягкий зелёный
        text.setPosition(pos);
    }

    bool isMouseOver(const sf::RenderWindow& window) const {
        auto mouse = sf::Mouse::getPosition(window);
        auto bounds = text.getGlobalBounds();
        return bounds.contains(static_cast<float>(mouse.x), static_cast<float>(mouse.y));
    }

    void update(float time) {
        if (hovered) {
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
};

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