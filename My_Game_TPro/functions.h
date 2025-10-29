#pragma once

//debug
#include <iostream>

//общие функции для игры и главного меню
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

//#include <utility>//на всякий случай если пригодится std::pair<*,*>
#include <fstream>
#include <nlohmann/json.hpp>

#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <random>
#include <chrono>

#define M_PI 3.1416

using json = nlohmann::json;
extern json JSONSettings;
//extern json JSONScore;

extern uint8_t LEVEL;
extern unsigned MONEY;
extern float HEALTH;
extern uint8_t DIFFICULT;
extern sf::Vector2f RESOLUTION;
extern unsigned long TIME;
extern unsigned long START_TIME;

//рандом функции
static std::mt19937 generator([]() {
	return std::mt19937(
		static_cast<unsigned int>(
			std::chrono::high_resolution_clock::now()
			.time_since_epoch()
			.count()
			)
	);
	}());
#define RAND_INT(min, max) std::uniform_int_distribution<int>(min, max)(generator)
#define RAND_FLOAT(min, max) std::uniform_real_distribution<float>(min, max)(generator)

//енам игроввых объектов
enum EnumGameObjects
{
	enemy,//15-23
	tower,//23-29
	core,//30
	backgroundStatic,//1-3
	backgroundDynamic,//3-9
	menuWindowObject,//40-49 + уведы 50+
	effect,//10-15
	bullet//31-39
};

class Bullet;
class Enemy;
class Tower;
class IGameObject;
class OBJStack;
class Core;

//очередь отрисовки
constexpr EnumGameObjects renderLine[] = {
	backgroundStatic,
	backgroundDynamic,
	effect,
	enemy,
	tower,
	core,
	bullet,
	menuWindowObject
};

constexpr EnumGameObjects renderLineReverce[] = {
	menuWindowObject,
	bullet,
	core,
	tower,
	enemy,
	effect,
	backgroundDynamic,
	backgroundStatic
};

const std::vector<sf::Vector2f> wayPoints[3] = //наши пути
{
	{
		sf::Vector2f(200, 980),   // start
		sf::Vector2f(200, 300),   // point1
		sf::Vector2f(1720, 300),  // point2  
		sf::Vector2f(1720, 880),  // point3
		sf::Vector2f(500, 880),   // point4
		sf::Vector2f(500, 520),   // point5
		sf::Vector2f(1520, 520),  // point6
		sf::Vector2f(1520, 600)   // end
	},
	{
		sf::Vector2f(200, 980),   // start
		sf::Vector2f(580, 300),   // point1
		sf::Vector2f(960, 980),   // point2  
		sf::Vector2f(1340, 300),  // point3
		sf::Vector2f(1720, 880)   // end
	},
	{
		sf::Vector2f(200, 980),   // start
		sf::Vector2f(200, 300),   // point1
		sf::Vector2f(1720, 300),  // point2
		sf::Vector2f(1720, 600)   // end
	}
};

const std::vector <sf::Vector2f> towerPoint[3] =
{
	{
		sf::Vector2f(350, 900),
		sf::Vector2f(350, 650),
		sf::Vector2f(350, 400),
		sf::Vector2f(770, 400),
		sf::Vector2f(1200, 400),
		sf::Vector2f(1620, 400),
		sf::Vector2f(770, 690),
		sf::Vector2f(1200, 690),
	},
	{
		sf::Vector2f(320, 480),
		sf::Vector2f(580, 850),
		sf::Vector2f(580, 600),
		sf::Vector2f(960, 600),
		sf::Vector2f(960, 350),
		sf::Vector2f(1340, 600),
		sf::Vector2f(1340, 850),
		sf::Vector2f(1600, 480),
	},
	{
		 sf::Vector2f(400, 520),
		sf::Vector2f(400, 710),
		sf::Vector2f(400, 900),
		sf::Vector2f(720, 520),
		sf::Vector2f(720, 710),
		sf::Vector2f(720, 900),
		sf::Vector2f(1050, 520),
		sf::Vector2f(1050, 710),
		sf::Vector2f(1050, 900),
		sf::Vector2f(1400, 520),
		sf::Vector2f(1400, 710),
		sf::Vector2f(1400, 900),

	}
};

