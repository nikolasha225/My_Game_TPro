#include "functions.h"

int main(bool __volume = 1, uint8_t __difficult = 1, unsigned __id = 0)
{

    //базовые параметры (отладка)
    __difficult = 1;
    __id = 0;
    __volume = 1;


    // SF базовые преременные
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(
        desktopMode,
        (std::string)JSONSettings["GENERAL"]["title"],
        sf::Style::Fullscreen //Fullscreen/Titlebar/Default
    );
    window.setVerticalSyncEnabled(JSONSettings["GENERAL"]["vsync"]);
    window.setFramerateLimit(JSONSettings["GENERAL"]["framerate"]);

    //static sf::Clock timer;

    LEVEL = 1;
    DIFFICULT = __difficult;
    HEALTH = JSONSettings["GAME"]["HP"][DIFFICULT - 1];
    MONEY = JSONSettings["GAME"]["startMoney"][DIFFICULT - 1];

    //базовые определения
    bool pauseState = 0;

    //отладка
    OBJStack drawStack;

    Enemy enemy1(Enemy::miniBossVirus);
    Enemy enemy4;
    Enemy enemy2(Enemy::fastVirus);
    Enemy enemy3(Enemy::tankVirus);


    enemy1.setLayer(3);
    enemy2.setLayer(2);
    enemy3.setLayer(1);
    enemy4.setLayer(4);

    //enemy3.multVelocity(5);

    //Bullet bullet(Tower::defender, &enemy3, sf::Vector2f(1000,1000));
    Tower tower1(Tower::defender, &drawStack, sf::Vector2f(800, 500));

    drawStack.add(&enemy1);
    drawStack.add(&enemy2);
    drawStack.add(&enemy4);
    drawStack.add(&enemy3);
    drawStack.add(&tower1);
    //drawStack.add(&bullet);
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
       /* if (pauseState) {
            //чтото
            pauseState = puase(window*);
            window.clear(sf::Color::Black);
            window.display();
        }
        if (game(window&)) {
            //чтото
        }
        else {
        */
        drawStack.tick();



        window.clear(sf::Color::Black);
        //тут вся отрисовка

        //отладка
        drawStack.draw(&window);

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