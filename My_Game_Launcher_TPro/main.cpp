#include "mainMenu.h"

int main() {

	sf::RenderWindow window(sf::VideoMode(1020, 640), "LauncherMenu", sf::Style::None);
	sf::Image icon;
	if (icon.loadFromFile("assets/img/icon.png")) {
		window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	}
	window.setFramerateLimit(60);
	window.setVerticalSyncEnabled(1);
	sf::Font font;
	if (!font.loadFromFile("assets/fonts/PressStart2P-Regular.ttf")) {
		std::cerr << "Не удалось загрузить шрифт!\n";
		return 1;
	}
	std::string screen = "main";
	bool needsRedraw = true;

	// ============= Функции пунктов меню =============
	auto startGame = []() {
		     
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

	std::vector<MenuItem> mainmenu = {
		MenuItem(L"Старт", font, 36, { 100.f, 200.f }, startGame, false),
		MenuItem(L"Настройки", font, 36, { 100.f, 270.f }, openSettings, false),
		MenuItem(L"Таблица лидеров", font, 36, { 100.f, 340.f }, records, false),
		MenuItem(L"О создателях", font, 36, { 100.f, 410.f }, owners, false),
		MenuItem(L"Выход", font, 36, { 100.f, 480.f }, exitGame, false)
	};

	std::vector<MenuItem> settingsmenu = {
		MenuItem(L"Настройки", font, 50, {250.f, 50.f}, []() {}, true),

	};
	std::vector<MenuItem> recordsmenu = {
		
	};
	std::vector<MenuItem> ownersmenu = {

	};

	sf::Clock clock;

	while (window.isOpen()) {
		float time = clock.getElapsedTime().asSeconds();

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
						}
					}
				}
				else if (screen == "settings") {
					for (auto& item : settingsmenu) {
						if (item.isMouseOver(window)) {
							item.onClick();
							needsRedraw = true;
						}
					}
				}
				else if (screen == "records") {
					for (auto& item : recordsmenu) {
						if (item.isMouseOver(window)) {
							item.onClick();
							needsRedraw = true;
						}
					}
				}
				else if (screen == "owners") {
					for (auto& item : ownersmenu) {
						if (item.isMouseOver(window)) {
							item.onClick();
							needsRedraw = true;
						}
					}
				}
			}
		}

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
		window.clear(sf::Color(30, 30, 30)); //фон

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