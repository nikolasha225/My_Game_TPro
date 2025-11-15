#pragma once

//��� ���� � ������
#include "functions.h"

#pragma once
#include <Windows.h>
#include <dshow.h>
#include <string>
#pragma comment(lib, "strmiids.lib")

#include <chrono>

class Spawner;
class TowerManager;
class GameLogic;

class VideoPlayer;

enum EnumGameState
{
	GAME,
	PAUSE,
	WIN,
	LOSE,
	AD,
	END_GAME,
	NEXT_LEVEL,
	OVER
};

extern unsigned long long TIME_STAMP_SCORE;

//==============================SPAWNER===================================
class Spawner
{
public:
	~Spawner() = default;
	Spawner(sf::RenderWindow* window, OBJStack* stack);

	void tick();
	//��������� ������ �� 5 ����� (���� ����� ��������) (������ ���� �������� �����������)
	bool* allowSpawnEnemy();

	bool existEnemy();

private:
	sf::RenderWindow* WINDOW;
	OBJStack* STACK;

	float PROGRESS; //����������� ����� ���� ������� ���� (1 - ���������� ���������� �� �� ��� ��� ����� ������ �� ����������)

	unsigned START_COUNT_ALL;

	//0 - basic 1 - fast 2 - tank 3 - miniBoss 4 - Boss
	unsigned TICK_OF_ENEMY[5]; //������� ����
	unsigned COUNT_MOBS[5]; //�� json + ������������
	unsigned START_COUNT_MOBS[5]; //�� json + ������������

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

	void checkEvents(sf::RenderWindow* window);//����� �������������
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

	//����������
	std::vector<Place*> TOWERS;
	OBJStack* STACK;

};

//==============================VIDEO_PLAYER===============================

class VideoPlayer {
private:
	IGraphBuilder* graph = nullptr;
	IMediaControl* control = nullptr;
	IMediaEvent* events = nullptr;
	bool isPlayingFlag = false;

public:
	enum VideoStatus {
		VIDEO_NOT_STARTED = 0,
		VIDEO_RUNNING = 1,
		VIDEO_FINISHED = 2,
		VIDEO_CLOSED = 3
	};

	bool playVideo(const std::wstring& path);
	VideoStatus checkVideoStatus();
	void stopVideo();
	~VideoPlayer() { stopVideo(); }
};

//==============================��������� �������=========================
unsigned getSummArray(unsigned* array, uint8_t length = 5);

bool mouseNearPoint(sf::Vector2f point, float distance, sf::RenderWindow* window);

bool mouseInButton(sf::RectangleShape* button, sf::RenderWindow* window);

bool vatchAD(VideoPlayer* player);

void sendAltTab();

void writeScore(OBJStack* stack, unsigned id);