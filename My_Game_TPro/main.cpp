#include "game.h"
#include "allMenu.h"

int main(int argc, char* argv[])
{

    //������� ��������� 
    uint8_t __difficult = 1;
    unsigned __id = 0;
    uint8_t __level = 1;

    if (argc > 1) __difficult = static_cast<uint8_t>(std::stoi(argv[1]));
    if (argc > 2) __id = static_cast<unsigned>(std::stoi(argv[2]));
    if (argc > 3) __level = static_cast<uint8_t>(std::stoi(argv[3]));


    // SF ������� �����������
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
    AdTimer adTimer(JSONSettings["GAME"]["adDelay"]);

    //������� �����������

    EnumGameState GAME_STATE = GAME;

    sf::Event EVENT;
    OBJStack OBJ_STACK;
    Spawner SPAWNER(&window, &OBJ_STACK);
    TowerManager TOWER_MANAGER(&OBJ_STACK);

    VideoPlayer VIDEO_PLAYER;

    auto drawGameBackground = [&](sf::RenderWindow* wnd) {
        OBJ_STACK.draw(wnd);
        };

        sf::Sound SOUND;

    sf::SoundBuffer SOUND_BUFFER_WIN;
    sf::SoundBuffer SOUND_BUFFER_LOSE;
    sf::SoundBuffer SOUND_BUFFER_ROUND;

    SOUND_BUFFER_WIN.loadFromFile(JSONSettings["GAME"]["soundVictory"]);
    SOUND_BUFFER_LOSE.loadFromFile(JSONSettings["GAME"]["soundLose"]);
    SOUND_BUFFER_ROUND.loadFromFile(JSONSettings["GAME"]["soundRound"][LEVEL - 1]);
    
    SOUND.setBuffer(SOUND_BUFFER_ROUND);
    Sleep(100);
    SOUND.play();
    Sleep(100);

    auto restartWithNewLevel = [&]() {//Добафь кнопочкам функционал---------------------------------------------------------
        window.close();

        // �������� ���� � �������� ������������ �����
        char exePath[MAX_PATH];
        GetModuleFileNameA(NULL, exePath, MAX_PATH);

        // ��������� ����� ����� � ������ �����������
        std::string command = std::string("\"") + exePath + "\" " +
            std::to_string(__difficult) + " " +
            std::to_string(__id) + " " +
            std::to_string(LEVEL);

        system(command.c_str());
        exit(0); // ��������� ������� �������
        };

    //===========================================
    while (window.isOpen())
    {
        //############EVENTS#############
        while (window.pollEvent(EVENT))
        {
            if (EVENT.type == sf::Event::Closed)
                window.close();

            // pause menu
            if (EVENT.type == sf::Event::KeyPressed && EVENT.key.code == sf::Keyboard::Escape) {
                if (GAME_STATE == GAME)
                    GAME_STATE = PAUSE;
                else if(GAME_STATE == PAUSE)
                    GAME_STATE = GAME;
                break;
            }

            // tower check mouse
            if (EVENT.type == sf::Event::MouseButtonPressed &&
                EVENT.mouseButton.button == sf::Mouse::Left)
            {
                TOWER_MANAGER.checkEvents(&window);
            }

            if (OBJ_STACK.getCountOf(enemy) == 0 && TIME == 0 || (EVENT.type == sf::Event::KeyPressed && EVENT.key.code == sf::Keyboard::W)) {
                if(LEVEL == 3)
                    GAME_STATE = OVER;
                else
                    GAME_STATE = WIN;
                break;
            }
            if (HEALTH <= 0 || (EVENT.type == sf::Event::KeyPressed && EVENT.key.code == sf::Keyboard::L))
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

            (TIME<=0) ? (0) : (TIME--);

            GAME_STATE = ((TIME > 0) || OBJ_STACK.getStackOfType(enemy).size()) ? (GAME_STATE) : (NEXT_LEVEL);
            GAME_STATE = (HEALTH > 0) ? (GAME_STATE) : (LOSE);

            break;
        case PAUSE:
            OBJ_STACK.draw(&window);
            renderPause(&window, GAME_STATE, drawGameBackground, adTimer);
            break;
        case WIN:
        SOUND.setBuffer(SOUND_BUFFER_WIN);
        SOUND.play();
            LEVEL++;
            renderWin(&window, GAME_STATE, LEVEL-1, drawGameBackground);//TODO:переход на следующий левел !!! функция restartWithNewLevel
            break;
        case LOSE:
            SOUND.setBuffer(SOUND_BUFFER_LOSE);
            SOUND.play();
            writeScore(&OBJ_STACK, __id * 10 + 0);
            GAME_STATE = END_GAME;
            HEALTH = 100;
            renderLose(&window, GAME_STATE, LEVEL, drawGameBackground);
            break;
        case AD:
            renderAd(GAME_STATE, adTimer, VIDEO_PLAYER);
            break;
        case OVER:
            SOUND.setBuffer(SOUND_BUFFER_WIN);
            SOUND.play();
            GAME_STATE = END_GAME;
            renderOver(&window, GAME_STATE, drawGameBackground);
            break;
        default:
            break;
        }

        if (IS_RESTART) {
            writeScore(&OBJ_STACK, __id*10 + 1);
            restartWithNewLevel();
        }

        window.display();
    }

    return 0;
}