const std::string enemyTypes[5] = {
	"basic",
	"fast",
	"tank",
	"miniBoss",
	"boss"
};

const std::string towerTypes[4] = {
	"defender",
	"avast",
	"drWeb",
	"kaspersky"
};

//=====================================IGameObject============================

class IGameObject
{
public:
	virtual ~IGameObject() = default;
	virtual uint8_t getLayer() = 0;//слой отрисовки
	virtual void setLayer(uint8_t layer) = 0;//слой отрисовки
	virtual sf::Vector2f getSize() = 0;//размеры x y
	virtual void setSize(sf::Vector2f size) = 0;//размеры x y
	virtual sf::Vector2f getPos(bool isMiddle = 1) = 0;//положение на экране
	virtual void setPos(sf::Vector2f vector, bool toMiddle = 1) = 0;//вектор перемещения
	virtual void draw(sf::RenderWindow*) = 0;//функция отрисовки
	virtual EnumGameObjects getTypeObjet() = 0;//возвращает тип объекта;
	virtual IGameObject* getPtr() = 0;//ссылка на сам объект
	virtual void tick() = 0;
private:
	/*
	В каждом дочернем должно быть (для игры, меню сам решай):
		В конструкторе:
			текстура + по умолчанию*
			начальное положение + по умолчанию*
			тип из енама!!
			размеры объекта + по умолчанию = -1 для подсасывания из файла*
			размеры хитбокса + по умолчанию = -1 для подсасывания из файла*
			* - в config.json для каждого типа объекта всё прописано по умолчанию
		В объекте
			слой отображения uint_8t (по умолчанию от енама подсасывается из файла)
			перегруженный оператор bool operator<(const NameClass& other) const {слой} для функции std::sort(vectorObj.begin(), vectorObj.end());
	*/
};

//======================================ENEMY=======================================
class Enemy :public IGameObject
{
public:
	enum EnumEnemyType
	{
		basicVirus,
		fastVirus,
		tankVirus,
		miniBossVirus,
		bossVirus
	};
	~Enemy() = default;
	Enemy(EnumEnemyType type = basicVirus);//через switch case из типа врага подсосать всё остальное

	//чисто enemy
	float getHP(bool startHP = 0);
	void subHP(float hp);//нанести урон
	void multVelocity(float coef);
	bool isDie();
	EnumEnemyType getEnemyType();
	float getPosPercent();
	void tick();//игровой тик
	bool checkBullet(Bullet* bullet);
	unsigned getMoney();
	float getDamage();
	//общие
	void setSize(sf::Vector2f size);
	void setPos(sf::Vector2f pos, bool toMiddle = 1);
	void setLayer(uint8_t newLayer);
	uint8_t getLayer();//слой отрисовки
	sf::Vector2f getSize();//размеры x y
	sf::Vector2f getPos(bool isMiddle = 1);//положение на экране
	void setMove(sf::Vector2f vector);//вектор перемещения
	void setDrawStatus(bool status);//задаёт статус отрисовки (если надо сделать невидимым)
	void draw(sf::RenderWindow* window);//функция отрисовки
	EnumGameObjects getTypeObjet();//возвращает тип объекта;
	Enemy* getPtr();//ссылка на сам объект
	bool getDrawStatus();
	sf::RectangleShape* getShape();

	bool operator<(const Enemy& other) const;

	void sound();

private:
	float POS;	//путь каждого врага делится на 100% и двигается враг в %
	sf::RectangleShape OBJ;
	sf::Texture TEXTURE;
	float HP;
	float START_HP;
	float VELOCITY;
	EnumEnemyType TYPE;
	float DAMAGE;
	unsigned PRICE;
	sf::Vector2f NEED_MOVE;

