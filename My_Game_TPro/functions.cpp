#include "functions.h"


//=================================JSON=============================================
json JSONSettings = json::parse(std::ifstream("./config/settings.json"));
//json JSONScore = json::parse(std::ifstream("./config/score.json"));

//=================================ѕ≈–≈ћ≈ЌЌџ≈=======================================
//определ€ем extern переменную level
uint8_t LEVEL = 1;
unsigned MONEY = 0;

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

void Enemy::setPos(sf::Vector2f pos, bool toMiddle)
{
	if (toMiddle)
	{
		pos.x -= SIZE.x/2;
		pos.y -= SIZE.y/2;
	}
	OBJ.setPosition(pos);
}

//++++++++++++++++-----------------
void Enemy::tick()
{
	POS += VELOCITY;
	this->setPos(wayToCoordinate(POS));
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

sf::Vector2f Enemy::getPos(bool isMiddle)
{
	sf::Vector2f POS_MIDDLE = wayToCoordinate(POS, LEVEL);
	if (isMiddle)
	{
		POS_MIDDLE.x += SIZE.x / 2;
		POS_MIDDLE.y += SIZE.y / 2;
	}
	return POS_MIDDLE;
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

sf::RectangleShape* Enemy::getShape()
{
	return &OBJ;
}

bool Enemy::operator<(const Enemy& other) const
{
	return this->LAYER<other.LAYER;
}

bool Enemy::checkBullet(Bullet& bullet)
{
	bool result = 0;
	if (isIntersected((*bullet.getShape()), OBJ)) {
		result = 1;
		bullet.complete();
	}
	return result;
}


//============================BULLET====================================

Bullet::Bullet(Tower::EnumTowerType type, Enemy* target, sf::Vector2f startPos)
{
	if (!target)
		throw "no_target";
	std::string bulletType = "";
	switch (type)
	{
	case Tower::tower1:
		bulletType = "tower1";
		break;
	case Tower::tower2:
		bulletType = "tower2";
		break;
	case Tower::tower3:
		bulletType = "tower3";
		break;
	case Tower::tower4:
		bulletType = "tower4";
		break;
	default:
		bulletType = "tower1";
		break;
	}
	POS = startPos;
	DAMAGE = (float)JSONSettings["BULLET"][bulletType]["damage"] 
		* (float)JSONSettings["BULLET"]["damageCoeficent"];
	LAYER = JSONSettings["BULLET"]["layer"];
	TARGET = target;
	SIZE = sf::Vector2f(
		(float)JSONSettings["BULLET"][bulletType]["size"][0],
		(float)JSONSettings["BULLET"][bulletType]["size"][1]
	);
	OBJ = sf::RectangleShape(SIZE);
	OBJ.setPosition(startPos);
	TEXTURE.loadFromFile(JSONSettings["BULLET"][bulletType]["texture"]);
	OBJ.setTexture(&TEXTURE);
	TYPE = bullet;
	IS_FLY = 1;
	VELOCITY = (float)JSONSettings["BULLET"][bulletType]["velocity"]
		* JSONSettings["BULLET"]["velocityCoeficent"];
	DRAW_STATUS = 1;
}

uint8_t Bullet::getLayer()
{
	return LAYER;
}

void Bullet::setLayer(uint8_t layer){
	LAYER = layer;
}

sf::Vector2f Bullet::getSize(){
	return SIZE;
}

sf::Vector2f Bullet::getPos(bool isMiddle){
	sf::Vector2f pos = POS;
	if (isMiddle)
	{
		pos.x += SIZE.x / 2;
		pos.y += SIZE.y / 2;
	}
	return pos;
}

void Bullet::setMove(sf::Vector2f vector){
	OBJ.move(vector);
	POS = OBJ.getPosition();
}

void Bullet::setPos(sf::Vector2f pos, bool toMiddle){
	if (toMiddle)
	{
		pos.x -= SIZE.x / 2;
		pos.y -= SIZE.y / 2;
	}
	OBJ.setPosition(pos);
	POS = OBJ.getPosition();
}

void Bullet::setDrowStatus(bool status){
	DRAW_STATUS = status;
}

bool Bullet::getDrowStatus(){
	return DRAW_STATUS;
}

void Bullet::draw(sf::RenderWindow* window){
	window->draw(OBJ);
}

EnumGameObjects Bullet::getTypeObjet(){
	return TYPE;
}

IGameObject* Bullet::getPtr(){
	return this;
}

void Bullet::tick(){
	move();
}

sf::RectangleShape* Bullet::getShape(){
	return &OBJ;
}

sf::Vector2f Bullet::getVectorToTarget(bool isNormalise){
	if (isNormalise)
		return normalize(TARGET->getPos() - POS);
	return TARGET->getPos() - POS;
}

bool Bullet::operator<(const Bullet& other) const{
	return LAYER < other.LAYER;
}

void Bullet::move(){
	setMove(getVectorToTarget() * VELOCITY);
}

void Bullet::complete(){
	IS_FLY = 0;
	//DRAW_STATUS = 0;
}

bool Bullet::isCompleted(){
	return !IS_FLY;
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

float getDistance(sf::Vector2f vector1, sf::Vector2f vector2)
{
	return sqrt((vector1.x - vector2.x) * (vector1.x - vector2.x) + (vector1.y - vector2.y) * (vector1.y - vector2.y));
}

bool isIntersected(sf::RectangleShape obj1, sf::RectangleShape obj2)
{
	bool result = 0;
	sf::Vector2f size = obj1.getSize();
	sf::Vector2f point[4];
	for (uint8_t i = 0; i < 4; i++) {
		point[i] = obj1.getPosition();
		point[i].x += size.x * (i % 3 != 0);
		point[i].y += size.y * (i > 1);
		result |= isPointIntoShape(point[i], obj2);
	}
	return result;
}

bool isPointIntoShape(sf::Vector2f point, sf::RectangleShape obj)
{
	sf::Vector2f pos = obj.getPosition();
	sf::Vector2f size = obj.getSize();
	return ((point.x > pos.x && point.x < pos.x + size.x)
		&& (point.y > pos.y && point.y < pos.y + size.y));
}
