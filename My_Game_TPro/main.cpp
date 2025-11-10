#include "game.h"
#include "allMenu.h"

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
    FONT.loadFromFile(JSONSettings["GENERAL"]["font"]);

    LEVEL = 1;
    DIFFICULT = __difficult;
    HEALTH = JSONSettings["GAME"]["HP"][DIFFICULT - 1];
    MONEY = JSONSettings["GAME"]["startMoney"][DIFFICULT - 1];

    TIME = (float)JSONSettings["GAME"]["roundTimeSec"][LEVEL - 1]
        * (unsigned)JSONSettings["GENERAL"]["framerate"];
    START_TIME = TIME;

    //базовые определения

    EnumGameState GAME_STATE = GAME;

    sf::Event EVENT;
    OBJStack OBJ_STACK;
    Spawner SPAWNER(&window, &OBJ_STACK);
    TowerManager TOWER_MANAGER(&OBJ_STACK);

    VideoPlayer VIDEO_PLAYER;

    auto drawGameBackground = [&](sf::RenderWindow* wnd) {
        OBJ_STACK.draw(wnd);
        };

    //===========================================
    while (window.isOpen())
    {
        //############EVENTS#############
        while (window.pollEvent(EVENT))
        {
            if (EVENT.type == sf::Event::Closed)
                window.close();

            // Пауза на ESC
            if (EVENT.type == sf::Event::KeyPressed && EVENT.key.code == sf::Keyboard::Escape)
                if (GAME_STATE == GAME)
                    GAME_STATE = PAUSE;
                else if (GAME_STATE == PAUSE)
                    GAME_STATE = GAME;

            // НАЖАТИЕ ЛКМ
            if (EVENT.type == sf::Event::MouseButtonPressed &&
                EVENT.mouseButton.button == sf::Mouse::Left)
            {
                TOWER_MANAGER.checkEvents(&window);
            }

            if (EVENT.type == sf::Event::KeyPressed && EVENT.key.code == sf::Keyboard::W)
                GAME_STATE = WIN;
            if (EVENT.type == sf::Event::KeyPressed && EVENT.key.code == sf::Keyboard::L)
                GAME_STATE = LOSE;
        }

        //##########CLEAR####################

        window.clear(sf::Color::Black);

        //###############TICK#################
        switch (GAME_STATE)
        {
        case GAME:
            SPAWNER.tick();

            OBJ_STACK.tick();

            OBJ_STACK.draw(&window);

            TIME--;
            break;
        case PAUSE:
            OBJ_STACK.draw(&window);
            renderPause(&window, GAME_STATE, drawGameBackground);
            break;
        case WIN:
            renderWin(&window, GAME_STATE, LEVEL, drawGameBackground);
            break;
        case LOSE:
            renderLose(&window, GAME_STATE, LEVEL, drawGameBackground);
            break;
        case AD:
            while (vatchAD(&VIDEO_PLAYER))
                GAME_STATE = GAME;
                MONEY += 25;

                //????????????????????????????????????
            break;
        default:
            break;
        }


        window.display();
    }

    return 0;
}
