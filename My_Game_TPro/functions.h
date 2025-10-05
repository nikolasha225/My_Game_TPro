#pragma once

//общие функции для игры и главного меню
#include <SFML/Graphics.hpp>
#include <utility>//на всякий случай если пригодится std::pair<*,*>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include <fstream>
#include <algorithm>

using json = nlohmann::json;
extern json JSONSettings;
extern json JSONScore;

enum EnumGameObjects
{
	enemy,
	tower,
	backgroundStatic,
	backgroundDynamic,
	menuWindowObject,
	effect,
	bullet
};

class IGameObject
{
public:
	virtual ~IGameObject() {};
	virtual uint8_t getLayer() = 0;//слой отрисовки
	virtual sf::Vector2f getSize() = 0;//размеры x y
	virtual sf::Vector2f getPos() = 0;//положение на экране
	virtual void setMove(sf::Vector2f vector) = 0;//вектор перемещения
	virtual void setDrowStatus(bool status) = 0;//задаёт статус отрисовки (если надо сделать невидимым)
	virtual void getDrowStatus() = 0;//задаёт статус отрисовки (если надо сделать невидимым)
	virtual void draw(sf::RenderWindow*) = 0;//функция отрисовки
	virtual EnumGameObjects getTypeObjet() = 0;//возвращает тип объекта;
	virtual IGameObject* getPtr() = 0;//ссылка на сам объект
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
			положение sf::vector2F
			текущий размер -||-
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
	void tick();//тик игрового времени

	//общие
	uint8_t getLayer();//слой отрисовки
	sf::Vector2f getSize();//размеры x y
	sf::Vector2f getPos();//положение на экране
	void setMove(sf::Vector2f vector);//вектор перемещения
	void setScale(sf::Vector2f vector);//вектор масштабирования
	void setDrowStatus(bool status);//задаёт статус отрисовки (если надо сделать невидимым)
	void draw(sf::RenderWindow* window);//функция отрисовки
	EnumGameObjects getTypeObjet();//возвращает тип объекта;
	Enemy* getPtr();//ссылка на сам объект
	void getDrowStatus();

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

class DrowStack
{
public:
	DrowStack() {};
	~DrowStack() {};
	void add(IGameObject*); //добавить объект и сразу его сортируем
	void add(std::vector<IGameObject*>); //добавить объект и сразу его сортируем
	void remove(IGameObject*); //удалить
	void draw(sf::RenderWindow*);
private:
	std::vector<IGameObject*> stack;
};

sf::Vector2f getCoordinate(sf::Vector2f, sf::RenderWindow);//функция пересчёта координат из 1920x1080 в текущий дисплей(относительно центра экрана)
bool game(sf::RenderWindow*);//основная функция игры
bool pause(sf::RenderWindow*);//функция паузы