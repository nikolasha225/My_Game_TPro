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
        sf::Style::Default //Fullscreen/Titlebar/Default
    );
    window.setVerticalSyncEnabled(JSONSettings["GENERAL"]["vsync"]);
    window.setFramerateLimit(JSONSettings["GENERAL"]["framerate"]);
    
    sf::Image icon;
    if (icon.loadFromFile(JSONSettings["GENERAL"]["iconPath"])) {
        window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    }

    //static sf::Clock timer;

    LEVEL = 1;
    DIFFICULT = __difficult;
    HEALTH = JSONSettings["GAME"]["HP"][DIFFICULT - 1];
    MONEY = JSONSettings["GAME"]["startMoney"][DIFFICULT - 1];

    //базовые определения
    bool pauseState = 0;

    //отладка
    sf::Event event;
    OBJStack drawStack;
    Spawner spawner(&window, &drawStack);
    TowerManager manager(&drawStack);

    //Enemy enemy1(Enemy::bossVirus);
    //Enemy enemy4;
    //Enemy enemy2(Enemy::fastVirus);
    //Enemy enemy3(Enemy::tankVirus);


    //enemy1.multVelocity(3);

    //Bullet bullet(Tower::defender, &enemy3, sf::Vector2f(1000,1000));
    //for (auto i : towerPoint[LEVEL - 1]) {
    //    drawStack.add(new Tower(Tower::kaspersky, &drawStack, i));
    //}
    //Tower tower1(Tower::defender, &drawStack, sf::Vector2f(100, 500));
    //for (size_t i = 0; i < towerPoint[LEVEL - 1].size(); i++)
    //    drawStack.add(new Tower(Tower::defender, &drawStack, towerPoint[LEVEL-1][i]));
    //drawStack.add(&enemy1);
    //drawStack.add(&enemy2);
    //drawStack.add(&enemy4);
    //drawStack.add(&enemy3);
    //.add(&tower1);
    //drawStack.add(&bullet);

    std::vector<sf::RectangleShape> path = createSimplePath(wayPoints[LEVEL -1], 20);

    //===========================================
    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            // Пауза на ESC
            //if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
            //{
                // код паузы
            //}

            // НАЖАТИЕ ЛКМ
            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left)
            {
                manager.checkEvents(&window);
            }
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
        
        spawner.tick();
        drawStack.tick();
        

        window.clear(sf::Color::Black);
        //тут вся отрисовка

        //отладка
        drawStack.draw(&window);
        for(auto i : path)
            window.draw(i);

        window.display();
        TIME--;
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