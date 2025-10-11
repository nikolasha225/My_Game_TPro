#include "functions.h"


//=================================JSON=============================================
json JSONSettings = json::parse(std::ifstream("./config/settings.json"));
//json JSONScore = json::parse(std::ifstream("./config/score.json"));

//=================================ПЕРЕМЕННЫЕ=======================================
//определяем extern переменные
uint8_t LEVEL = 1;
unsigned MONEY = 0;
float HEALTH = 0;
uint8_t DIFFICULT = 1;

//==================================ENEMY==============================================
Enemy::Enemy(EnumEnemyType type)
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
		typeOfGet = "boss";
		break;
	default:
		typeOfGet = "basic";
		break;
	}

	POS = 0;
	HP = JSONSettings["ENEMY"][typeOfGet]["hp"];
	OBJ = sf::RectangleShape(
		sf::Vector2f(
			JSONSettings["ENEMY"][typeOfGet]["size"][0],
			JSONSettings["ENEMY"][typeOfGet]["size"][1]
		)
	);
	TEXTURE.loadFromFile(JSONSettings["ENEMY"][typeOfGet]["texture"]);
	OBJ.setTexture(&TEXTURE);
	VELOCITY = (float)JSONSettings["ENEMY"]["velocityCoeficent"]
		* (float)JSONSettings["ENEMY"][typeOfGet]["velocity"]
		* getWayCoeficent()
		* (float)JSONSettings["ENEMY"]["levelCoeficent"][DIFFICULT - 1];
	OBJ.setPosition(100, 100);
	DAMAGE = (float)JSONSettings["ENEMY"][typeOfGet]["damage"]
		* (float)JSONSettings["ENEMY"]["damageCoeficent"];
	PRICE = (unsigned)JSONSettings["ENEMY"][typeOfGet]["money"]
		* (unsigned)JSONSettings["ENEMY"]["moneyCoeficent"];

	START_HP = HP;
}

float Enemy::getHP(bool startHP)
{
	if (startHP)
		return START_HP;
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
		pos.x -= OBJ.getSize().x / 2;
		pos.y -= OBJ.getSize().y / 2;
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
	return OBJ.getSize();
}

sf::Vector2f Enemy::getPos(bool isMiddle)
{
	sf::Vector2f POS_MIDDLE = wayToCoordinate(POS, LEVEL);
	if (!isMiddle)
	{
		POS_MIDDLE.x -= OBJ.getSize().x / 2;
		POS_MIDDLE.y -= OBJ.getSize().y / 2;
	}
	return POS_MIDDLE;
}

void Enemy::setMove(sf::Vector2f vector)
{
	OBJ.move(vector);
}

void Enemy::setDrawStatus(bool status)
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

bool Enemy::getDrawStatus()
{
	return DRAW_STATUS;
}

sf::RectangleShape* Enemy::getShape()
{
	return &OBJ;
}

bool Enemy::operator<(const Enemy& other) const
{
	return this->LAYER < other.LAYER;
}

bool Enemy::checkBullet(Bullet* bullet)
{
	bool result = 0;
	if (isIntersected(*(bullet->getShape()), OBJ)) {
		result = 1;
		bullet->complete();
	}
	return result;
}

unsigned Enemy::getMoney()
{
	return PRICE;
}


//============================BULLET====================================

Bullet::Bullet(Tower::EnumTowerType type, Enemy* target, sf::Vector2f startPos)
{
	if (!target)
		throw "no_target";
	std::string bulletType = "";
	switch (type)
	{
	case Tower::defender:
		bulletType = "defender";
		break;
	case Tower::avast:
		bulletType = "avast";
		break;
	case Tower::drWeb:
		bulletType = "drWeb";
		break;
	case Tower::kaspersky:
		bulletType = "kaspersky";
		break;
	default:
		bulletType = "defender";
		break;
	}

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
	OBJ.setPosition(startPos);
	IS_FLY = 1;
	VELOCITY = (float)JSONSettings["BULLET"][bulletType]["velocity"]
		* JSONSettings["BULLET"]["velocityCoeficent"];
	DRAW_STATUS = 1;
}

uint8_t Bullet::getLayer()
{
	return LAYER;
}

void Bullet::setLayer(uint8_t layer) {
	LAYER = layer;
}

sf::Vector2f Bullet::getSize() {
	return SIZE;
}

sf::Vector2f Bullet::getPos(bool isMiddle) {
	sf::Vector2f pos = OBJ.getPosition();
	if (isMiddle)
	{
		pos.x += SIZE.x / 2;
		pos.y += SIZE.y / 2;
	}
	return pos;
}

void Bullet::setMove(sf::Vector2f vector) {
	OBJ.move(vector);
}

void Bullet::setPos(sf::Vector2f pos, bool toMiddle) {
	if (toMiddle)
	{
		pos.x -= SIZE.x / 2;
		pos.y -= SIZE.y / 2;
	}
	OBJ.setPosition(pos);
}

