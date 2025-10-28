#pragma once

//дл€ коли и кирюхи
#include "functions.h"

class Spawner;
class TowerManager;
class GameLogic;

enum EnumGameEvent {//игровые событи€ 
	bossExist
};

//==============================SPAWNER===================================
class Spawner
{
public:
	~Spawner() = default;
	Spawner(sf::RenderWindow* window, OBJStack* stack);

	void tick();
	//вовращает массив из 5 булов (кого можно спавнить) (должна сама обнулить заспавненые)
	bool* allowSpawnEnemy();

private:
	sf::RenderWindow* WINDOW;
	OBJStack* STACK;

	float PROGRESS; //высчитываем через весы каждого моба (1 - количество оставшихс€ на их тик пер спавн делить на оначальное)

	unsigned START_COUNT_ALL;

	//0 - basic 1 - fast 2 - tank 3 - miniBoss 4 - Boss
	unsigned TICK_OF_ENEMY[5]; //текущие тики
	unsigned COUNT_MOBS[5]; //из json + рандомизаци€
	unsigned START_COUNT_MOBS[5]; //из json + рандомизаци€

	sf::SoundBuffer SOUND_BUFF;
	sf::Sound SOUND;
};

//==============================TOWER_MANAGER=============================

class TowerManager
{
public:
	~TowerManager() = default;
	TowerManager();

	enum EnumSelectType
	{
		other,
		towerSpace,
		buyMeny,

	};

	void checkEvents();
	bool eventSelectTower();
	bool eventUnselectLast();
	bool eventBuyTower();
private:
	struct DownCell;
	struct Place;

	enum placeState
	{
		empty,
		select,
		bye,
		tower,
		manage
	};
	struct Place
	{
		sf::RectangleShape OBJ;
		sf::Texture TEXTURE;
		Tower* TOWER;
		placeState STATE;
		std::vector<DownCell*> BUY_MENU;

		~Place() = default;
		Place(sf::Vector2f pos);
		void setStage(placeState state);
		void draw(sf::RenderWindow* window);
		bool isEmpty();
		void addTower();
	};
	struct DownCell
	{
		enum EnumCellState
		{
			unselectFather,
			selectFather,
			selectBuy,
			manageTower
		};
		Place* FATHER;
		sf::RectangleShape OBJ;
		sf::RectangleShape DESC;
		sf::RectangleShape MANAGER;
		sf::Texture TEXTURE_MANAGER;
		sf::Texture TEXTURE_TOWER;
		sf::Texture TEXTURE_DESC;
		EnumCellState STATE;
		Tower::EnumTowerType NUMBER;

		~DownCell() = default;
		DownCell(Place* father, Tower::EnumTowerType numbber);
		void draw(sf::RenderWindow* window);
		void select();
		void unselect();
		bool tryBue();
		void removeTower();
	};

	//переменные
	std::vector<Place*> TOWERS;
};

//==============================ќ“ƒ≈Ћ№Ќџ≈ ‘”Ќ ÷»»=========================
unsigned getSummArray(unsigned* array, uint8_t length = 5);

bool mouseNearPoint(sf::Vector2f point, float distance, sf::RenderWindow* window);

bool mouseInButton(sf::RectangleShape button, sf::RenderWindow* window);

