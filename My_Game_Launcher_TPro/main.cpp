#include <SFML/Graphics.hpp>

int main()
{
    // Создаем окно
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();

    sf::RenderWindow window(
        desktopMode,
        "qwerty",
        sf::Style::Default //Fullscreen/Titlebar/Default
    );

    //базовые определения

    // Главный цикл
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

        window.clear(sf::Color::Black);
        //тут вся отрисовка
        //enemy.draw(&window);


        window.display();
    }

    return 0;
}