void Bullet::setDrawStatus(bool status) {
	DRAW_STATUS = status;
}

bool Bullet::getDrawStatus() {
	return DRAW_STATUS;
}

void Bullet::draw(sf::RenderWindow* window) {
	window->draw(OBJ);
}

EnumGameObjects Bullet::getTypeObjet() {
	return bullet;
}

IGameObject* Bullet::getPtr() {
	return this;
}

void Bullet::tick() {
	move();
}

sf::RectangleShape* Bullet::getShape() {
	return &OBJ;
}

sf::Vector2f Bullet::getVectorToTarget(bool isNormalise) {
	if (isNormalise)
		return normalize(TARGET->getPos() - OBJ.getPosition() - SIZE / 2.f);
	return TARGET->getPos() - OBJ.getPosition() - SIZE / 2.f;
}

bool Bullet::operator<(const Bullet& other) const {
	return LAYER < other.LAYER;
}

void Bullet::move() {
	setMove(getVectorToTarget() * VELOCITY);
}

void Bullet::complete() {
	IS_FLY = 0;
	//DRAW_STATUS = 0;
}

bool Bullet::isCompleted() {
	return !IS_FLY;
}

float Bullet::getDamage()
{
	return DAMAGE;
}

bool Bullet::targetIsDie()
{
	return TARGET->isDie();
}

void Bullet::multDamage(float coef)
{
	DAMAGE *= coef;
}

void Bullet::multVelocity(float coef)
{
	VELOCITY *= coef;
}

void Bullet::setDamage(float damage)
{
	DAMAGE = damage;
}

//============================TOWER=====================================

//---------------------------------------------------
Tower::Tower(EnumTowerType type, std::vector<Bullet*>* bullets, sf::Vector2f pos)
{

}

uint8_t Tower::getLayer()
{
	return LAYER;
}

void Tower::setLayer(uint8_t layer)
{
	LAYER = layer;
}

sf::Vector2f Tower::getSize()
{
	return OBJ.getSize();
}

sf::Vector2f Tower::getPos(bool isMiddle)
{
	sf::Vector2f POS_MIDDLE = OBJ.getPosition();
	if (!isMiddle)
	{
		POS_MIDDLE.x -= OBJ.getSize().x / 2;
		POS_MIDDLE.y -= OBJ.getSize().y / 2;
	}
	return POS_MIDDLE;
}

void Tower::setMove(sf::Vector2f vector)
{
	OBJ.move(vector);
}

void Tower::setPos(sf::Vector2f pos, bool toMiddle)
{
	if (toMiddle)
	{
		pos.x -= OBJ.getSize().x / 2;
		pos.y -= OBJ.getSize().y / 2;
	}
	OBJ.setPosition(pos);
}

void Tower::setDrawStatus(bool status)
{
	DRAW_STATUS = status;
}

bool Tower::getDrawStatus()
{
	return DRAW_STATUS;
}

void Tower::draw(sf::RenderWindow* window)
{
	window->draw(OBJ);
}

EnumGameObjects Tower::getTypeObjet()
{
	return tower;
}

IGameObject* Tower::getPtr()
{
	return this;
}

//---------------------------------------------------
void Tower::tick()
{

}

sf::RectangleShape* Tower::getShape()
{
	return &OBJ;
}

//---------------------------------------------------
Bullet* Tower::shoot(Enemy* target)
{
	Bullet newBullet(TYPE, target, getPos());
	newBullet.setDamage(BULLET_DAMAGE);
	newBullet.multVelocity(BULLET_VELOCITY_COEF);
	BULLET_BUFF->push_back(&newBullet);
}

Enemy* Tower::getTarget(std::vector<Enemy*> vector)
{
	float lastDistance = vector[0]->getPosPercent();
	Enemy* lastEnemy = vector[0];
	for (auto& ENEMY : vector) {
		float distance = ENEMY->getPosPercent();
		if (lastDistance > distance) {
			lastDistance = distance;
			lastEnemy = ENEMY;
		}
	}
	return lastEnemy;
}

void Tower::upgrade(uint8_t level)
{
	if (TOWER_LEVEL + level > 5)
		return;
	TOWER_LEVEL += level;
	float coef = JSONSettings["TOWER"]["upgrade"][TOWER_LEVEL];
	float lastCoef = JSONSettings["TOWER"]["upgrade"][(TOWER_LEVEL>0)?(TOWER_LEVEL-1):(0)];
	BULLET_DAMAGE *= coef / lastCoef;
	BULLET_VELOCITY_COEF *= coef / lastCoef;
	TOWER_VELOCITY /= coef / lastCoef;
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
	for (auto& i : vector)
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
		for (auto& obj : stack[i]) {
			sf::Vector2f pos = obj->getPos();
			obj->setPos(getNewCoordinate(pos));
			obj->draw(window);
			obj->setPos(pos);
		}
}