	uint8_t LAYER;
	bool DRAW_STATUS;

	sf::SoundBuffer SOUND_BUFF;
	sf::Sound SOUND;

};

//======================================TOWER=======================================

class Tower : public IGameObject {
public:
	enum EnumTowerType {
		defender,
		avast,
		drWeb,
		kaspersky
	};

	Tower(
		EnumTowerType type,
		OBJStack* stack,
		sf::Vector2f pos
	);
	~Tower() = default;
	uint8_t getLayer();//слой отрисовки
	void setLayer(uint8_t layer);//слой отрисовки
	sf::Vector2f getSize();//размеры x y
	sf::Vector2f getPos(bool isMiddle = 1);//положение на экране
	void setMove(sf::Vector2f vector);//вектор перемещения
	void setPos(sf::Vector2f vector, bool toMiddle = 1);//вектор перемещения
	void setDrawStatus(bool status);//задаёт статус отрисовки (если надо сделать невидимым)
	bool getDrawStatus();//задаёт статус отрисовки (если надо сделать невидимым)
	void draw(sf::RenderWindow* window);//функция отрисовки
	EnumGameObjects getTypeObjet();//возвращает тип объекта;
	IGameObject* getPtr();//ссылка на сам объект
	void tick();
	sf::RectangleShape* getShape();
	void setSize(sf::Vector2f size);

	//свои
	Bullet* shoot(Enemy* target); //специально оставил, мб там сделаю другие функции выбора целей (не последнюю а ближайшую и тп)
	Enemy* getTarget();
	void upgrade(uint8_t level = 1);
	EnumTowerType getTowerType();
	unsigned getPrice();
	void sound();


private:
	sf::RectangleShape OBJ;
	sf::Texture TEXTURE;
	sf::Vector2f SCALE;
	uint8_t TOWER_LEVEL;
	float BULLET_DAMAGE;
	float BULLET_VELOCITY_COEF;
	unsigned TOWER_VELOCITY;
	EnumTowerType TYPE;
	OBJStack* STACK;
	uint8_t LAYER;
	unsigned PRICE;
	bool DRAW_STATUS;
	float SHOOT_SCALE;
	unsigned STATE_SHOOT;// < TOWER_VELOCITY
	float RANGE;
	//Enemy* LAST_TARGET; // мне слишком в падлу писать оптимизированный код где будет проверяться не умерла ли последняя цель и тп
	sf::SoundBuffer SOUND_BUFF;
	sf::Sound SOUND;
};

//======================================BULLET======================================

class Bullet : public IGameObject
{
public:

	Bullet(
		Tower::EnumTowerType type = Tower::defender,
		Enemy* target = nullptr,
		sf::Vector2f startPos = sf::Vector2f(100, 100));
	~Bullet() = default;

	//baza
	uint8_t getLayer();//слой отрисовки
	void setLayer(uint8_t layer);//слой отрисовки
	sf::Vector2f getSize();//размеры x y
	sf::Vector2f getPos(bool isMiddle = 1);//положение на экране
	void setMove(sf::Vector2f vector);//вектор перемещения
	void setPos(sf::Vector2f vector, bool toMiddle = 1);//вектор перемещения
	void setDrawStatus(bool status);//задаёт статус отрисовки (если надо сделать невидимым)
	bool getDrawStatus();//задаёт статус отрисовки (если надо сделать невидимым)
	void draw(sf::RenderWindow*);//функция отрисовки
	EnumGameObjects getTypeObjet();//возвращает тип объекта;
	IGameObject* getPtr();//ссылка на сам объект
	void tick();
	sf::RectangleShape* getShape();
	void setSize(sf::Vector2f size);

