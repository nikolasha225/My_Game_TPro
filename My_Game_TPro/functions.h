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
	virtual void setScale(sf::Vector2f vector) = 0;//вектор масштабирования
	virtual void setDrowStatus(bool status) = 0;//задаёт статус отрисовки (если надо сделать невидимым)
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


class DrowStack
{
public:
	DrowStack() {};
	~DrowStack() {};
	void add(IGameObject*); //добавить объект и сразу его сортируем
	void remove(IGameObject*); //удалить
	void draw(sf::RenderWindow*);
private:
	std::vector<IGameObject*> stack;
};

bool game(sf::RenderWindow*);
bool pause(sf::RenderWindow*);