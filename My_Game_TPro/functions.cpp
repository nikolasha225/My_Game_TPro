#include "functions.h"


//=================================JSON=============================================
json JSONSettings = json::parse(std::ifstream("./config/settings.json"));
//json JSONScore = json::parse(std::ifstream("./config/score.json"));

//=================================ѕ≈–≈ћ≈ЌЌџ≈=======================================
//определ€ем extern переменную level
uint8_t LEVEL = 1;

//==================================ENEMY==============================================
Enemy::Enemy(EnumEnemyType type, float startPos, float hp, float velocity, std::string texture, sf::Vector2f textureScale)
{
	TYPE = type;
	DRAW_STATUS = 1;
	LAYER = JSONSettings["ENEMY"]["layer"];
	std::string typeOfGet = "basic";
	switch (type)
	{
	case Enemy::basicVirus:
		typeOfGet = "basic";
		break;
	case Enemy::fastVirus:
		typeOfGet = "fast";
		break;
	case Enemy::tankVirus:
		typeOfGet = "tank";
		break;
	case Enemy::miniBossVirus:
		typeOfGet = "miniBoss";
		break;
	case Enemy::bossVirus:
		typeOfGet = "Boss";
		break;
	default:
		typeOfGet = "basic";
		break;
	}
	if (startPos >= 0) {
		POS = startPos;
		HP = hp;
		OBJ = sf::RectangleShape(textureScale);
		TEXTURE.loadFromFile(texture);
		OBJ.setTexture(&TEXTURE);
		OBJ.setPosition(100, 100);
		SIZE = textureScale;
		VELOCITY = velocity;
	}
	else
	{
		POS = 0;
		HP = JSONSettings["ENEMY"][typeOfGet]["hp"];
		SIZE = sf::Vector2f(
			JSONSettings["ENEMY"][typeOfGet]["size"][0],
			JSONSettings["ENEMY"][typeOfGet]["size"][1]
		);
		OBJ = sf::RectangleShape(SIZE);
		TEXTURE.loadFromFile(JSONSettings["ENEMY"][typeOfGet]["texture"]);
		OBJ.setTexture(&TEXTURE);
		VELOCITY = (float)JSONSettings["ENEMY"]["velocityCoeficent"] * (float)JSONSettings["ENEMY"][typeOfGet]["velocity"];
		OBJ.setPosition(100, 100);

	}
}

float Enemy::getHP()
{
	return HP;
}

void Enemy::subHP(float hp)
{
	HP -= hp;
}

void Enemy::multVelocity(float coef)
{
	VELOCITY *= coef;
}

bool Enemy::isDie()
{
	return HP <= 0;
}

Enemy::EnumEnemyType Enemy::getEnemyType()
{
	return TYPE;
}

float Enemy::getPosPercent()
{
	return POS;
}

void Enemy::setPos(sf::Vector2f pos)
{
	OBJ.setPosition(pos);
}

//++++++++++++++++-----------------
void Enemy::tick()
{
	POS += VELOCITY;
}

void Enemy::setLayer(uint8_t newLayer)
{
	LAYER = newLayer;
}

uint8_t Enemy::getLayer()
{
	return LAYER;
}

sf::Vector2f Enemy::getSize()
{
	return SIZE;
}

sf::Vector2f Enemy::getPos()
{
	return wayToCoordinate(POS, LEVEL);
}

void Enemy::setMove(sf::Vector2f vector)
{
	OBJ.move(vector);
}

void Enemy::setDrowStatus(bool status)
{
	DRAW_STATUS = status;
}

void Enemy::draw(sf::RenderWindow* window)
{
	this->setPos(wayToCoordinate(POS));
	window->draw(OBJ);

}

EnumGameObjects Enemy::getTypeObjet()
{
	return enemy;
}

Enemy* Enemy::getPtr()
{
	return this;
}

bool Enemy::getDrowStatus()
{
	return DRAW_STATUS;
}

bool Enemy::operator<(const Enemy& other) const
{
	return this->LAYER<other.LAYER;
}


//============================OBJ STACK=================================

OBJStack::OBJStack()
{
	stack = {
	{enemy, {}},
	{tower, {}},
	{backgroundStatic, {}},
	{backgroundDynamic, {}},
	{menuWindowObject, {}},
	{effect, {}},
	{bullet, {}}
	};
}

void OBJStack::add(IGameObject* obj)
{

	stack[obj->getTypeObjet()].push_back(obj);
}

void OBJStack::add(std::vector<IGameObject*> vector)
{
	for(auto &i : vector)
		stack[i->getTypeObjet()].push_back(i);
}

void OBJStack::remove(IGameObject* obj)
{
	if (obj == nullptr) return;

	auto it = std::find(
		stack[obj->getTypeObjet()].begin(), 
		stack[obj->getTypeObjet()].end(), obj
	);

	if (it != stack[obj->getTypeObjet()].end()) {
		stack[obj->getTypeObjet()].erase(it);
	}
}
void OBJStack::draw(sf::RenderWindow* window)
{
	this->sortByLayer();
	for (auto i : renderLine)
		for (auto& obj : stack[i])
			obj->draw(window);
}

void OBJStack::sortByLayer()
{
	for (auto i : renderLine) 
		std::sort(stack[i].begin(), stack[i].end());
	
}
void OBJStack::tick()
{
	for (auto i : renderLineReverce)
		for (auto& obj : stack[i])
			obj->tick();
}

//============================ќ“ƒ≈Ћ№Ќџ≈ ‘”Ќ ÷»»==========================

//-----------------------------------------------------------------------
sf::Vector2f wayToCoordinate(float pos, uint8_t level)
{
	//в значении по умолчанию extern пременна€ левел, так что передавать функции можно только пос, остальное сама подсосЄт из экстернов

	//тут временна€ заглушка чтобы всЄ компилировалось
	return sf::Vector2f(100 + 10*pos, 500);
}

sf::Vector2f normalize(sf::Vector2f vec) {
	float length = std::sqrt(vec.x * vec.x + vec.y * vec.y);
	return (length > 0) ? vec / length : sf::Vector2f(0, 0);
}
