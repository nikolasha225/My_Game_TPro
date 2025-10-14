#include "mainMenu.h"


    MenuItem::MenuItem(const sf::String& label, sf::Font& font, unsigned int size, const sf::Vector2f& pos, std::function<void()> callback, bool title)
        : onClick(callback), title(title)
    {
        text.setFont(font);
        text.setString(label);
        text.setCharacterSize(size);
        text.setFillColor(sf::Color(180, 255, 180)); // мягкий зелёный
        text.setPosition(pos);
    }

    bool MenuItem::isMouseOver(const sf::RenderWindow& window) const {
        auto mouse = sf::Mouse::getPosition(window);
        auto bounds = text.getGlobalBounds();
        return bounds.contains(static_cast<float>(mouse.x), static_cast<float>(mouse.y));
    }

    void MenuItem::update(float time) {
        if (hovered && !title) {
            // Переливающийся зелёный (плавно пульсирует)
            float pulse = (std::sin(time * 3.0f) + 1.f) / 2.f; // 0..1
            sf::Uint8 green = static_cast<sf::Uint8>(180 + 75 * pulse); // 180..255
            text.setFillColor(sf::Color(0, green, 0));

            // Обводка тоже переливается, но слабее
            sf::Uint8 glow = static_cast<sf::Uint8>(80 + 100 * pulse);
            text.setOutlineColor(sf::Color(0, glow, 0));
        }
        else {
            // Цвет, когда не наведено
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

			// Инициализация случайного генератора
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

				//Градиент цвета - головной символ ярче
				int greenValue = 255 - (i * 15);
				int alphaValue = 255 - (i * 20);
				if (greenValue < 100) greenValue = 100;
				if (alphaValue < 50) alphaValue = 50;

				symbol.setFillColor(sf::Color(0, greenValue, 0, alphaValue));
				chain.symbols.push_back(symbol);

				//Уникальные таймеры для каждого символа
				chain.switchCounters.push_back(0);
				chain.switchIntervals.push_back(rand() % 20 + 5); //Разный интервал для каждого символа
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

					//Обновляем позиции всех символов в цепочке
					for (size_t i = 0; i < chain.symbols.size(); ++i) {
						chain.symbols[i].setPosition(
							chain.symbols[i].getPosition().x,
							chain.positionY - i * 20
						);

						//Анимация КАЖДОГО символа независимо
						chain.switchCounters[i]++;
						if (chain.switchCounters[i] >= chain.switchIntervals[i]) {
							chain.switchCounters[i] = 0;
							chain.switchIntervals[i] = rand() % 20 + 5; //Новый случайный интервал

							wchar_t symbols[] = { L'A', L'B', L'C', L'D', L'E', L'F', L'G', L'Y', L'X', L'Z',
										L'0', L'1', L'2', L'3', L'4', L'5', L'6', L'7', L'8', L'9',
										L'+', L'-', L'~', L')', L'(', L'^', L'☆', L'★', L'◆', L'■',
										L'|', L'/', L'?', L'!', L'@', L'#', L'*', L'&', L'$', L'%' };

							chain.symbols[i].setString(std::wstring(1, symbols[rand() % 40]));

							//Иногда меняем размер символа для большего разнообразия
							if (rand() % 10 == 0) {
								chain.symbols[i].setCharacterSize(rand() % 6 + 14);
							}
						}
					}

					//Деактивация цепочки, когда ушла за экран
					if (chain.positionY - chain.symbols.size() * 20 > 700) {
						chain.active = false;
					}
				}

				//Очистка неактивных цепочек
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