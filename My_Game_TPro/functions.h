#pragma once

//общие функции для игры и главного меню
#include <SFML/Graphics.hpp>
#include <utility>//на всякий случай если пригодится std::pair<*,*>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include <fstream>
#include <algorithm>
#include <map>

using json = nlohmann::json;
extern json JSONSettings;
//extern json JSONScore;

extern uint8_t LEVEL;

//енам игроввых объектов
enum EnumGameObjects
{
	enemy,//15-23
	tower,//23-30
	backgroundStatic,//1-3
	backgroundDynamic,//3-9
	menuWindowObject,//40-49 + уведы 50+
	effect,//10-15
	bullet//30-39
};

class Bullet;
class Enemy;
class Tower;

//очередь отрисовки
constexpr EnumGameObjects renderLine[] = {
	backgroundStatic,
	backgroundDynamic,
	effect,
	enemy,
	tower,
	bullet,
	menuWindowObject
};

constexpr EnumGameObjects renderLineReverce[] = {
	menuWindowObject,
	bullet,
	tower,
	enemy,
	effect,
	backgroundDynamic,
	backgroundStatic
};

class IGameObject
{
public:
	virtual ~IGameObject() {};
	virtual uint8_t getLayer() = 0;//слой отрисовки
	virtual void setLayer(uint8_t layer) = 0;//слой отрисовки
	virtual sf::Vector2f getSize() = 0;//размеры x y
	virtual sf::Vector2f getPos(bool isMiddle = 1) = 0;//положение на экране
	virtual void setMove(sf::Vector2f vector) = 0;//вектор перемещения
	virtual void setPos(sf::Vector2f vector, bool toMiddle = 1) = 0;//вектор перемещения
	virtual void setDrowStatus(bool status) = 0;//задаёт статус отрисовки (если надо сделать невидимым)
	virtual bool getDrowStatus() = 0;//задаёт статус отрисовки (если надо сделать невидимым)
	virtual void draw(sf::RenderWindow*) = 0;//функция отрисовки
	virtual EnumGameObjects getTypeObjet() = 0;//возвращает тип объекта;
	virtual IGameObject* getPtr() = 0;//ссылка на сам объект
	virtual void tick() =0;
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
	~Enemy() {};
	Enemy(
		EnumEnemyType type = basicVirus,
		float startPos = -1.f,
		float hp = 100.f,
		float velocity = 0.05f,
		std::string texture = "./textures/debug/base.png",
		sf::Vector2f textureScale = sf::Vector2f(0, 0)
	);//через switch case из типа врага подсосать всё остальное

	//чисто enemy
	float getHP();
	void subHP(float hp);//нанести урон
	void multVelocity(float coef);
	bool isDie();
	EnumEnemyType getEnemyType();
	float getPosPercent();
	void tick();//игровой тик

	//общие
	void setPos(sf::Vector2f pos, bool toMiddle = 1);
	void setLayer(uint8_t newLayer);
	uint8_t getLayer();//слой отрисовки
	sf::Vector2f getSize();//размеры x y
	sf::Vector2f getPos(bool isMiddle = 1);//положение на экране
	void setMove(sf::Vector2f vector);//вектор перемещения
	void setDrowStatus(bool status);//задаёт статус отрисовки (если надо сделать невидимым)
	void draw(sf::RenderWindow* window);//функция отрисовки
	EnumGameObjects getTypeObjet();//возвращает тип объекта;
	Enemy* getPtr();//ссылка на сам объект
	bool getDrowStatus();

	bool operator<(const Enemy& other) const;
	
	void checkBullet(Bullet& bullet);

private:
	float POS;	//путь каждого врага делится на 100% и двигается враг в %
	sf::RectangleShape OBJ;
	sf::Vector2f SIZE;
	sf::Texture TEXTURE;
	EnumEnemyType TYPE;
	float HP;
	float VELOCITY;

	uint8_t LAYER;
	bool DRAW_STATUS;

};

class Bullet : public IGameObject
{
public:
	Bullet();
	~Bullet() {};

	//baza
	uint8_t getLayer();//слой отрисовки
	void setLayer(uint8_t layer);//слой отрисовки
	sf::Vector2f getSize();//размеры x y
	sf::Vector2f getPos(bool isMiddle = 1);//положение на экране
	void setMove(sf::Vector2f vector);//вектор перемещения
	void setPos(sf::Vector2f vector, bool toMiddle = 1);//вектор перемещения
	void setDrowStatus(bool status);//задаёт статус отрисовки (если надо сделать невидимым)
	bool getDrowStatus();//задаёт статус отрисовки (если надо сделать невидимым)
	void draw(sf::RenderWindow*);//функция отрисовки
	EnumGameObjects getTypeObjet();//возвращает тип объекта;
	IGameObject* getPtr();//ссылка на сам объект
	void tick();

	//свои
	sf::Vector2f getVectorToTarget();
	bool operator<(const Bullet& other) const;
	void move();//через вектор до цели домноженный на скорость двигается сетмувом


private:
	bool FLY;
	sf::Vector2f POS;
	Enemy* TARGET;
	float DAMAGE;
	sf::RectangleShape OBJ;
	sf::Vector2f SIZE;
	sf::Texture TEXTURE;
	EnumGameObjects TYPE;
	float VELOCITY;
	uint8_t LAYER;
	bool STATE;
};
class OBJStack
{
public:
	OBJStack();
	~OBJStack() {};
	void add(IGameObject*); //добавить объект и сразу его сортируем
	void add(std::vector<IGameObject*>); //добавить объект и сразу его сортируем
	void remove(IGameObject*); //удалить
	void draw(sf::RenderWindow*);
	void tick();
	void sortByLayer();

private:
	std::map<EnumGameObjects, std::vector<IGameObject*>> stack;
};

sf::Vector2f getCoordinate(sf::Vector2f, sf::RenderWindow);//функция пересчёта координат из 1920x1080 в текущий дисплей(относительно центра экрана)

sf::Vector2f wayToCoordinate(float pos, uint8_t level = LEVEL);//функция пути

sf::Vector2f normalize(sf::Vector2f vector);//возвращает единичный вектор
