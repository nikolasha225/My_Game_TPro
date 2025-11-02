#include "game.h"

int main(uint8_t __difficult = 1, unsigned __id = 0)
{


    //базовые параметры (отладка)
    __difficult = 1;
    __id = 0;


    // SF базовые преременные
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(
        desktopMode,
        (std::string)JSONSettings["GENERAL"]["title"],
        sf::Style::Fullscreen //Fullscreen/Titlebar/Default
    );
    window.setVerticalSyncEnabled(JSONSettings["GENERAL"]["vsync"]);
    window.setFramerateLimit(JSONSettings["GENERAL"]["framerate"]);
    
    sf::Image icon;
    if (icon.loadFromFile(JSONSettings["GENERAL"]["iconPath"])) {
        window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    }

    LEVEL = 1;
    DIFFICULT = __difficult;
    HEALTH = JSONSettings["GAME"]["HP"][DIFFICULT - 1];
    MONEY = JSONSettings["GAME"]["startMoney"][DIFFICULT - 1];

    //базовые определения
    bool pauseState = 0;
    sf::Event event;
    OBJStack drawStack;
    Spawner spawner(&window, &drawStack);
    TowerManager manager(&drawStack);

    //пока ктото карту не сделал
    std::vector<sf::RectangleShape> path = createSimplePath(wayPoints[LEVEL -1], 20);

    //===========================================
    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            // Пауза на ESC
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
            {
            pauseState = !pauseState;
            }

            // НАЖАТИЕ ЛКМ
            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left)
            {
                manager.checkEvents(&window);
            }
        }
        window.clear(sf::Color::Black);
        //логика игры
        if (!pauseState) {
            spawner.tick();
            drawStack.tick();


            drawStack.draw(&window);

            //отладка
            for (auto& i : path)
                window.draw(i);

            TIME--;
        }
        else {
            //меню паузы
        }
        window.display();
    }

    return 0;
}

/*
getElapsedTime() - возвращает sf::Time прошедшее время

sf::Time elapsed = timer.getElapsedTime();

float seconds = timer.getElapsedTime().asSeconds();
int milliseconds = timer.getElapsedTime().asMilliseconds();
int microseconds = timer.getElapsedTime().asMicroseconds();


restart() - перезапускает и возвращает прошедшее время

sf::Time elapsed = timer.restart();
*/