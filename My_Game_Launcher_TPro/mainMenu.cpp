#include "mainMenu.h"

    MenuItem::MenuItem(const sf::String& label, sf::Font& font, unsigned int size, const sf::Vector2f& pos, std::function<void()> callback, bool title)
        : onClick(callback), title(title)
    {
        text.setFont(font);
        text.setString(label);
        text.setCharacterSize(size);
        text.setFillColor(sf::Color(180, 255, 180)); //light green
        text.setPosition(pos);
    }

    bool MenuItem::isMouseOver(const sf::RenderWindow& window) const {
        auto mouse = sf::Mouse::getPosition(window);
        auto bounds = text.getGlobalBounds();
        return bounds.contains(static_cast<float>(mouse.x), static_cast<float>(mouse.y));
    }

    void MenuItem::update(float time) {
        if (hovered && !title) {
            //pulse plavno green
            float pulse = (std::sin(time * 3.0f) + 1.f) / 2.f;
            sf::Uint8 green = static_cast<sf::Uint8>(180 + 75 * pulse);
            text.setFillColor(sf::Color(0, green, 0));

            //obvodka toje pulse
            sf::Uint8 glow = static_cast<sf::Uint8>(80 + 100 * pulse);
            text.setOutlineColor(sf::Color(0, glow, 0));
        }
        else {
            //color if no click
            text.setFillColor(sf::Color::Yellow);
            text.setOutlineColor(sf::Color(0, 60, 0));
        }
    }

	bool MenuItem::gettitle() const {
		return this->title;
	}

	AdvancedMatrixBackground::AdvancedMatrixBackground() : spawnTimer(0.f), spawnInterval(0.1f), columns(30) {
			if (!font.loadFromFile("assets/fonts/PressStart2P-Regular.ttf")) {
				std::cerr << "Не удалось загрузить шрифт для матрицы!\n";
			}
			float columnWidth = 1020.f / columns;
			for (int i = 0; i < columns; ++i) {
				columnPositions.push_back(i * columnWidth);
			}
			//random generator
			srand(static_cast<unsigned int>(time(nullptr)));
		}

	void AdvancedMatrixBackground::createNewChain(int column) {
			SymbolChain chain;
			chain.positionY = -100;
			chain.speed = 100.f;
			chain.active = true;
			int chainLength = rand() % 15 + 8;
			for (int i = 0; i < chainLength; ++i) {
				sf::Text symbol;
				symbol.setFont(font);

				wchar_t symbols[] = { L'A', L'B', L'C', L'D', L'E', L'F', L'G', L'Y', L'X', L'Z',
									L'0', L'1', L'2', L'3', L'4', L'5', L'6', L'7', L'8', L'9',
									L'+', L'-', L'~', L')', L'(', L'^', L'☆', L'★', L'◆', L'■',
									L'|', L'/', L'?', L'!', L'@', L'#', L'*', L'&', L'$', L'%' };

				symbol.setString(std::wstring(1, symbols[rand() % 40]));
				symbol.setCharacterSize(rand() % 6 + 14);
				symbol.setPosition(columnPositions[column] + rand() % 10 - 5, chain.positionY - i * 20);

				//Gradient, main symbol = lighter
				int greenValue = 255 - (i * 15);
				int alphaValue = 255 - (i * 20);
				if (greenValue < 100) greenValue = 100;
				if (alphaValue < 50) alphaValue = 50;

				symbol.setFillColor(sf::Color(0, greenValue, 0, alphaValue));
				chain.symbols.push_back(symbol);

				//any timer for every symbol
				chain.switchCounters.push_back(0);
				chain.switchIntervals.push_back(rand() % 20 + 5);
			}
			chains.push_back(chain);
			}

	void AdvancedMatrixBackground::updating(float deltaTime) {
				spawnTimer += deltaTime;
				if (spawnTimer >= spawnInterval) {
					spawnTimer = 0.f;
					createNewChain(rand() % columns);
				}
				for (auto& chain : chains) {
					if (!chain.active) continue;

					chain.positionY += chain.speed * deltaTime;
					//update positionn
					for (size_t i = 0; i < chain.symbols.size(); ++i) {
						chain.symbols[i].setPosition(
							chain.symbols[i].getPosition().x,
							chain.positionY - i * 20
						);
						//anime every symbol
						chain.switchCounters[i]++;
						if (chain.switchCounters[i] >= chain.switchIntervals[i]) {
							chain.switchCounters[i] = 0;
							chain.switchIntervals[i] = rand() % 20 + 5; //new radom interval

							wchar_t symbols[] = { L'A', L'B', L'C', L'D', L'E', L'F', L'G', L'Y', L'X', L'Z',
										L'0', L'1', L'2', L'3', L'4', L'5', L'6', L'7', L'8', L'9',
										L'+', L'-', L'~', L')', L'(', L'^', L'☆', L'O', L'◆', L'■',
										L'|', L'/', L'?', L'!', L'@', L'#', L'*', L'&', L'$', L'%' };

							chain.symbols[i].setString(std::wstring(1, symbols[rand() % 40]));
							//size mb
							if (rand() % 10 == 0) {
								chain.symbols[i].setCharacterSize(rand() % 6 + 14);
							}
						}
					}
					//deactivate where leave from screen
					if (chain.positionY - chain.symbols.size() * 20 > 700) {
						chain.active = false;
					}
				}
				//clearr
				chains.erase(
					std::remove_if(chains.begin(), chains.end(),
						[](const SymbolChain& c) { return !c.active; }),
					chains.end()
				);
			};

		void AdvancedMatrixBackground::draw(sf::RenderWindow& window) {
			for (auto& chain : chains) {
				for (auto& symbol : chain.symbols) {
					window.draw(symbol);
				}
			}
		}

		std::vector<Record> loadRecords() {
			std::vector<Record> records;
			try {
				//read json)
				std::ifstream file("config/score.json");
				if (!file.is_open()) {
					std::cerr << "Не удалось открыть файл со статистикой" << std::endl;
					return records;
				}
				nlohmann::json jsonData;
				file >> jsonData;
				file.close();

				//check data
				if (!jsonData.contains("players") || jsonData["players"].is_null()) {
					std::cerr << "Нет данных о игроках" << std::endl;
					return records;
				}
				//all players
				for (const auto& playerEntry : jsonData["players"].items()) {
					std::string playerId = playerEntry.key();
					const auto& playerData = playerEntry.value();
					int maxKills = 0;
					//check all games
					for (const auto& gameEntry : playerData.items()) {
						const auto& gameData = gameEntry.value();
						//check killed
						if (gameData.contains("enemies") &&
							gameData["enemies"].is_object() &&
							gameData["enemies"].contains("total_killed")) {

							int kills = gameData["enemies"]["total_killed"];
							if (kills > maxKills) {
								maxKills = kills;
							}
						}
					}
					//add kills
					if (maxKills > 0) {
						records.push_back({ playerId, maxKills });
					}
				}
				//sort
				std::sort(records.begin(), records.end(),
					[](const Record& a, const Record& b) {
						return a.total_kills > b.total_kills;
					});
				//top - 10
				if (records.size() > 10) {
					records.resize(10);
				}
			}
			catch (const std::exception& e) {
				std::cerr << "Ошибка при загрузке рекордов: " << e.what() << std::endl;
			}
			return records;
		}

		void updateRecords(std::vector<MenuItem>& recordsmenu, sf::Font& font) {
			auto topRecords = loadRecords();
			//remove stariy
			if (recordsmenu.size() > 2) {
				recordsmenu.erase(recordsmenu.begin() + 1, recordsmenu.end() - 1);
			}
			//add new
			float y = 150.f;
			for (int i = 0; i < topRecords.size(); i++) {
				std::string text = std::to_string(i + 1) + ". ID: " + topRecords[i].playerId +
					" - " + std::to_string(topRecords[i].total_kills) + " kills";
				std::wstring wtext(text.begin(), text.end());

				recordsmenu.insert(recordsmenu.end() - 1,
					MenuItem(wtext, font, 20, { 100.f, y }, []() {}, true));
				y += 40.f;
			}
			//if records empty
			if (topRecords.empty()) {
				recordsmenu.insert(recordsmenu.end() - 1,
					MenuItem(L"Пока нет рекордов", font, 24, { 150.f, 150.f }, []() {}, true));
			}
		}

		bool loadAssets(sf::RenderWindow& window, GameRes& assets) {
			//image icon
			sf::Image icon;
			if (icon.loadFromFile("assets/img/icon.png")) {
				window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
			}

			//font
			if (!assets.font.loadFromFile("assets/fonts/PressStart2P-Regular.ttf")) {
				std::cerr << "Шрифт не загружен\n";
				return false;
			}

			//click
			if (!assets.bufferclick.loadFromFile("assets/sound/click.wav")) {
				std::cerr << "Звук клика не загружен\n";
				return false;
			}

			//hello sound
			if (!assets.bufferhello.loadFromFile("assets/sound/hello.wav")) {
				std::cerr << "Приветственный звук не загружен\n";
				return false;
			}

			//start sound
			if (!assets.bufferstart.loadFromFile("assets/sound/startgame.wav")) {
				std::cerr << "Звук запуска не загружен\n";
				return false;
			}

			assets.soundclick.setBuffer(assets.bufferclick);
			assets.soundhello.setBuffer(assets.bufferhello);
			assets.soundstart.setBuffer(assets.bufferstart);

			//for zagryzka
			assets.soundhello.setVolume(25);
			Sleep(100);
			assets.soundhello.play();
			Sleep(100);

			return true;
		}

		void handleMenu(
			sf::Event& event, sf::RenderWindow& window, std::string& screen, bool& needsRedraw, GameRes& res,
			std::vector<MenuItem>& mainmenu,
			std::vector<MenuItem>& settingsmenu,
			std::vector<MenuItem>& recordsmenu,
			std::vector<MenuItem>& ownersmenu,
			std::vector<MenuItem>& difficultyMenu,
			std::string& playerId, bool& editingId, sf::Text& idText,
			sf::Clock& cursorClock, bool& showCursor) {

			if (event.type == sf::Event::MouseButtonPressed &&
				event.mouseButton.button == sf::Mouse::Left) {

				sf::Vector2f mousePos = window.mapPixelToCoords(
					sf::Vector2i(event.mouseButton.x, event.mouseButton.y));

				if (idText.getGlobalBounds().contains(mousePos)) {
					editingId = !editingId;
					cursorClock.restart();
					showCursor = true;
					res.soundclick.play();

					if (editingId) {
						idText.setFillColor(sf::Color::Yellow);
					}
					else {
						std::string text = idText.getString();
						size_t cursorPos = text.find('|');
						if (cursorPos != std::string::npos) {
							text.erase(cursorPos, 1);
						}
						if (text.length() > 4) {
							std::string newId = text.substr(4);

							bool allDigits = !newId.empty();
							for (char c : newId) {
								if (!std::isdigit(c)) {
									allDigits = false;
									break;
								}
							}

							if (allDigits) {
								// max (4294967295)
								try {
									unsigned long long idNum = std::stoull(newId);
									if (idNum > 4294967295ULL) {
										idNum = 4294967295ULL;
										newId = "4294967295";
									}
								}
								catch (...) {
									newId = "1";
								}

								playerId = newId;
							}
							else {
								playerId = "1"; //1111
							}
						}
						if (playerId.empty()) playerId = "1";

						idText.setString("ID: " + playerId);
						idText.setFillColor(sf::Color::White);
					}
				}
				else if (editingId) {
					editingId = false;
					std::string text = idText.getString();
					size_t cursorPos = text.find('|');
					if (cursorPos != std::string::npos) {
						text.erase(cursorPos, 1);
					}
					if (text.length() > 4) {
						std::string newId = text.substr(4);

						//only numbers
						bool allDigits = !newId.empty();
						for (char c : newId) {
							if (!std::isdigit(c)) {
								allDigits = false;
								break;
							}
						}

						if (allDigits) {
							//check max
							try {
								unsigned long long idNum = std::stoull(newId);
								if (idNum > 4294967295ULL) {
									idNum = 4294967295ULL;
									newId = "4294967295";
								}
							}
							catch (...) {
								newId = "1";
							}

							playerId = newId;
						}
						else {
							playerId = "1";
						}
					}
					if (playerId.empty()) playerId = "1";

					idText.setString("ID: " + playerId);
					idText.setFillColor(sf::Color::White);
				}

				//menu without id
				if (!editingId || !idText.getGlobalBounds().contains(mousePos)) {
					clickMenu(window, screen, needsRedraw, res,
						mainmenu, settingsmenu, recordsmenu,
						ownersmenu, difficultyMenu);
				}
			}

			// text with editing
			if (editingId) {
				if (event.type == sf::Event::KeyPressed) {
					if (event.key.code == sf::Keyboard::Enter) {
						editingId = false;
						std::string text = idText.getString();
						size_t cursorPos = text.find('|');
						if (cursorPos != std::string::npos) {
							text.erase(cursorPos, 1);
						}
						if (text.length() > 4) {
							std::string newId = text.substr(4);

							//check numbers
							bool allDigits = !newId.empty();
							for (char c : newId) {
								if (!std::isdigit(c)) {
									allDigits = false;
									break;
								}
							}

							if (allDigits) {
								//check max
								try {
									unsigned long long idNum = std::stoull(newId);
									if (idNum > 4294967295ULL) {
										idNum = 4294967295ULL;
										newId = "4294967295";
									}
								}
								catch (...) {
									newId = "1";
								}

								playerId = newId;
							}
							else {
								playerId = "1";
							}
						}
						if (playerId.empty()) playerId = "1";

						idText.setString("ID: " + playerId);
						idText.setFillColor(sf::Color::White);
					}
					else if (event.key.code == sf::Keyboard::Escape) {
						editingId = false;
						idText.setString("ID: " + playerId);
						idText.setFillColor(sf::Color::White);
					}
				}
				else if (event.type == sf::Event::TextEntered) {
					if (event.text.unicode < 128 && event.text.unicode != 13 && event.text.unicode != 27) {
						char c = static_cast<char>(event.text.unicode);

						std::string currentText = idText.getString();
						size_t cursorPos = currentText.find('|');

						if (c == '\b') { //Backspace - delete
							if (cursorPos != std::string::npos && cursorPos > 4) {
								currentText.erase(cursorPos - 1, 1);
								idText.setString(currentText);
							}
						}
						else if (std::isdigit(c)) { //Only NUMBERS
							if (cursorPos != std::string::npos) {
								currentText.erase(cursorPos, 1);
							}

							//lengh = 9 max
							std::string idOnly = currentText.substr(4);
							if (idOnly.length() < 9) {
								if (cursorPos != std::string::npos) {
									currentText.insert(cursorPos, 1, c);
									currentText.insert(cursorPos + 1, "|");
								}
								else {
									currentText += c;
									currentText += "|";
								}
								idText.setString(currentText);
							}
						}
					}
				}
			}
		}


		void clickMenu(sf::RenderWindow& window,
			std::string& screen,
			bool& needsRedraw,
			GameRes& res,
			std::vector<MenuItem>& mainmenu,
			std::vector<MenuItem>& settingsmenu,
			std::vector<MenuItem>& recordsmenu,
			std::vector<MenuItem>& ownersmenu,
			std::vector<MenuItem>& difficultyMenu) {

			//risovka
			if (screen == "main") {
				for (auto& item : mainmenu) {
					if (item.isMouseOver(window)) {
						item.onClick();
						needsRedraw = true;
						if (!item.gettitle()) {
							res.soundclick.play();
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
							res.soundclick.play();
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
							res.soundclick.play();
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
							res.soundclick.play();
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
							res.soundclick.play();
						}
					}
				}
			}
		}

		void updateMenu(sf::RenderWindow& window,
			std::string& screen,
			float time,
			std::vector<MenuItem>& mainmenu,
			std::vector<MenuItem>& settingsmenu,
			std::vector<MenuItem>& recordsmenu,
			std::vector<MenuItem>& ownersmenu,
			std::vector<MenuItem>& difficultyMenu,
			std::string& playerId, bool& editingId, sf::Text& idText,
			sf::Clock& cursorClock, bool& showCursor) {

			//when hovered
			if (screen == "main") {
				for (auto& item : mainmenu) {
					item.hovered = item.isMouseOver(window);
					if (item.isMouseOver(window) != item.hovered);
					item.update(time);
				}
				//miganie
				if (editingId && screen == "main") {
					if (cursorClock.getElapsedTime().asSeconds() > 0.5f) {
						showCursor = !showCursor;
						cursorClock.restart();

						std::string currentText = idText.getString();
						size_t cursorPos = currentText.find('|');

						if (showCursor) {
							if (cursorPos == std::string::npos) {
								currentText += "|";
								idText.setString(currentText);
							}
						}
						else {
							if (cursorPos != std::string::npos) {
								currentText.erase(cursorPos, 1);
								idText.setString(currentText);
							}
						}
					}
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
		}

		void drawMenu(sf::RenderWindow& window,
			std::string& screen,
			std::vector<MenuItem>& mainmenu,
			std::vector<MenuItem>& settingsmenu,
			std::vector<MenuItem>& recordsmenu,
			std::vector<MenuItem>& ownersmenu,
			std::vector<MenuItem>& difficultyMenu,
			sf::Text idText, bool editingId, GameRes& res) {
			//otobrajenie
			if (screen == "main") {
				for (auto& item : mainmenu)
					window.draw(item.text);
				//id in right ygol
				window.draw(idText);
				//if edit = hint!
				if (editingId) {
					sf::Text hintText;
					hintText.setFont(res.font);
					hintText.setCharacterSize(14);
					hintText.setFillColor(sf::Color(150, 200, 100));
					hintText.setPosition(700.f, 540.f);
					hintText.setString(L"Введите ID (до 9 цифр)");
					window.draw(hintText);
				}
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
		}

		std::vector<MenuItem> createMain(GameRes& res,
			std::function<void()> difficultiesFunc,
			std::function<void()> settingsFunc,
			std::function<void()> recordsFunc,
			std::function<void()> ownersFunc,
			std::function<void()> exitFunc) {
			return {
				MenuItem(L"Guards of ELBRUS", res.font, 50, {100.f, 50.f}, []() {}, true),
				MenuItem(L"Старт", res.font, 36, {100.f, 200.f}, difficultiesFunc, false),
				MenuItem(L"Настройки", res.font, 36, {100.f, 270.f}, settingsFunc, false),
				MenuItem(L"Таблица лидеров", res.font, 36, {100.f, 340.f}, recordsFunc, false),
				MenuItem(L"О создателях", res.font, 36, {100.f, 410.f}, ownersFunc, false),
				MenuItem(L"Выход", res.font, 36, {100.f, 480.f}, exitFunc, false)
			};
		}


		std::vector<MenuItem> createDiff(GameRes& res,
			std::function<void()> easy,
			std::function<void()> medium,
			std::function<void()> hard,
			std::function<void()> back) {
			return {
				MenuItem(L"Выберите сложность", res.font, 36, {200.f, 70.f}, []() {}, true),
			MenuItem(L"Легко", res.font, 30, {150.f, 180.f}, easy, false),
			MenuItem(L"Больше денег, меньше врагов", res.font, 18, {150.f, 220.f}, []() {}, true),

			MenuItem(L"Средне", res.font, 30, {150.f, 280.f}, medium, false),
			MenuItem(L"Стандартная сложность", res.font, 18, {150.f, 320.f}, []() {}, true),

			MenuItem(L"Сложно", res.font, 30, {150.f, 380.f}, hard, false),
			MenuItem(L"Мало денег, много сильных врагов", res.font, 18, {150.f, 420.f}, []() {}, true),

			MenuItem(L"Назад", res.font, 36, {100.f, 550.f}, back, false)
			};
		}

		std::vector<MenuItem> createSettings(GameRes& res,
			MenuItem*& soundToggle,
			std::function<void()> audio,
			std::function<void()> back) {

			std::vector<MenuItem> menu = {
			MenuItem(L"Настройки", res.font, 50, {300.f, 50.f}, []() {}, true),
			MenuItem(L"Громкость", res.font, 30, {100.f, 200.f}, []() {}, true),
			MenuItem(L"Вкл", res.font, 24, {800.f, 200.f}, audio, false),
			MenuItem(L"Сохранить", res.font, 36, {350.f, 500.f}, back, false)
			};
			soundToggle = &menu[2];
			return menu;
		}
		std::vector<MenuItem> createRecords(GameRes& res,
			std::function<void()> back) {

			return {
			MenuItem(L"Таблица рекордов", res.font, 50, { 140.f, 50.f }, []() {}, true),
			MenuItem(L"Назад", res.font, 36, { 100.f, 550.f }, back, false)
			};
		}

		std::vector<MenuItem> createOwners(GameRes& res,
			std::function<void()> back) {
			return {
				MenuItem(L"О создателях", res.font, 50, { 200.f, 50.f }, []() {}, true),
			MenuItem(L"Ducktor74", res.font, 28, { 100.f, 150.f }, []() {}, true),
			MenuItem(L"Owner/Dev", res.font, 22, { 650.f, 150.f }, []() {}, true),
			MenuItem(L"Bucktor74", res.font, 28, { 100.f, 200.f }, []() {}, true),
			MenuItem(L"Designer/Dev", res.font, 22, { 650.f, 200.f }, []() {}, true),
			MenuItem(L"Broguss", res.font, 28, { 100.f, 250.f }, []() {}, true),
			MenuItem(L"Developer", res.font, 22, { 650.f, 250.f }, []() {}, true),
			MenuItem(L"Назад", res.font, 36, { 100.f, 550.f }, back, false)
			};
		}
