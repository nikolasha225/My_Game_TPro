#include "mainMenu.h"

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
	soundhello.play();

	sf::SoundBuffer bufferstart;
	if (!bufferstart.loadFromFile("assets/sound/startgame.wav")) {
		std::cerr << "No";
		return -1;
	}
	sf::Sound soundstart;
	soundstart.setBuffer(bufferstart);

	std::string screen = "main";
	bool needsRedraw = true;

	// ============= Функции пунктов меню =============
	auto startGame = [&soundstart]() {
		soundstart.play();
		};
	auto openSettings = [&screen]() {
		screen = "settings";
		};
	auto records = [&screen]() {
		screen = "records";
		};
	auto owners = [&screen]() {
		screen = "owners";
		};
	auto exitGame = [&window]() {
		window.close(); 
		};
	auto back = [&screen]() {
		screen = "main";
		};

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

	auto resolution = [] {

		};

	std::vector<MenuItem> mainmenu = {
		MenuItem(L"Guards of ELBRUS", font, 50, {100.f, 50.f}, []() {}, true),
		MenuItem(L"Старт", font, 36, { 100.f, 200.f }, startGame, false),
		MenuItem(L"Настройки", font, 36, { 100.f, 270.f }, openSettings, false),
		MenuItem(L"Таблица лидеров", font, 36, { 100.f, 340.f }, records, false),
		MenuItem(L"О создателях", font, 36, { 100.f, 410.f }, owners, false),
		MenuItem(L"Выход", font, 36, { 100.f, 480.f }, exitGame, false)
	};

	std::vector<MenuItem> settingsmenu = {
		MenuItem(L"Настройки", font, 50, {300.f, 50.f}, []() {}, true),

		MenuItem(L"Звук", font, 30, {100.f, 200.f}, []() {}, true),
		MenuItem(L"Вкл", font, 24, {700.f, 200.f}, audio, false),

		MenuItem(L"Разрешение", font, 30, {100.f, 270.f}, []() {}, true),
		MenuItem(L"1920 x 1080", font, 24, {700.f, 270.f}, resolution, false),
	// разрешение
		//music.setLoop(true);        // Зацикливание
	//music.setVolume(50.f);      // Громкость (0-100)
	//music.setPitch(1.0f);       // Высота тона

		MenuItem(L"Сохранить", font, 36, {350.f, 440.f}, back, false)
	};
	soundToggle = &settingsmenu[2];

	std::vector<MenuItem> recordsmenu = {
		MenuItem(L"Таблица рекордов", font, 50, {140.f, 50.f}, []() {}, true),
		MenuItem(L"Назад", font, 36, {100.f, 550.f}, back, false)

	};

	std::vector<MenuItem> ownersmenu = {
		MenuItem(L"О создателях", font, 50, {200.f, 50.f}, []() {}, true),
		MenuItem(L"Назад", font, 36, {100.f, 550.f}, back, false)
	};

	sf::Clock clock;

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

		window.display();
	}

	return 0;

}