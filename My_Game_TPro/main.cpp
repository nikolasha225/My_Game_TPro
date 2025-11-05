#include "game.h"
#include "winLoseMenu.h"
#include "pauseMenu.h"

int main(int argc, char* argv[])
{

    //базовые параметры 
    uint8_t __difficult = 1;
    unsigned __id = 0;
    uint8_t __level = 1;

    if (argc > 1) __difficult = static_cast<uint8_t>(std::stoi(argv[1]));
    if (argc > 2) __id = static_cast<unsigned>(std::stoi(argv[2]));
    if (argc > 3) __level = static_cast<uint8_t>(std::stoi(argv[3]));


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

    LEVEL = __level;
    DIFFICULT = __difficult;
    HEALTH = JSONSettings["GAME"]["startHP"][DIFFICULT - 1];
    MONEY = JSONSettings["GAME"]["startMoney"][DIFFICULT - 1];

    TIME = (float)JSONSettings["GAME"]["roundTimeSec"][LEVEL - 1]
        * (unsigned)JSONSettings["GENERAL"]["framerate"];
    START_TIME = TIME;

    //базовые определения

    EnumGameState GAME_STATE = GAME;

    bool TO_NEXT_LEVEL = 0;

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

    auto restartWithNewLevel = [&]() {
        window.close();

        // Получаем путь к текущему исполняемому файлу
        char exePath[MAX_PATH];
        GetModuleFileNameA(NULL, exePath, MAX_PATH);

        // Запускаем новую копию с новыми параметрами
        std::string command = std::string("\"") + exePath + "\" " +
            std::to_string(__difficult) + " " +
            std::to_string(__id) + " " +
            std::to_string(LEVEL);

        system(command.c_str());
        exit(0); // Завершаем текущий процесс
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

            (TIME<=0) ? (0) : (TIME--);

            GAME_STATE = ((TIME > 0) || OBJ_STACK.getStackOfType(enemy).size()) ? (GAME_STATE) : (NEXT_LEVEL);
            GAME_STATE = (HEALTH > 0) ? (GAME_STATE) : (LOSE);

            break;
        case PAUSE:
            OBJ_STACK.draw(&window);
            GAME_STATE = AD;
            break;
        case WIN:
            SOUND.setBuffer(SOUND_BUFFER_WIN);
            SOUND.play();
            writeScore(&OBJ_STACK, 123);
            GAME_STATE = END_GAME;
            break;
        case LOSE:
            SOUND.setBuffer(SOUND_BUFFER_LOSE);
            SOUND.play();
            writeScore(&OBJ_STACK, 123);
            GAME_STATE = END_GAME;
            HEALTH = 100;
            break;
        case AD:
            while (vatchAD(&VIDEO_PLAYER))
                GAME_STATE = GAME;
            break;
        case END_GAME:
            OBJ_STACK.draw(&window);
            break;
        case NEXT_LEVEL:
            LEVEL++;
            if (LEVEL > 3)
            {
                GAME_STATE = WIN;
                break;
            }
            OBJ_STACK.draw(&window);
            //---------------отрисовка менюшки перехода к следующему левелу

            //запуск себя с нечт левелом
            if(TO_NEXT_LEVEL)
                restartWithNewLevel();
            break;
        default:
            break;
        }

        window.display();
    }

    return 0;
}
