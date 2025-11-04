#include "game.h"
#include "winLoseMenu.h"
#include "pauseMenu.h"

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
    HEALTH = JSONSettings["GAME"]["startHP"][DIFFICULT - 1];
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

    sf::Sound SOUND;

    sf::SoundBuffer SOUND_BUFFER_WIN;
    sf::SoundBuffer SOUND_BUFFER_LOSE;
    sf::SoundBuffer SOUND_BUFFER_ROUND;

    SOUND_BUFFER_WIN.loadFromFile(JSONSettings["GAME"]["soundVictory"]);
    SOUND_BUFFER_LOSE.loadFromFile(JSONSettings["GAME"]["soundLose"]);
    SOUND_BUFFER_ROUND.loadFromFile(JSONSettings["GAME"]["soundRound"][LEVEL - 1]);
    
    SOUND.setBuffer(SOUND_BUFFER_ROUND);
    SOUND.play();
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

            GAME_STATE = (TIME > 0 && SPAWNER.existEnemy()) ? (GAME_STATE) : (WIN);
            GAME_STATE = (HEALTH > 0) ? (GAME_STATE) : (LOSE);

            break;
        case PAUSE:
            OBJ_STACK.draw(&window);
            GAME_STATE = AD;
            break;
        case WIN:
            SOUND.setBuffer(SOUND_BUFFER_WIN);
            SOUND.play();

            GAME_STATE = GAME;
            break;
        case LOSE:
            SOUND.setBuffer(SOUND_BUFFER_LOSE);
            SOUND.play();

            GAME_STATE = GAME;
            HEALTH = 100;
            break;
        case AD:
            while (vatchAD(&VIDEO_PLAYER))
                GAME_STATE = GAME;
            break;
        default:
            break;
        }

        window.display();
    }

    return 0;
}
