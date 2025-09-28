#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>
#include <fstream>

using json = nlohmann::json;

using namespace sf;

int main()
{
    json config;
    config["window_width"] = 800;
    config["window_height"] = 600;
    config["game_title"] = "My Game";

    std::ofstream file("config.json");
    file << config.dump(4); // красивый вывод с отступами
    RenderWindow window(VideoMode(400, 400), L"Ќовый проект", Style::Default);

    window.setVerticalSyncEnabled(true);

    CircleShape shape(100.f, 3);
    shape.setPosition(100, 100);
    shape.setFillColor(Color::Magenta);

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }

        window.clear(Color::Blue);
        window.draw(shape);
        window.display();
    }
    return 0;
}