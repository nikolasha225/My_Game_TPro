#include <SFML/Graphics.hpp>
#include <vector>
#include <sstream>
#include <iostream>

const std::vector<sf::Vector2f> wayPoints[3] =
{
    {
        sf::Vector2f(200, 980),   // start
        sf::Vector2f(200, 300),   // point1
        sf::Vector2f(1720, 300),  // point2  
        sf::Vector2f(1720, 880),  // point3
        sf::Vector2f(500, 880),   // point4
        sf::Vector2f(500, 520),   // point5
        sf::Vector2f(1520, 520),  // point6
        sf::Vector2f(1520, 600)   // end
    },
    {
        sf::Vector2f(200, 980),   // start
        sf::Vector2f(580, 300),   // point1
        sf::Vector2f(960, 980),   // point2  
        sf::Vector2f(1340, 300),  // point3
        sf::Vector2f(1720, 880)   // end
    },
    {
        sf::Vector2f(200, 980),   // start
        sf::Vector2f(200, 300),   // point1
        sf::Vector2f(1720, 300),  // point2
        sf::Vector2f(1720, 600)   // end
    }
};

int main()
{
    // Создаем окно
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "WayPoints Visualization - Press 1,2,3 to switch paths");
    window.setFramerateLimit(60);

    // Цвета для каждого пути
    sf::Color pathColors[3] = {
        sf::Color::Red,      // Путь 1
        sf::Color::Green,    // Путь 2
        sf::Color::Blue      // Путь 3
    };

    int currentPath = 0; // Текущий отображаемый путь (0, 1, 2)
    bool showAllPaths = false; // Показывать все пути или только один

    // Пробуем загрузить шрифт
    sf::Font font;
    bool fontLoaded = false;

    // Пробуем разные пути к шрифту
    const char* fontPaths[] = {
        "arial.ttf",
        "C:/Windows/Fonts/arial.ttf",
        "/usr/share/fonts/truetype/freefont/FreeMono.ttf"  // для Linux
    };

    for (const char* fontPath : fontPaths) {
        if (font.loadFromFile(fontPath)) {
            fontLoaded = true;
            break;
        }
    }

    // Основной цикл
    while (window.isOpen())
    {
        // Получаем позицию курсора мыши
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        // Обработка событий
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Num1)
                    currentPath = 0;
                else if (event.key.code == sf::Keyboard::Num2)
                    currentPath = 1;
                else if (event.key.code == sf::Keyboard::Num3)
                    currentPath = 2;
                else if (event.key.code == sf::Keyboard::A)
                    showAllPaths = !showAllPaths; // Переключение режима показа всех путей
            }
        }

        // Очистка экрана
        window.clear(sf::Color::White);

        // Рисуем сетку для удобства
        for (int x = 0; x <= 1920; x += 100)
        {
            sf::Vertex line[] = {
                sf::Vertex(sf::Vector2f(x, 0), sf::Color(200, 200, 200)),
                sf::Vertex(sf::Vector2f(x, 1080), sf::Color(200, 200, 200))
            };
            window.draw(line, 2, sf::Lines);
        }
        for (int y = 0; y <= 1080; y += 100)
        {
            sf::Vertex line[] = {
                sf::Vertex(sf::Vector2f(0, y), sf::Color(200, 200, 200)),
                sf::Vertex(sf::Vector2f(1920, y), sf::Color(200, 200, 200))
            };
            window.draw(line, 2, sf::Lines);
        }

        // Определяем какие пути рисовать
        int startPath = showAllPaths ? 0 : currentPath;
        int endPath = showAllPaths ? 3 : currentPath + 1;

        // Рисуем выбранные пути
        for (int pathIndex = startPath; pathIndex < endPath; ++pathIndex)
        {
            const auto& path = wayPoints[pathIndex];
            sf::Color pathColor = pathColors[pathIndex];

            // Рисуем линии между точками
            for (size_t i = 0; i < path.size() - 1; ++i)
            {
                sf::Vertex line[] = {
                    sf::Vertex(path[i], pathColor),
                    sf::Vertex(path[i + 1], pathColor)
                };
                window.draw(line, 2, sf::Lines);
            }

            // Рисуем точки
            for (size_t i = 0; i < path.size(); ++i)
            {
                sf::CircleShape point(8);
                point.setFillColor(pathColor);
                point.setOrigin(8, 8); // Центрируем точку
                point.setPosition(path[i]);
                window.draw(point);

                // Подписи точек (только если шрифт загружен)
                if (fontLoaded)
                {
                    sf::Text label;
                    label.setFont(font);
                    label.setString("P" + std::to_string(pathIndex + 1) + "-" + std::to_string(i));
                    label.setCharacterSize(14);
                    label.setFillColor(sf::Color::Black);
                    label.setPosition(path[i].x + 15, path[i].y - 15);
                    window.draw(label);
                }
            }
        }

        // Отображаем информацию о текущем режиме и координаты
        if (fontLoaded)
        {
            // Информация о режиме
            sf::Text infoText;
            infoText.setFont(font);

            std::string infoString;
            if (showAllPaths) {
                infoString = "Showing ALL paths (Press A to switch)";
            }
            else {
                infoString = "Showing Path " + std::to_string(currentPath + 1) + " (Press 1,2,3 to switch, A for all)";
            }

            infoText.setString(infoString);
            infoText.setCharacterSize(24);
            infoText.setFillColor(sf::Color::Black);
            infoText.setPosition(50, 30);
            window.draw(infoText);

            // Легенда цветов (только в режиме показа всех путей)
            if (showAllPaths)
            {
                for (int i = 0; i < 3; ++i)
                {
                    sf::Text legendText;
                    legendText.setFont(font);
                    legendText.setString("Path " + std::to_string(i + 1));
                    legendText.setCharacterSize(20);
                    legendText.setFillColor(pathColors[i]);
                    legendText.setPosition(50, 70 + i * 30);
                    window.draw(legendText);
                }
            }

            // Координаты курсора
            std::stringstream coordStream;
            coordStream << "Mouse: (" << mousePos.x << ", " << mousePos.y << ")";

            sf::Text mouseCoordText;
            mouseCoordText.setFont(font);
            mouseCoordText.setString(coordStream.str());
            mouseCoordText.setCharacterSize(20);
            mouseCoordText.setFillColor(sf::Color::Black);
            mouseCoordText.setPosition(50, 1080 - 40); // В нижнем левом углу
            window.draw(mouseCoordText);
        }
        else
        {
            // Альтернативный способ отображения координат без шрифта
            // Рисуем прямоугольник с координатами
            std::stringstream coordStream;
            coordStream << "X: " << mousePos.x << " Y: " << mousePos.y;

            // Простой вывод в консоль (для отладки)
            static sf::Vector2i lastMousePos(-1, -1);
            if (mousePos != lastMousePos) {
                std::cout << "Mouse coordinates: (" << mousePos.x << ", " << mousePos.y << ")" << std::endl;
                lastMousePos = mousePos;
            }

            // Рисуем простой индикатор координат (квадратик с цветом)
            sf::RectangleShape coordIndicator(sf::Vector2f(20, 20));
            coordIndicator.setFillColor(sf::Color(mousePos.x % 255, mousePos.y % 255, 128));
            coordIndicator.setPosition(1800, 20);
            window.draw(coordIndicator);
        }

        // Отображаем все на экране
        window.display();
    }

    return 0;
}