	//свои
	sf::Vector2f getVectorToTarget(bool isNormalise = 1);
	bool operator<(const Bullet& other) const;
	void move();//через вектор до цели домноженный на скорость двигается сетмувом
	void complete();//попадание зафиксировано
	bool isCompleted(); //возвращает статус
	float getDamage();
	bool targetIsDie();
	void multDamage(float coef);
	void multVelocity(float coef);
	void setDamage(float damage);
	bool update(std::vector<IGameObject*> targets);

private:
	Enemy* TARGET;
	float DAMAGE;
	sf::RectangleShape OBJ;
	sf::Texture TEXTURE;
	float VELOCITY;
	uint8_t LAYER;
	bool IS_FLY;
	bool DRAW_STATUS;
};

//====================================OBJ STACK=====================================

class OBJStack
{
public:
	OBJStack();
	~OBJStack() = default;
	void add(IGameObject*); //добавить объект и сразу его сортируем
	void add(std::vector<IGameObject*>); //добавить объект и сразу его сортируем
	void remove(IGameObject*); //удалить
	void draw(sf::RenderWindow*);
	void tick();
	void sortByLayer();
	bool deleteObj(IGameObject* obj);
	OBJStack* getPtr();
	size_t getCountOf(EnumGameObjects type);
	std::map<EnumGameObjects, std::vector<IGameObject*>>* getStack();
private:
	std::map<EnumGameObjects, std::vector<IGameObject*>> stack;
	std::vector<IGameObject*> deleted;
};

//==========================================CORE=================================

class Core :public IGameObject
{
public:
	Core();
	~Core() = default;

	uint8_t getLayer();//слой отрисовки
	void setLayer(uint8_t layer);//слой отрисовки
	sf::Vector2f getSize();//размеры x y
	void setSize(sf::Vector2f size);//размеры x y
	sf::Vector2f getPos(bool isMiddle = 1);//положение на экране
	void setMove(sf::Vector2f vector);//вектор перемещения
	void setPos(sf::Vector2f vector, bool toMiddle = 1);//вектор перемещения
	void setDrawStatus(bool status);//задаёт статус отрисовки (если надо сделать невидимым)
	bool getDrawStatus();//задаёт статус отрисовки (если надо сделать невидимым)
	void draw(sf::RenderWindow*);//функция отрисовки
	EnumGameObjects getTypeObjet();//возвращает тип объекта;
	IGameObject* getPtr();//ссылка на сам объект
	void tick();
	sf::RectangleShape* getShape();

	bool isDamaged();

private:
	sf::RectangleShape CORE;
	sf::Texture TEXTURE;
	float LAST_HEALTH;
	unsigned TICK_COUNTER;
	unsigned TICK_DAMAGE;
	uint8_t LAYER;
	bool DRAW_STATUS;
	float MOVE;
};


//==================================ОТДЕЛЬНЫЕ ФУНКЦИИ===============================

sf::Vector2f getNewCoordinate(sf::Vector2f pos);//функция пересчёта координат из 1920x1080 в текущий дисплей(относительно центра экрана)

sf::Vector2f wayToCoordinate(float pos, uint8_t level = LEVEL);//функция пути

sf::Vector2f normalize(sf::Vector2f vector);//возвращает единичный вектор

sf::Vector2f getPositionOnPathByDistance(float pos, const std::vector<sf::Vector2f> pathPoints);//по значению pos и вектору точек находит точку на многоугольнике

float getDistance(sf::Vector2f vector1, sf::Vector2f vector2);

bool isIntersected(sf::RectangleShape obj1, sf::RectangleShape obj2);

bool isPointIntoShape(sf::Vector2f point, sf::RectangleShape obj);

float getWayCoeficent(uint8_t level = LEVEL);

float getWayLength(std::vector<sf::Vector2f> pathPoints);

//=================================DEBUG ONLY====================================

std::vector<sf::RectangleShape> createSimplePath(const std::vector<sf::Vector2f>& points, float width);