void OBJStack::sortByLayer()
{
	for (auto i : renderLine)
		std::sort(stack[i].begin(), stack[i].end());

}

bool OBJStack::deleteObj(IGameObject* obj)
{
	EnumGameObjects type = obj->getTypeObjet();
	auto it = std::find(
		stack[type].begin(),
		stack[type].end(),
		obj
	);
	if (it != stack[type].end()) {
		stack[type].erase(it);
		return 1;
	}
	return 0;
}


//--------------------------------+++++++++++++++++++++++
void OBJStack::tick()
{
	for (auto& i : renderLineReverce)
		for (auto& obj : stack[i])
			obj->tick();
	for (auto& ENEMY : stack[enemy])
		for (auto& BULLET : stack[bullet])
		{
			Enemy* nowEnemy = (Enemy*)ENEMY;
			Bullet* nowBullet = (Bullet*)BULLET;
			if (nowEnemy->checkBullet(nowBullet))
			{

				//иожно ещё проверять что пуля попала только по одному,но так интереснее (типо прострел)
				nowEnemy->subHP(nowBullet->getDamage());
				nowBullet->complete();
				float colorCoef = nowEnemy->getHP() / nowEnemy->getHP(1);
				nowEnemy->
					getShape()->
					setFillColor(
						sf::Color(
							255,
							255 * colorCoef,
							255 * colorCoef
						)
					);
			}
		}

	//проверяем какие пули достигли цели и у каких цель умерла
	for (auto& BULLET : stack[bullet])
		if (((Bullet*)BULLET)->isCompleted() || ((Bullet*)BULLET)->targetIsDie())
			deleteObj(BULLET);

	//проверяем кто умер
	for (auto& ENEMY : stack[enemy])
		if (((Enemy*)ENEMY)->isDie()) {
			MONEY += ((Enemy*)ENEMY)->getMoney();
			deleteObj(ENEMY);
		}
}

//============================ОТДЕЛЬНЫЕ ФУНКЦИИ==========================

sf::Vector2f getPositionOnPathByDistance(float pos, const std::vector<sf::Vector2f> pathPoints)
{

	float totalLength = 0.0f;	        // общая длина пути
	std::vector<float> segmentLengths;  // длины сегментов

	for (size_t i = 0; i < pathPoints.size() - 1; ++i) {
		float segmentLength = getDistance(pathPoints[i], pathPoints[i + 1]);
		segmentLengths.push_back(segmentLength);
		totalLength += segmentLength;
	}

	// целевое расстояние, которое должен пройти враг
	float targetDistance = (pos / 100.0f) * totalLength;

	// проходим по сегментам, пока не найдем нужный
	float accumulatedDistance = 0.0f;
	for (size_t i = 0; i < pathPoints.size() - 1; ++i) {
		float segmentLength = segmentLengths[i];

		if (targetDistance <= accumulatedDistance + segmentLength) {
			float t = (targetDistance - accumulatedDistance) / segmentLength;
			return sf::Vector2f(pathPoints[i].x + t * (pathPoints[i + 1].x - pathPoints[i].x), pathPoints[i].y + t * (pathPoints[i + 1].y - pathPoints[i].y));
		}
		accumulatedDistance += segmentLength;
	}

	// Если дошли до конца, возвращаем последнюю точку
	return pathPoints.back();
}

sf::Vector2f getNewCoordinate(sf::Vector2f pos)
{
	sf::Vector2f nowSize(
		(float)JSONSettings["GENERAL"]["resolution"][0],
		(float)JSONSettings["GENERAL"]["resolution"][1]
	);
	if (nowSize.x == 1920 && nowSize.y == 1080)
		return pos;
	pos.x *= nowSize.x / 1920;
	pos.y *= nowSize.y / 1080;
	return pos;
}

sf::Vector2f wayToCoordinate(float pos, uint8_t level)
{
	if (pos < 0.0f || (level > 3 || level < 1)) {
		return sf::Vector2f(0, 0);
	}
	else if (pos > 100.0f) {
		return sf::Vector2f(1920, 1080);
	}

	return getPositionOnPathByDistance(pos, wayPoints[level - 1]);
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

float getWayCoeficent(uint8_t level)
{
	return getWayLength(wayPoints[2]) / getWayLength(wayPoints[level - 1]);
}

float getWayLength(std::vector<sf::Vector2f> pathPoints)
{
	if (pathPoints.size() < 2) {
		return 0.f;
	}

	float totalLength = 0.f;

	for (size_t i = 1; i < pathPoints.size(); i++) {
		sf::Vector2f delta = pathPoints[i] - pathPoints[i - 1];
		totalLength += std::sqrt(delta.x * delta.x + delta.y * delta.y);
	}

	return totalLength;
}