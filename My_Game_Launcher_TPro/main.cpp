#include "mainMenu.h"
#include "Windows.h"

int main() {

	sf::RenderWindow window(sf::VideoMode(1020, 640), "LauncherMenu", sf::Style::None);
	sf::Image icon;
	if (icon.loadFromFile("assets/img/icon.png")) {
		window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	}

	window.setFramerateLimit(60);
	window.setVerticalSyncEnabled(true);

	AdvancedMatrixBackground matrixBackground;

	sf::Font font;
	if (!font.loadFromFile("assets/fonts/PressStart2P-Regular.ttf")) {
		std::cerr << "Не удалось загрузить шрифт!\n";
		return 1;
	}

	sf::SoundBuffer bufferclick;
	if (!bufferclick.loadFromFile("assets/sound/click.wav")) {
		std::cerr << "Не удалось загрузить аудиофайл!" << std::endl;
		return -1;
	}
	sf::Sound soundclick;
	soundclick.setBuffer(bufferclick);

	sf::SoundBuffer bufferhello;
	if (!bufferhello.loadFromFile("assets/sound/hello.wav")) {
		std::cerr << "No";
		return 1;
	}
	sf::Sound soundhello;
	soundhello.setBuffer(bufferhello);
	soundhello.setVolume(25);
	Sleep(100);//надо чтоб прогрузилась
	soundhello.play();
	Sleep(100);//надо чтоб прогрузилась

	sf::SoundBuffer bufferstart;
	if (!bufferstart.loadFromFile("assets/sound/startgame.wav")) {
		std::cerr << "No";
		return -1;
	}
	sf::Sound soundstart;
	soundstart.setBuffer(bufferstart);

	std::string screen = "main";
	bool needsRedraw = true;
	std::vector<MenuItem> recordsmenu;
	// ============= Функции пунктов меню =============
	bool isSoundOn = true;
	MenuItem* soundToggle = nullptr;

	auto audio = [&isSoundOn, &soundToggle, &soundclick, &soundstart] {
		isSoundOn = !isSoundOn;
		if (soundToggle) {
			soundToggle->text.setString(isSoundOn ? L"Вкл" : L"Выкл");
		}
		soundclick.setVolume(isSoundOn ? 100.f : 0.f);
		soundstart.setVolume(isSoundOn ? 100.f : 0.f);
		};
	auto startGame = [&soundstart, &window, &isSoundOn](int difficulty) {
		Sleep(100);//надо чтоб прогрузилась
		soundstart.play();

		std::string soundParam = isSoundOn ? "1" : "0"; // передавать 1 - вкл  0 - выкл
		std::string diffParam = std::to_string(difficulty); // тоже надо передать как то
		system(("start \"\" /B " + std::string("My_Game_TPro.exe 1 7 1") + " >nul 2>&1").c_str());

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
	auto records = [&screen, &recordsmenu, &font]() {
		updateRecords(recordsmenu, font);
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

	
	
	std::vector<MenuItem> difficultyMenu = {
		MenuItem(L"Выберите сложность", font, 36, {200.f, 70.f}, []() {}, true),
		MenuItem(L"Легко", font, 30, {150.f, 180.f}, easy, false),
		MenuItem(L"Больше денег, меньше врагов", font, 18, {150.f, 220.f}, []() {}, true),

		MenuItem(L"Средне", font, 30, {150.f, 280.f}, medium, false),
		MenuItem(L"Стандартная сложность", font, 18, {150.f, 320.f}, []() {}, true),

		MenuItem(L"Сложно", font, 30, {150.f, 380.f}, hard, false),
		MenuItem(L"Мало денег, много сильных врагов", font, 18, {150.f, 420.f}, []() {}, true),

		MenuItem(L"Назад", font, 36, {100.f, 550.f}, back, false)
	};
	std::vector<MenuItem> mainmenu = {
		MenuItem(L"Guards of ELBRUS", font, 50, {100.f, 50.f}, []() {}, true),
		MenuItem(L"Старт", font, 36, { 100.f, 200.f }, difficulties, false),
		MenuItem(L"Настройки", font, 36, { 100.f, 270.f }, openSettings, false),
		MenuItem(L"Таблица лидеров", font, 36, { 100.f, 340.f }, records, false),
		MenuItem(L"О создателях", font, 36, { 100.f, 410.f }, owners, false),
		MenuItem(L"Выход", font, 36, { 100.f, 480.f }, exitGame, false)
	};

	std::vector<MenuItem> settingsmenu = {
		MenuItem(L"Настройки", font, 50, {300.f, 50.f}, []() {}, true),
		MenuItem(L"Громкость", font, 30, {100.f, 200.f}, []() {}, true),
		MenuItem(L"Вкл", font, 24, {800.f, 200.f}, audio, false),
		MenuItem(L"Сохранить", font, 36, {350.f, 500.f}, back, false)
	};
	soundToggle = &settingsmenu[2];

	recordsmenu = {
		MenuItem(L"Таблица рекордов", font, 50, {140.f, 50.f}, []() {}, true),
		MenuItem(L"Назад", font, 36, {100.f, 550.f}, back, false)
	};

	std::vector<MenuItem> ownersmenu = {
		MenuItem(L"О создателях", font, 50, {200.f, 50.f}, []() {}, true),
		MenuItem(L"Ducktor74", font, 28, {100.f, 150.f}, []() {}, true),
		MenuItem(L"Owner/Dev", font, 22, {650.f, 150.f}, []() {}, true),
		MenuItem(L"Bucktor74", font, 28, {100.f, 200.f}, []() {}, true),
		MenuItem(L"Designer/Dev", font, 22, {650.f, 200.f}, []() {}, true),
		MenuItem(L"Broguss", font, 28, {100.f, 250.f}, []() {}, true),
		MenuItem(L"Developer", font, 22, {650.f, 250.f}, []() {}, true),
		MenuItem(L"Назад", font, 36, {100.f, 550.f}, back, false)
	};


	sf::Clock clock;

	screen = "main";//на всякий

	while (window.isOpen()) {
		float time = clock.getElapsedTime().asSeconds();
		float deltaTime = clock.restart().asSeconds();
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
			
			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
				if (screen == "main") {
					for (auto& item : mainmenu) {
						if (item.isMouseOver(window)) {
							item.onClick();
							needsRedraw = true;
							if (!item.gettitle()) {
								soundclick.play();
							}
						}
					}
				}
				else if (screen == "settings") {
					for (auto& item : settingsmenu) {
						if (item.isMouseOver(window)) {
							item.onClick();
							needsRedraw = true;
							if (!item.gettitle()) {
								soundclick.play();
							}
						}
					}
				}
				else if (screen == "records") {
					for (auto& item : recordsmenu) {
						if (item.isMouseOver(window)) {
							item.onClick();
							needsRedraw = true;
							if (!item.gettitle()) {
								soundclick.play();
							}
						}
					}
				}
				else if (screen == "owners") {
					for (auto& item : ownersmenu) {
						if (item.isMouseOver(window)) {
							item.onClick();
							needsRedraw = true;
							if (!item.gettitle()) {
								soundclick.play();
							}
						}
					}
				}
				else if (screen == "difficulty") {
					for (auto& item : difficultyMenu) {
						if (item.isMouseOver(window)) {
							item.onClick();
							needsRedraw = true;
							if (!item.gettitle()) {
								soundclick.play();
							}
						}
					}
				}
			}
		}

		matrixBackground.updating(deltaTime);
		// Проверяем наведение

		if (screen == "main") {
			for (auto& item : mainmenu) {
				item.hovered = item.isMouseOver(window);
				if (item.isMouseOver(window) != item.hovered);
				item.update(time);
			}
		}
		else if (screen == "settings") {
			for (auto& item : settingsmenu) {
				item.hovered = item.isMouseOver(window);
				if (item.isMouseOver(window) != item.hovered);
				item.update(time);
			}
		}
		else if (screen == "records") {
			for (auto& item : recordsmenu) {
				item.hovered = item.isMouseOver(window);
				if (item.isMouseOver(window) != item.hovered);
				item.update(time);
			}
		}
		else if (screen == "owners") {
			for (auto& item : ownersmenu) {
				item.hovered = item.isMouseOver(window);
				if (item.isMouseOver(window) != item.hovered);
				item.update(time);
			}
		}
		else if (screen == "difficulty") {
			for (auto& item : difficultyMenu) {
				item.hovered = item.isMouseOver(window);
				if (item.isMouseOver(window) != item.hovered);
				item.update(time);
			}
		}

		// ============= Рендер =============
		window.clear(sf::Color(30, 30, 30)); //изначальный фон
		matrixBackground.draw(window);
		sf::RectangleShape overlay(sf::Vector2f(1020, 640));
		overlay.setFillColor(sf::Color(0, 0, 0, 128)); // Полупрозрачный черный
		window.draw(overlay);

		if (screen == "main") {
			for (auto& item : mainmenu)
				window.draw(item.text);
		}
		else if (screen == "settings") {
			for (auto& item : settingsmenu)
				window.draw(item.text);
		}
		else if (screen == "records") {
			for (auto& item : recordsmenu)
				window.draw(item.text);
		}
		else if (screen == "owners") {
			for (auto& item : ownersmenu)
				window.draw(item.text);
		}
		else if (screen == "difficulty") {
			for (auto& item : difficultyMenu)
				window.draw(item.text);
		}

		window.display();
	}

	return 0;

}