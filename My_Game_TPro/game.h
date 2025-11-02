#pragma once

//дл€ коли и кирюхи
#include "functions.h"

class Spawner;
class TowerManager;
class GameLogic;

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
private:
	struct DownCell;
	struct Place;
public:
	~TowerManager() = default;
	TowerManager(OBJStack* stack);

	enum EnumSelectType
	{
		other,
		towerSpace,
		buyMeny,

	};

	void checkEvents(sf::RenderWindow* window);//когда Ћ ћќ“∆»ћј≈“—я

private:

	struct Place : public IGameObject
	{
		enum placeState
		{
			empty,
			select,
			tower,
			selectTower
		};
		sf::RectangleShape OBJ;
		sf::Texture TEXTURE;
		Tower* TOWER;
		OBJStack* STACK;
		placeState STATE;
		std::vector<DownCell*> BUY_MENU;

		sf::SoundBuffer SOUND_BUFF_SPAWN;
		sf::SoundBuffer SOUND_BUFF_ERROR;
		sf::SoundBuffer SOUND_BUFF_UPGRADE;
		sf::SoundBuffer SOUND_BUFF_DELETE;
		sf::Sound SOUND;

		~Place() = default;
		Place(sf::Vector2f pos, OBJStack* stack);
		void setState(placeState state);
		bool isEmpty();
		bool isSelect();
		void addTower(Tower* tower);
		Tower* getTower();
		sf::RectangleShape* getShapePtr();
		void selectPlace();
		void unselectPlace();
		void draw(sf::RenderWindow* window);
		uint8_t getLayer();
		void setLayer(uint8_t layer) {}
		sf::Vector2f getSize();
		void setSize(sf::Vector2f size);
		sf::Vector2f getPos(bool isMiddle = 1);
		void setPos(sf::Vector2f vector, bool toMiddle = 1);
		EnumGameObjects getTypeObjet();
		IGameObject* getPtr();
		void tick() {};
	};
	struct DownCell//++
	{
		enum EnumCellState
		{
			unselectFather,
			selectFather,
			selectBuy,
			existTower,
			selectFatherTower
		};
		Place* FATHER;
		sf::RectangleShape MANAGER;
		sf::Texture TEXTURE_MANAGER;
		sf::Texture TEXTURE_TOWER;
		EnumCellState STATE;
		Tower::EnumTowerType NUMBER;

		~DownCell() = default;
		DownCell(Place* father, Tower::EnumTowerType number);//++
		void draw(sf::RenderWindow* window);//++
		void select(bool isFather = 0);//++
		void unselect();//++
		bool tryBuy();//++
		sf::RectangleShape* getCellShapePtr();
		sf::RectangleShape* getDescShapePtr();

		sf::Text TEXT_DESC;
		sf::Text TEXT_PRICE;
		sf::RectangleShape DESC;
		sf::RectangleShape OBJ_PRICE;


	};

	//переменные
	std::vector<Place*> TOWERS;
	OBJStack* STACK;

};

//==============================ќ“ƒ≈Ћ№Ќџ≈ ‘”Ќ ÷»»=========================
unsigned getSummArray(unsigned* array, uint8_t length = 5);

bool mouseNearPoint(sf::Vector2f point, float distance, sf::RenderWindow* window);

bool mouseInButton(sf::RectangleShape* button, sf::RenderWindow* window);

