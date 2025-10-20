#pragma once

//дл€ коли и кирюхи
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
	void updateProgress();

private:
	sf::RenderWindow* WINDOW;
	OBJStack* STACK;
	sf::Event* EVENT;

	float PROGRESS; //высчитываем через весы каждого моба (1 - количество оставшихс€ на их тик пер спавн делить на оначальное)
	
	//0 - basic 1 - fast 2 - tank 3 - miniBoss 4 - Boss
	int TICK_PER_SPAWN[5]; //высчитываем через врем€ и количество
	int TICK_OF_ENEMY[5]; //текущие тики
	int COUNT_OF_MOBS[5]; //из json + рандомизаци€
};

//==============================ќ“ƒ≈Ћ№Ќџ≈ ‘”Ќ ÷»»=========================

bool mouseNearPoint(sf::Vector2f point, float distance, sf::RenderWindow* window);

bool mouseClickOnButton(sf::RectangleShape button, sf::RenderWindow* window);
