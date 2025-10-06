#include "functions.h"


//=================================JSON=============================================
json JSONSettings = json::parse(std::ifstream("./config/settings.json"));
//json JSONScore = json::parse(std::ifstream("./config/score.json"));
uint8_t LEVEL = 2;

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

uint8_t Enemy::getLayer()
{
	return LAYER;
}

sf::Vector2f Enemy::getSize()
{
	return SIZE;
}

//-----------------------------------------------------
sf::Vector2f Enemy::getPos()
{
	uint8_t LEVEL;
	return sf::Vector2f();
}

//-----------------------------------------------------
void Enemy::setMove(sf::Vector2f vector)
{
}

//-----------------------------------------------------
void Enemy::setScale(sf::Vector2f vector)
{
}

//-----------------------------------------------------
void Enemy::setDrowStatus(bool status)
{
}

void Enemy::draw(sf::RenderWindow* window)
{
	window->draw(OBJ);

}

//-----------------------------------------------------
EnumGameObjects Enemy::getTypeObjet()
{
	return EnumGameObjects();
}

//-----------------------------------------------------
Enemy* Enemy::getPtr()
{
	return nullptr;
}

//-----------------------------------------------------
void Enemy::getDrowStatus()
{
}
