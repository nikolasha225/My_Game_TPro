#include "functions.h"

int main()
{
    // —оздаем окно
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();

    sf::RenderWindow window(
        desktopMode,
        (std::string)JSONSettings["GENERAL"]["title"],
        sf::Style::Fullscreen //Titelbar/Default
    );

    //базовые определени€
    bool pauseState = 0;
    // √лавный цикл
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            //другие событи€
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
        window.display();
    }

    return 0;
}