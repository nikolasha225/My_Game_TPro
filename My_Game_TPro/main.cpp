#include "functions.h"

int main(bool volume = 1, uint8_t difficult = 2, unsigned id = 0)
{
    // SF базовые преременные
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(
        desktopMode,
        (std::string)JSONSettings["GENERAL"]["title"],
        sf::Style::Fullscreen //Fullscreen/Titlebar/Default
    );
    window.setVerticalSyncEnabled(JSONSettings["GENERAL"]["vsync"]);
    window.setFramerateLimit(JSONSettings["GENERAL"]["framerate"]);

    static sf::Clock timer;

    //базовые определения
    bool pauseState = 0;

    //отладка
    OBJStack drawStack;

    Enemy enemy1;
    Enemy enemy2(Enemy::fastVirus);
    Enemy enemy3;

    enemy2.multVelocity(2);
    enemy3.multVelocity(5);
    enemy1.multVelocity(1);

    enemy1.setLayer(3);
    enemy2.setLayer(2);
    enemy3.setLayer(1);

    drawStack.add(&enemy1);
    drawStack.add(&enemy2);
    drawStack.add(&enemy3);
    //===========================================
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            //другие события
            //  пауза на esc
        }
        //логика игры
       /* while (pauseState) {
            //чтото
            pauseState = puase(window*);
            window.clear(sf::Color::Black);
            window.display();
        }
        if (game(window&)) {
            //чтото
        }*/
        enemy1.tick();
        enemy2.tick();
        enemy3.tick();

        window.clear(sf::Color::Black);
        //тут вся отрисовка

        //отладка
        drawStack.draw(&window);


        window.display();
    }

    return 0;
}