#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <functional>
#include <cmath> // для sin

class MenuItem;
class AdvancedMatrixBackground;

class MenuItem {
public:
    sf::Text text;
    std::function<void()> onClick;
    bool title;
    bool hovered = false;
    MenuItem(const sf::String& label, sf::Font& font, unsigned int size, const sf::Vector2f& pos, std::function<void()> callback, bool title);
    bool isMouseOver(const sf::RenderWindow& window) const;
    void update(float time);
};

struct SymbolChain {
	std::vector<sf::Text> symbols;
	std::vector<int> switchCounters;
	std::vector<int> switchIntervals;
	float positionY;
	float speed;
	bool active;
};

class AdvancedMatrixBackground {
private:
	struct SymbolChain {
		std::vector<sf::Text> symbols;
		std::vector<int> switchCounters;
		std::vector<int> switchIntervals;
		float positionY;
		float speed;
		bool active;
	};

	std::vector<SymbolChain> chains;
	sf::Font font;
	float spawnTimer;
	float spawnInterval;
	int columns;
	std::vector<float> columnPositions;

public:
	AdvancedMatrixBackground();
	void createNewChain(int column);
	void updating(float deltaTime);
	void draw(sf::RenderWindow& window);
};