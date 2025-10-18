#pragma once

//для коли и кирюхи
#include "functions.h"

class Spawner;
class GameLogic;


class Spawner
{
public:
	~Spawner() = default;
	Spawner(sf::RenderWindow* window, OBJStack* stack, sf::Event* event);

	void checkEvent();
	void draw();
	void tick();
	void game();
private:
	sf::RenderWindow* WINDOW;
	OBJStack* STACK;
	sf::Event* EVENT;
};