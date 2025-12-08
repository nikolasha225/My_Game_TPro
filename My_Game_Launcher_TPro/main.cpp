#include "mainMenu.h"

int main() {

	sf::RenderWindow window(sf::VideoMode(1020, 640), "LauncherMenu", sf::Style::None);

	window.setFramerateLimit(60);
	window.setVerticalSyncEnabled(true);
	AdvancedMatrixBackground matrixBackground;

	//sound, font
	GameRes res;
	if (!loadAssets(window, res)) {
		return 1;
	}

	std::string screen = "main";
	bool needsRedraw = true;
	std::vector<MenuItem> recordsmenu;
	//============= function point menu =============
	bool isSoundOn = true;
	MenuItem* soundToggle = nullptr;

	// ============= SIMPLE ID =============
	std::string playerId = "1";
	bool editingId = false;
	sf::Clock cursorClock;
	bool showCursor = true;
	sf::Text idText;
	idText.setFont(res.font);
	idText.setCharacterSize(18);
	idText.setFillColor(sf::Color::White);
	idText.setString("ID: " + playerId);
	idText.setPosition(700.f, 570.f);

	auto audio = [&isSoundOn, &soundToggle, &res] {
		isSoundOn = !isSoundOn;
		if (soundToggle) {
			soundToggle->text.setString(isSoundOn ? L"Вкл" : L"Выкл");
		}
		res.soundclick.setVolume(isSoundOn ? 100.f : 0.f);
		res.soundstart.setVolume(isSoundOn ? 100.f : 0.f);
		};
	auto startGame = [&res, &window, &isSoundOn, &playerId](int difficulty) {
		Sleep(100);//for progryz
		res.soundstart.play();
		
		if (playerId.empty()) playerId = "1";
		std::string soundParam = isSoundOn ? "1" : "0"; // 1 - vkl  0 - vikl
		std::string diffParam = std::to_string(difficulty);
		system(("start \"\" /B " + std::string("My_Game_TPro.exe ") + diffParam + " " + playerId + " 1 " + soundParam + " >nul 2>&1").c_str());

		//system("My_Game_TPro.exe 1 7 1");//тут надо добавить параметры (глянь как они у меня идут, там id и уровень сложности) (просто через пробел как стринг добавить)
		window.close();
		exit(0);
		};

	auto easy = [&startGame]() {
		startGame(1);
		};
	auto medium = [&startGame]() {
		startGame(2);
		};
	auto hard = [&startGame]() {
		startGame(3);
		};
	auto difficulties = [&screen]() {
		screen = "difficulty";
		};
	auto openSettings = [&screen]() {
		screen = "settings";
		};
	auto records = [&screen, &recordsmenu, &res]() {
		updateRecords(recordsmenu, res.font);
		screen = "records";
		};
	auto owners = [&screen]() {
		screen = "owners";
		};
	auto exitGame = [&window]() {
		window.close(); 
		exit(0);
		};
	auto back = [&screen]() {
		screen = "main";
		};
	
	//==================MENU==================
	std::vector<MenuItem> mainmenu = createMain(res, difficulties, openSettings, records, owners, exitGame);
	std::vector<MenuItem> difficultyMenu = createDiff(res, easy, medium, hard, back);
	std::vector<MenuItem> settingsmenu = createSettings(res, soundToggle, audio, back);
	recordsmenu = createRecords(res, back);
	std::vector<MenuItem> ownersmenu = createOwners(res, back);

	sf::Clock clock;

	screen = "main";//na vsyakiy

	while (window.isOpen()) {
		float time = clock.getElapsedTime().asSeconds();
		float deltaTime = clock.restart().asSeconds();
		sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            //for id
            if (screen == "main") {
				handleMenu(event, window, screen, needsRedraw, res, mainmenu, settingsmenu, recordsmenu, ownersmenu, difficultyMenu, playerId, editingId, idText, cursorClock, showCursor);
            }
            else {
                //for other
                if (event.type == sf::Event::MouseButtonPressed &&
                    event.mouseButton.button == sf::Mouse::Left) {
                    clickMenu(window, screen, needsRedraw, res,
                        mainmenu, settingsmenu, recordsmenu,
                        ownersmenu, difficultyMenu);
                }
            }
        }

		matrixBackground.updating(deltaTime);
		//when naveli
		updateMenu(window, screen, time, mainmenu, settingsmenu, recordsmenu, ownersmenu, difficultyMenu, playerId, editingId, idText, cursorClock, showCursor);

		// ============= RENDER =============
		window.clear(sf::Color(30, 30, 30)); //fon navsyakiy
		matrixBackground.draw(window);
		sf::RectangleShape overlay(sf::Vector2f(1020, 640));
		overlay.setFillColor(sf::Color(0, 0, 0, 128)); //dark
		window.draw(overlay);

		drawMenu(window, screen, mainmenu, settingsmenu, recordsmenu, ownersmenu, difficultyMenu, idText, editingId, res);

		window.display();
	}

	return 0;

}