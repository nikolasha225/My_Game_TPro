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
		* (float)JSONSettings["ENEMY"]["difficultCoeficent"][DIFFICULT - 1];
	OBJ.setPosition(100, 100);
	DAMAGE = (float)JSONSettings["ENEMY"][typeOfGet]["damage"]
		* (float)JSONSettings["ENEMY"]["damageCoeficent"];
	PRICE = (unsigned)JSONSettings["ENEMY"][typeOfGet]["money"]
		* (unsigned)JSONSettings["ENEMY"]["moneyCoeficent"];
	START_HP = HP;
	OBJ.setOrigin(OBJ.getSize().x / 2, OBJ.getSize().y / 2);
	SOUND_BUFF.loadFromFile(JSONSettings["ENEMY"]["dieSound"]);
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
	if (!toMiddle)
	{
		pos.x += OBJ.getSize().x / 2;
		pos.y += OBJ.getSize().y / 2;
	}
	OBJ.setPosition(pos);
}

//++++++++++++++++-----------------
void Enemy::tick()
{
	POS += VELOCITY;
	this->setPos(wayToCoordinate(POS));
	if (POS >= 100) {
		SOUND_BUFF.loadFromFile(JSONSettings["ENEMY"]["damageSound"]);
		HEALTH -= getDamage();
		HP = -1;
	}
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

float Enemy::getDamage()
{
	return DAMAGE;
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

void Enemy::sound()
{
	SOUND.setBuffer(SOUND_BUFF);
	//sound.setVolume(99);
	SOUND.play();
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

void Enemy::setSize(sf::Vector2f size)
{
	OBJ.setSize(size);
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
	OBJ = sf::RectangleShape(
		sf::Vector2f(
			(float)JSONSettings["BULLET"][bulletType]["size"][0],
			(float)JSONSettings["BULLET"][bulletType]["size"][1]
		)
	);
	TEXTURE.loadFromFile(JSONSettings["BULLET"][bulletType]["texture"]);
	OBJ.setTexture(&TEXTURE);
	OBJ.setPosition(startPos);
	IS_FLY = 1;
	VELOCITY = (float)JSONSettings["BULLET"][bulletType]["velocity"]
		* JSONSettings["BULLET"]["velocityCoeficent"];
	DRAW_STATUS = 1;
	OBJ.setOrigin(OBJ.getSize().x / 2, OBJ.getSize().y / 2);
}

uint8_t Bullet::getLayer()
{
	return LAYER;
}

void Bullet::setLayer(uint8_t layer) {
	LAYER = layer;
}

sf::Vector2f Bullet::getSize() {
	return OBJ.getSize();
}

sf::Vector2f Bullet::getPos(bool isMiddle) {
	sf::Vector2f pos = OBJ.getPosition();
	if (isMiddle)
	{
		pos.x += getSize().x / 2;
		pos.y += getSize().y / 2;
	}
	return pos;
}

void Bullet::setMove(sf::Vector2f vector) {
	OBJ.move(vector);
}

void Bullet::setPos(sf::Vector2f pos, bool toMiddle) {
	if (!toMiddle)
	{
		pos.x -= getSize().x / 2;
		pos.y -= getSize().y / 2;
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

void Bullet::setSize(sf::Vector2f size)
{
	OBJ.setSize(size);
}

sf::Vector2f Bullet::getVectorToTarget(bool isNormalise) {
	if (isNormalise)
		return normalize(TARGET->getPos() - OBJ.getPosition() - getSize() / 2.f);
	return TARGET->getPos() - OBJ.getPosition() - getSize() / 2.f;
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


Tower::Tower(EnumTowerType type, OBJStack* stack, sf::Vector2f pos)
{
	std::string stringType = "defender";
	switch (type)
	{
	case Tower::defender:
		break;
	case Tower::avast:
		stringType = "avast";
		break;
	case Tower::drWeb:
		stringType = "drWeb";
		break;
	case Tower::kaspersky:
		stringType = "kaspersky";
		break;
	default:
		break;
	}
	LAYER = JSONSettings["TOWER"]["layer"];
	OBJ = sf::RectangleShape(
		sf::Vector2f(
			JSONSettings["TOWER"][stringType]["size"][0],
			JSONSettings["TOWER"][stringType]["size"][1]
		)
	);
	TEXTURE.loadFromFile(JSONSettings["TOWER"][stringType]["texture"]);
	PRICE = (unsigned)JSONSettings["TOWER"][stringType]["price"]
		* (unsigned)JSONSettings["TOWER"]["priceCoeficent"];
	TOWER_VELOCITY = (unsigned)JSONSettings["TOWER"][stringType]["framePerShoot"]
		* (unsigned)JSONSettings["TOWER"]["framePerShootCoeficent"];
	TOWER_LEVEL = 1;
	BULLET_DAMAGE = (float)JSONSettings["BULLET"][stringType]["damage"]
		* (float)JSONSettings["BULLET"]["damageCoeficent"];
	BULLET_VELOCITY_COEF = 1;
	TYPE = type;
	setPos(pos);
	OBJ.setTexture(&TEXTURE);
	OBJ.setOrigin(OBJ.getSize().x / 2, OBJ.getSize().y / 2);
	DRAW_STATUS = 1;
	STATE_SHOOT = 0;
	SHOOT_SCALE = JSONSettings["TOWER"]["shootScale"];
	STACK = stack;
	RANGE = (float)JSONSettings["TOWER"][stringType]["range"]
		* (float)JSONSettings["TOWER"]["rangeCoeficent"];
	SOUND_BUFF.loadFromFile(JSONSettings["BULLET"]["shootSound"]);
	SOUND.setVolume(10);
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
	if (!toMiddle)
	{
		pos.x += OBJ.getSize().x / 2;
		pos.y += OBJ.getSize().y / 2;
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

void Tower::sound()
{
	SOUND.setBuffer(SOUND_BUFF);
	SOUND.play();
}

//+++++++++++++++++++++---------------------------
void Tower::tick()
{
	STATE_SHOOT = (STATE_SHOOT >= TOWER_VELOCITY)
		? (TOWER_VELOCITY)
		: (STATE_SHOOT + 1);
	float coefSize = (1.f - SHOOT_SCALE)
		+ (SHOOT_SCALE * ((float)(STATE_SHOOT) / (float)(TOWER_VELOCITY)));
	if (STATE_SHOOT >= TOWER_VELOCITY)
		shoot(getTarget());
	OBJ.setScale(sf::Vector2f(coefSize,coefSize));
}

sf::RectangleShape* Tower::getShape()
{
	return &OBJ;
}

Bullet* Tower::shoot(Enemy* target)
{
	if (!target)
		return nullptr;
	Bullet* newBullet = new Bullet(TYPE, target, getPos()); 
	newBullet->setDamage(BULLET_DAMAGE);
	newBullet->multVelocity(BULLET_VELOCITY_COEF);
	STACK->add(newBullet);
	STATE_SHOOT = 0;
	sound();
	return newBullet;
}

Enemy* Tower::getTarget()
{
	if (STACK->getCountOf(enemy) == 0)
		return nullptr;
	std::vector<Enemy*> enemys;
	for (auto& i : (*(STACK->getStack()))[enemy])
		enemys.push_back((Enemy*)i);
	float lastDistance = enemys[0]->getPosPercent();
	Enemy* lastEnemy = enemys[0];
	for (auto& ENEMY : enemys) {
		float distance = ENEMY->getPosPercent();
		if (lastDistance < distance) {
			lastDistance = distance;
			lastEnemy = ENEMY;
		}
	}
	
	return (getDistance(lastEnemy->getPos(), this->getPos()) <= RANGE)
		? (lastEnemy)
		: nullptr;
}

void Tower::upgrade(uint8_t level)
{
	if (TOWER_LEVEL + level > 5)
		return;
	TOWER_LEVEL += level;
	float coef = JSONSettings["TOWER"]["upgrade"][TOWER_LEVEL];
	float lastCoef = JSONSettings["TOWER"]["upgrade"][(TOWER_LEVEL > 0) ? (TOWER_LEVEL - 1) : (0)];
	BULLET_DAMAGE *= coef / lastCoef;
	BULLET_VELOCITY_COEF *= coef / lastCoef;
	TOWER_VELOCITY = (float) TOWER_VELOCITY * ( coef / lastCoef);
}

Tower::EnumTowerType Tower::getTowerType()
{
	return TYPE;
}

unsigned Tower::getPrice()
{
	return PRICE;
}

void Tower::setSize(sf::Vector2f size)
{
	OBJ.setSize(size);
}

//============================OBJ STACK=================================

OBJStack::OBJStack()
{
	stack = {
	{enemy, {}},
	{tower, {}},
	{core, {new Core}},
	{backgroundStatic, {}},
	{backgroundDynamic, {}},
	{menuWindowObject, {}},
	{effect, {}},
	{bullet, {}}
	};
	deleted = {};

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
			sf::Vector2f size = obj->getSize();
			obj->setPos(getNewCoordinate(pos));
			obj->setSize(getNewCoordinate(size));
			obj->draw(window);
			obj->setPos(pos);
			obj->setSize(size);
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
		deleted.push_back(*it);
		stack[type].erase(it);
		return 1;
	}
	return 0;
}

OBJStack* OBJStack::getPtr() {
	return this;
}

size_t OBJStack::getCountOf(EnumGameObjects type)
{
	return stack[type].size();
}

std::map<EnumGameObjects, std::vector<IGameObject*>>* OBJStack::getStack()
{
	return &stack;
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
			((Enemy*)ENEMY)->sound();
			MONEY += ((Enemy*)ENEMY)->getMoney();
			deleteObj(ENEMY);
		}

}

//================================CORE===================================

Core::Core()
{
	CORE = sf::RectangleShape(
		sf::Vector2f(
			JSONSettings["CORE"]["coreSize"][0],
			JSONSettings["CORE"]["coreSize"][1]
		)
	);
	TEXTURE.loadFromFile(JSONSettings["CORE"]["coreTexture"]);
	CORE.setTexture(&TEXTURE);
	LAYER = 30;
	TICK_DAMAGE = JSONSettings["CORE"]["tickOfDamage"];
	TICK_COUNTER = TICK_DAMAGE;
	LAST_HEALTH = HEALTH;
	CORE.setOrigin(CORE.getSize().x / 2, CORE.getSize().y / 2);
	CORE.setPosition(
		wayToCoordinate(100.f) 
		+ sf::Vector2f(0, CORE.getSize().y/2)
	);
	DRAW_STATUS = 1;
	MOVE = JSONSettings["CORE"]["coreMove"];
}

//-----------------------------
void Core::tick() 
{
	TICK_COUNTER += (TICK_COUNTER < TICK_DAMAGE);
	TICK_COUNTER *= !isDamaged();
	if (TICK_COUNTER < TICK_DAMAGE) {
		float coef = (float)TICK_COUNTER / (float)TICK_DAMAGE;
		CORE.setFillColor(
			sf::Color(
				255,
				255 * coef,
				255 * coef
			)
		);
		CORE.setPosition(
			wayToCoordinate(100.f)
			+ sf::Vector2f(0, CORE.getSize().y / 2)
		);
		CORE.move(
			sf::Vector2f(
				RAND_FLOAT(-1.f, 1.f) * MOVE * coef,
				RAND_FLOAT(-1.f, 1.f) * MOVE * coef
			)
		);
	}

	if(TICK_DAMAGE == TICK_COUNTER + 1)
		CORE.setPosition(
			wayToCoordinate(100.f)
			+ sf::Vector2f(0, CORE.getSize().y / 2)
		);
}

EnumGameObjects Core::getTypeObjet() 
{
	return core;
}

void Core::draw(sf::RenderWindow* window)
{
	window->draw(CORE);
}

uint8_t Core::getLayer()
{
	return LAYER;
}

void Core::setLayer(uint8_t layer)
{
	LAYER = layer;
}

sf::Vector2f Core::getSize()
{
	return CORE.getSize();
}

void Core::setSize(sf::Vector2f size)
{
	CORE.setSize(size);
}

sf::Vector2f Core::getPos(bool)
{
	return CORE.getPosition();
}

void Core::setMove(sf::Vector2f vector)
{
	CORE.move(vector);
}

void Core::setPos(sf::Vector2f vector, bool)
{
	CORE.setPosition(vector);
}

void Core::setDrawStatus(bool status)
{
	DRAW_STATUS = status;
}

bool Core::getDrawStatus()
{
	return DRAW_STATUS;
}

IGameObject* Core::getPtr() 
{
	return this;
}

sf::RectangleShape* Core::getShape()
{
	return &CORE;
}

bool Core::isDamaged()
{
	if (HEALTH != LAST_HEALTH) {
		LAST_HEALTH = HEALTH;
		return 1;
	}
	return 0;
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
		switch (i)
		{
		case 0:
			point[i] += sf::Vector2f(size.x / -2, size.y / -2);
			break;
		case 1:
			point[i] += sf::Vector2f(size.x / 2, size.y / -2);
			break;
		case 2:
			point[i] += sf::Vector2f(size.x / 2, size.y / 2);
			break;
		case 3:
			point[i] += sf::Vector2f(size.x / -2, size.y / 2);
			break;
		default:
			break;
		}
		result |= isPointIntoShape(point[i], obj2);
	}
	return result;

	//ускоренные вычисления (менее точно)
	//sf::Vector2f size1 = obj1.getSize(), size2 = obj2.getSize();
	//return getDistance(obj1.getPosition(), obj2.getPosition())
	//	< ((size1.x / 4) + (size1.y / 4) + (size2.x / 4) + (size2.y / 4));// / 1.6;//sqrt 2 + ещё чутьчуть
}

bool isPointIntoShape(sf::Vector2f point, sf::RectangleShape obj)
{
	sf::Vector2f pos = obj.getPosition();
	sf::Vector2f size = obj.getSize();
	return ((point.x > pos.x - size.x/2 && point.x < pos.x + size.x/2)
		&& (point.y > pos.y - size.y/2 && point.y < pos.y + size.y/2));
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


//==============================DEBUG ONLY========================================

std::vector<sf::RectangleShape> createSimplePath(const std::vector<sf::Vector2f>& points, float width) {
	std::vector<sf::RectangleShape> pathSegments;

	if (points.size() < 2) {
		return pathSegments;
	}

	// Серо-синий цвет
	sf::Color pathColor(100, 120, 180);

	for (size_t i = 0; i < points.size() - 1; ++i) {
		const sf::Vector2f& start = points[i];
		const sf::Vector2f& end = points[i + 1];

		// Вычисляем длину и угол сегмента
		sf::Vector2f direction = end - start;
		float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
		float angle = std::atan2(direction.y, direction.x) * 180 / 3.14159f;

		// Создаем прямоугольник для сегмента пути
		sf::RectangleShape segment(sf::Vector2f(length, width));
		segment.setFillColor(pathColor);
		segment.setPosition(start);
		segment.setRotation(angle);

		// Смещаем так чтобы путь был центрирован относительно линии
		segment.move(-width / 2 * std::sin(angle * 3.14159f / 180),
			width / 2 * std::cos(angle * 3.14159f / 180));

		pathSegments.push_back(segment);
	}

	return pathSegments;
}