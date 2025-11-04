#include "game.h"

//==============================SPAWNER===================================
Spawner::Spawner(sf::RenderWindow* window, OBJStack* stack)
{
	WINDOW = window;
	STACK = stack;
	for (uint8_t i = Enemy::basicVirus; i <= Enemy::bossVirus ; i++) {
        COUNT_MOBS[i] = (float)JSONSettings["ENEMY"][enemyTypes[i]]["mobCount"]
			* (float)JSONSettings["ENEMY"]["difficultCoeficent"][DIFFICULT]
			* (float)RAND_FLOAT(1 - JSONSettings["ENEMY"]["mobCountDisperce"], 1 + JSONSettings["ENEMY"]["mobCountDisperce"]);
        START_COUNT_MOBS[i] = COUNT_MOBS[i];
		TICK_OF_ENEMY[i] = 0;
	}
	PROGRESS = 0.f;
    START_COUNT_ALL = getSummArray(START_COUNT_MOBS);

    SOUND_BUFF.loadFromFile(JSONSettings["ENEMY"]["boss"]["spawnSound"]);
    SOUND.setVolume(70);
}

void Spawner::tick() {
    bool* allowSpawn = allowSpawnEnemy();
    for (uint8_t i = 0; i < 5; i++) {
        TICK_OF_ENEMY[i]++;
        if (allowSpawn[i])
            STACK->add(new Enemy((Enemy::EnumEnemyType)i));
    }
    if (allowSpawn[4]) {
        SOUND.setBuffer(SOUND_BUFF);
        SOUND.play();
    }
}

bool* Spawner::allowSpawnEnemy()
{
    static bool allowSpawn[5] = { false };

    // Константы волн
    const uint8_t TOTAL_WAVES = 10;
    const unsigned WAVE_DURATION_TICKS = START_TIME / TOTAL_WAVES;

    // Соотношения спавна по волнам
    const float SPAWN_RATIOS[5][10] = {
        {1.2f, 1.1f, 1.0f, 1.0f, 0.9f, 0.9f, 0.8f, 0.8f, 0.7f, 0.6f},
        {0.9f, 0.8f, 1.2f, 1.5f, 1.3f, 1.1f, 0.9f, 0.7f, 0.5f, 0.7f},
        {0.0f, 0.0f, 0.1f, 0.7f, 1.0f, 1.3f, 1.4f, 1.2f, 0.9f, 0.5f},
        {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.8f, 1.5f, 1.8f, 1.2f, 0.7f},
        {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.5f, 2.5f}
    };

    // Константы интервалов спавна
    const unsigned MIN_INTERVAL = 5;                  // Минимальный интервал между спавнами
    const unsigned MAX_INTERVAL = 280;                // Максимальный интервал между спавнами

    // Константы вероятности спавна
    const float BASE_SPAWN_CHANCE = 0.5f;             // Базовая вероятность спавна
    const float HIGH_SPAWN_CHANCE = 0.9f;             // Высокая вероятность при отставании
    const float LOW_SPAWN_CHANCE = 0.3f;              // Низкая вероятность при опережении

    // Константы коррекции интервалов
    const float ACCELERATION_CORRECTION = 0.7f;       // Коррекция при ускорении (отставание)
    const float DECELERATION_CORRECTION = 1.3f;       // Коррекция при замедлении (опережение)

    // Константы отклонений от графика
    const int MIN_DEVIATION_FOR_CORRECTION = 1;       // Минимальное отклонение для коррекции
    const int STRONG_DEVIATION_THRESHOLD = 2;         // Порог сильного отклонения

    // Константы случайной вариации
    const float MIN_VARIATION = 0.9f;                 // Минимальная вариация количества мобов
    const float MAX_VARIATION = 1.1f;                 // Максимальная вариация количества мобов

    // Рассчитываем текущую волну
    float roundProgress = 1.0f - ((float)TIME / (float)START_TIME);
    uint8_t currentWave = (uint8_t)(roundProgress / (1.0f / TOTAL_WAVES));
    if (currentWave >= TOTAL_WAVES) currentWave = TOTAL_WAVES - 1;

    // Прогресс внутри текущей волны (0.0 - 1.0)
    unsigned currentWaveStartTick = START_TIME - (currentWave + 1) * WAVE_DURATION_TICKS;
    unsigned currentWaveEndTick = START_TIME - currentWave * WAVE_DURATION_TICKS;
    float waveProgress = (float)(START_TIME - TIME - currentWaveStartTick) / (float)WAVE_DURATION_TICKS;

    // Предварительно вычисляем проценты распределения для каждого типа
    float spawnPercentages[5][10] = { 0 };
    for (uint8_t i = 0; i < 5; i++) {
        float totalRatio = 0.0f;
        for (uint8_t wave = 0; wave < TOTAL_WAVES; wave++) {
            totalRatio += SPAWN_RATIOS[i][wave];
        }

        for (uint8_t wave = 0; wave < TOTAL_WAVES; wave++) {
            if (totalRatio > 0.0f) {
                spawnPercentages[i][wave] = (SPAWN_RATIOS[i][wave] / totalRatio) * 100.0f;
            }
        }
    }

    // Рассчитываем целевое количество мобов для текущей волны
    unsigned targetMobsForCurrentWave[5] = { 0 };

    for (uint8_t i = Enemy::basicVirus; i <= Enemy::bossVirus; i++) {
        targetMobsForCurrentWave[i] = (unsigned)(START_COUNT_MOBS[i] * spawnPercentages[i][currentWave] / 100.0f);

        // Добавляем небольшую случайную вариацию
        float variation = RAND_FLOAT(MIN_VARIATION, MAX_VARIATION);
        targetMobsForCurrentWave[i] = (unsigned)(targetMobsForCurrentWave[i] * variation);

        // Минимум 1 моб если должно спавниться
        if (SPAWN_RATIOS[i][currentWave] > 0.0f && targetMobsForCurrentWave[i] == 0) {
            targetMobsForCurrentWave[i] = 1;
        }
    }

    // Рассчитываем сколько должно быть заспавнено к текущему моменту волны
    unsigned shouldBeSpawnedByNow[5] = { 0 };
    for (uint8_t i = Enemy::basicVirus; i <= Enemy::bossVirus; i++) {
        shouldBeSpawnedByNow[i] = (unsigned)(targetMobsForCurrentWave[i] * waveProgress);
    }

    // Рассчитываем сколько уже заспавнено в этой волне
    unsigned spawnedInThisWave[5] = { 0 };
    for (uint8_t i = Enemy::basicVirus; i <= Enemy::bossVirus; i++) {
        unsigned spawnedBeforeThisWave = 0;
        for (uint8_t wave = 0; wave < currentWave; wave++) {
            unsigned waveTarget = (unsigned)(START_COUNT_MOBS[i] * spawnPercentages[i][wave] / 100.0f);
            spawnedBeforeThisWave += waveTarget;
        }

        unsigned totalSpawned = START_COUNT_MOBS[i] - COUNT_MOBS[i];
        if (totalSpawned > spawnedBeforeThisWave) {
            spawnedInThisWave[i] = totalSpawned - spawnedBeforeThisWave;
        }
    }

    for (uint8_t i = Enemy::basicVirus; i <= Enemy::bossVirus; i++) {
        allowSpawn[i] = false;
        if (COUNT_MOBS[i] <= 0) continue;

        // Проверяем, должен ли этот тип моба спавниться в текущей волне
        if (targetMobsForCurrentWave[i] == 0) continue;

        // Проверяем, не превысили ли лимит для этой волны
        if (spawnedInThisWave[i] >= targetMobsForCurrentWave[i]) continue;

        // Вычисляем интервал спавна на основе длительности волны и количества мобов
        unsigned idealSpawnInterval = WAVE_DURATION_TICKS / targetMobsForCurrentWave[i];

        // Ограничиваем интервал
        unsigned baseSpawnInterval = idealSpawnInterval;
        if (baseSpawnInterval < MIN_INTERVAL) baseSpawnInterval = MIN_INTERVAL;
        if (baseSpawnInterval > MAX_INTERVAL) baseSpawnInterval = MAX_INTERVAL;

        // Корректируем интервал в зависимости от отклонения от графика
        int spawnDeviation = (int)shouldBeSpawnedByNow[i] - (int)spawnedInThisWave[i];
        float deviationCorrection = 1.0f;

        if (spawnDeviation > MIN_DEVIATION_FOR_CORRECTION) {
            deviationCorrection = ACCELERATION_CORRECTION;
        }
        else if (spawnDeviation < -MIN_DEVIATION_FOR_CORRECTION) {
            deviationCorrection = DECELERATION_CORRECTION;
        }

        unsigned modifiedInterval = (unsigned)(baseSpawnInterval * deviationCorrection);

        // Проверяем интервал спавна
        if (TICK_OF_ENEMY[i] >= modifiedInterval) {
            // Базовая вероятность спавна
            float spawnChance = BASE_SPAWN_CHANCE;

            // Корректируем вероятность в зависимости от отклонения
            if (spawnDeviation > STRONG_DEVIATION_THRESHOLD) {
                spawnChance = HIGH_SPAWN_CHANCE;
            }
            else if (spawnDeviation < -STRONG_DEVIATION_THRESHOLD) {
                spawnChance = LOW_SPAWN_CHANCE;
            }

            if (RAND_FLOAT(0.0f, 1.0f) < spawnChance) {
                allowSpawn[i] = true;
                TICK_OF_ENEMY[i] = 0;
                COUNT_MOBS[i]--;
            }
        }

    }

    return allowSpawn;
}

bool Spawner::existEnemy()
{
    unsigned total = 0;
    for (uint8_t i = 0; i < 5; i++)
        total += COUNT_MOBS[i];
    return total;
}

//==============================TOWER_MANAGER==============================

TowerManager::TowerManager(OBJStack* stack)
{
    STACK = stack;
    for (auto& i : towerPoint[LEVEL-1])
        TOWERS.push_back(new Place(i, STACK));
    for (auto& i : TOWERS)
        STACK->add(i);
}

void TowerManager::checkEvents(sf::RenderWindow* window)
{
    bool clickedOnSomething = false;
    Place* clickedPlace = nullptr;

    //================== ОБРАБОТКА КЛИКОВ ПО МЕСТАМ С БАШНЯМИ ==================
    for (auto& place : TOWERS) {
        if (place->STATE == Place::placeState::tower) {
            if (mouseInButton(place->getShapePtr(), window)) {
                // Сбрасываем selectFatherTower у всех других башен
                for (auto& otherPlace : TOWERS) {
                    if (otherPlace != place && otherPlace->STATE == Place::placeState::tower) {
                        for (auto& cell : otherPlace->BUY_MENU) {
                            if (cell->STATE == DownCell::selectFatherTower) {
                                cell->STATE = DownCell::unselectFather;
                            }
                        }
                    }
                }
                for (auto& cell : place->BUY_MENU)
                {
                    cell->STATE = DownCell::selectFatherTower;
                    float price = 0;
                    if (cell->NUMBER == Tower::upgradedTower)
                        price = (float)JSONSettings["TOWER"]["upgrade"][cell->FATHER->TOWER->getLevel()]
                        * (float)JSONSettings["TOWER"][towerTypes[cell->FATHER->TOWER->getTowerType()]]["price"];
                    else
                        price = (float)JSONSettings["TOWER"]["removeCoef"]
                        * (float)JSONSettings["TOWER"][towerTypes[cell->FATHER->TOWER->getTowerType()]]["price"];
                    cell->TEXT_PRICE.setString(std::to_string((int)price));
                    sf::FloatRect textBounds = cell->TEXT_PRICE.getLocalBounds();
                    cell->TEXT_PRICE.setOrigin(
                        textBounds.left + textBounds.width / 2.f,
                        textBounds.top + textBounds.height / 2.f
                    );
                }
                clickedOnSomething = true;
                clickedPlace = place;
                break;
            }

            for (auto& cell : place->BUY_MENU) {
                if (cell->NUMBER > Tower::kaspersky && mouseInButton(cell->getCellShapePtr(), window)) {
                    if (cell->NUMBER == Tower::upgradedTower) {
                        if (place->TOWER->upgrade()) {
                            place->SOUND.setBuffer(place->SOUND_BUFF_UPGRADE);
                            place->SOUND.play();
                        }
                        else {
                            place->SOUND.setBuffer(place->SOUND_BUFF_ERROR);
                            place->SOUND.play();
                        }
                    }
                    else if (cell->NUMBER == Tower::deletedTower) {
                        place->SOUND.setBuffer(place->SOUND_BUFF_DELETE);
                        place->SOUND.play();
                        MONEY += (float)JSONSettings["TOWER"][towerTypes[place->TOWER->getTowerType()]]["price"]
                            * (float)JSONSettings["TOWER"]["removeCoef"];
                        if (place->TOWER != nullptr) {
                            STACK->remove(place->TOWER);
                            delete place->TOWER;
                            place->TOWER = nullptr;
                        }
                        place->STATE = Place::placeState::empty;
                        for (auto& menuCell : place->BUY_MENU) {
                            menuCell->unselect();
                        }
                    }
                    clickedOnSomething = true;
                    clickedPlace = place;
                    place->unselectPlace();
                    break;
                }
            }
            if (clickedOnSomething) break;
        }
    }
    if (clickedOnSomething) {
        // Снимаем выделение со всех других мест
        for (auto& place : TOWERS) {
            if (place != clickedPlace && place->isSelect()) {
                place->unselectPlace();
            }
        }
        return;
    }

    //================== ОБРАБОТКА КЛИКОВ ПО ПУСТЫМ МЕСТАМ ==================
    // Сначала проверяем клики по DownCell и DESC (только для выделенных мест)
    for (auto& place : TOWERS) {
        // Пропускаем места с башнями
        if (place->STATE == Place::placeState::tower) continue;

        // Если место выделено, проверяем клики по его меню
        if (place->isSelect()) {
            for (auto& cell : place->BUY_MENU) {
                // Клик по DownCell - показываем описание
                if (mouseInButton(cell->getCellShapePtr(), window)) {
                    // Сбрасываем состояние selectBuy у всех ячеек этого места
                    for (auto& otherCell : place->BUY_MENU) {
                        if (otherCell != cell && otherCell->STATE == DownCell::EnumCellState::selectBuy) {
                            otherCell->STATE = DownCell::selectFather;
                        }
                    }
                    cell->STATE = DownCell::selectBuy;
                    clickedOnSomething = true;
                    clickedPlace = place;
                    break;
                }

                // Клик по описанию - покупаем башню
                if (cell->STATE == DownCell::EnumCellState::selectBuy &&
                    mouseInButton(cell->getDescShapePtr(), window)) {
                    if (cell->tryBuy()) {
                        // Башня куплена успешно
                        place->unselectPlace();
                    }
                    else {
                        // Не хватило денег
                        place->SOUND.setBuffer(place->SOUND_BUFF_ERROR);
                        place->SOUND.play();
                        // Место остается выделенным для повторной попытки
                    }
                    clickedOnSomething = true;
                    clickedPlace = place;
                    break;
                }
            }
            if (clickedOnSomething) break;
        }
    }

    // Если кликнули на какой-то элемент интерфейса, выходим
    if (clickedOnSomething) {
        // Сбрасываем selectFatherTower у всех башен
        for (auto& place : TOWERS) {
            if (place->STATE == Place::placeState::tower) {
                for (auto& cell : place->BUY_MENU) {
                    if (cell->STATE == DownCell::selectFatherTower) {
                        cell->STATE = DownCell::unselectFather;
                    }
                }
            }
        }
        // Снимаем выделение со всех других мест, кроме текущего
        for (auto& place : TOWERS) {
            if (place != clickedPlace && place->isSelect()) {
                place->unselectPlace();
            }
        }
        return;
    }

    // Если ничего не кликнули в меню, проверяем клики по самим местам
    for (auto& place : TOWERS) {
        // Пропускаем места с башнями
        if (place->STATE == Place::placeState::tower) continue;

        // Клик по месту
        if (mouseInButton(place->getShapePtr(), window)) {
            // Сбрасываем selectFatherTower у всех башен
            for (auto& towerPlace : TOWERS) {
                if (towerPlace->STATE == Place::placeState::tower) {
                    for (auto& cell : towerPlace->BUY_MENU) {
                        if (cell->STATE == DownCell::selectFatherTower) {
                            cell->STATE = DownCell::unselectFather;
                        }
                    }
                }
            }

            if (!place->isSelect()) {
                place->selectPlace();
                // При выделении нового места сбрасываем все selectBuy состояния
                for (auto& cell : place->BUY_MENU) {
                    cell->STATE = DownCell::selectFather;
                }
            }
            clickedOnSomething = true;
            clickedPlace = place;
            break;
        }
    }

    // Если кликнули на место или мимо всех элементов
    if (clickedOnSomething) {
        // Снимаем выделение со всех других мест, кроме текущего
        for (auto& place : TOWERS) {
            if (place != clickedPlace && place->isSelect()) {
                place->unselectPlace();
            }
        }
    }
    else {
        // Кликнули мимо всех элементов - снимаем все выделения
        for (auto& place : TOWERS) {
            if (place->isSelect()) {
                place->unselectPlace();
            }
            // Сбрасываем selectFatherTower у всех башен
            if (place->STATE == Place::placeState::tower) {
                for (auto& cell : place->BUY_MENU) {
                    if (cell->STATE == DownCell::selectFatherTower) {
                        cell->STATE = DownCell::unselectFather;
                    }
                }
            }
        }
    }
}

//===========PLACE===============
TowerManager::Place::Place(sf::Vector2f pos, OBJStack* stack)
{
    STACK = stack;
    OBJ.setSize(
        sf::Vector2f(
            JSONSettings["TOWER"]["kaspersky"]["size"][0],
            JSONSettings["TOWER"]["kaspersky"]["size"][1]
        )
    );
    OBJ.setPosition(pos);
    TEXTURE.loadFromFile(JSONSettings["TOWER"]["selectTexture"]);
    OBJ.setTexture(&TEXTURE);
    STATE = empty;
    for (uint8_t i = Tower::defender; i <= Tower::upgradedTower; i++)
        BUY_MENU.push_back(
            new DownCell(this, (Tower::EnumTowerType)i)
        );
    TOWER = nullptr;
    OBJ.setOrigin(OBJ.getSize().x / 2, OBJ.getSize().y / 2);
    SOUND_BUFF_ERROR.loadFromFile(JSONSettings["GAME"]["soundError"]);
    SOUND_BUFF_SPAWN.loadFromFile(JSONSettings["GAME"]["soundTowerSpawn"]);
    SOUND_BUFF_UPGRADE.loadFromFile(JSONSettings["GAME"]["soundUpgrade"]);
    SOUND_BUFF_DELETE.loadFromFile(JSONSettings["GAME"]["soundDelete"]);
}

void TowerManager::Place::setState(placeState state)
{
    STATE = state;
}

bool TowerManager::Place::isEmpty()
{
    return !TOWER;
}

bool TowerManager::Place::isSelect()
{
    return STATE >= placeState::select && STATE != placeState::tower;
}

void TowerManager::Place::addTower(Tower* tower)
{
    TOWER = tower;
    STATE = placeState::tower;
    SOUND.setBuffer(SOUND_BUFF_SPAWN);
    SOUND.play();
}

Tower* TowerManager::Place::getTower()
{
    return TOWER;
}

sf::RectangleShape* TowerManager::Place::getShapePtr()
{
    return &OBJ;
}

void TowerManager::Place::selectPlace()
{
    if (STATE == empty || STATE == tower)
        STATE = (placeState)((uint8_t)STATE + 1);
    for (auto& i : BUY_MENU)
        i->select(1);
}

void TowerManager::Place::unselectPlace()
{
    STATE = (STATE == tower || STATE == selectTower)?(tower):(empty);
    for (auto& i : BUY_MENU)
        i->unselect();
}

void TowerManager::Place::draw(sf::RenderWindow* window)
{
    for (auto& i : BUY_MENU)
        i->draw(window);
    if (STATE < tower) {
        OBJ.setScale(
            sf::Vector2f(1,1)
            * (float)(1.f - abs(((float)(((int)TIME % 80) - 40))/ 300.f))
            );
        window->draw(OBJ);
    }
}

uint8_t TowerManager::Place::getLayer() {
    return 5;
}

sf::Vector2f TowerManager::Place::getSize() {
    return OBJ.getSize();
}

void TowerManager::Place::setSize(sf::Vector2f size)
{
    OBJ.setSize(size);
}

sf::Vector2f TowerManager::Place::getPos(bool isMiddle) {
    return OBJ.getPosition() - (float)(1-isMiddle) * (OBJ.getPosition()/2.f);
}

void TowerManager::Place::setPos(sf::Vector2f vector, bool toMiddle)
{
    OBJ.setPosition(vector + (float)(1 - toMiddle) * (OBJ.getPosition() / 2.f));
}

EnumGameObjects TowerManager::Place::getTypeObjet() {
    return EnumGameObjects::menuWindowObject;
}

IGameObject* TowerManager::Place::getPtr() {
    return this;
}

//===========DOWN_CELL===========

TowerManager::DownCell::DownCell(Place* father, Tower::EnumTowerType number)
{
    FATHER = father;
    NUMBER = number;

    MANAGER.setSize(
        sf::Vector2f(
            JSONSettings["TOWER"]["buyCellSize"][0],
            JSONSettings["TOWER"]["buyCellSize"][1]
        )
    );
    MANAGER.setOrigin(MANAGER.getSize().x / 2, MANAGER.getSize().y / 2);
    if (NUMBER <= Tower::kaspersky) {
        MANAGER.setPosition(
            sf::Vector2f(
                FATHER->getPos().x + (float)(1 - 2 * (NUMBER % 2 == 0)) * MANAGER.getSize().x / 2,
                FATHER->getPos().y + FATHER->getSize().y / 2 + MANAGER.getSize().y / 2 + MANAGER.getSize().y * (float)(NUMBER >= 2)
            )
        );
        TEXTURE_MANAGER.loadFromFile(
            JSONSettings["TOWER"][towerTypes[number]]["cellTexture"]
        );
        MANAGER.setTexture(&TEXTURE_MANAGER);
        DESC.setSize(
            sf::Vector2f(
                JSONSettings["TOWER"]["buyDecsSize"][0],
                JSONSettings["TOWER"]["buyDecsSize"][1]
            )
        );
        //DESC.setOutlineThickness(borderScale);
        //DESC.setOutlineColor(
        //    sf::Color(
        //        12/2,
        //        150/2,
        //        12/2
        //    )
        //);
        DESC.setOrigin(DESC.getSize().x / 2, DESC.getSize().y / 2);
        DESC.setPosition(
            sf::Vector2f(
                FATHER->getPos().x + MANAGER.getSize().x + DESC.getSize().x / 2,
                FATHER->getPos().y + FATHER->getSize().y / 2 + DESC.getSize().y / 2
            )
        );

        DESC.setFillColor(sf::Color(12, 150, 12));

        TEXT_DESC.setFont(FONT);
        TEXT_DESC.setString(
            "BUY\n" 
            + std::to_string((unsigned)JSONSettings["TOWER"][towerTypes[number]]["price"])
        );
        TEXT_DESC.setCharacterSize(24);
        TEXT_DESC.setFillColor(sf::Color::White);

        sf::FloatRect textBounds = TEXT_DESC.getLocalBounds();
        TEXT_DESC.setOrigin(
            textBounds.left + textBounds.width / 2.f,
            textBounds.top + textBounds.height / 2.f
        );
        TEXT_DESC.setPosition(
            DESC.getPosition().x ,
            DESC.getPosition().y 
        );
    }
    else {
        TEXTURE_MANAGER.loadFromFile(
            JSONSettings["TOWER"][(number == Tower::upgradedTower) ? ("upgradeTexture") : ("removeTexture")]
        );
        MANAGER.setTexture(&TEXTURE_MANAGER);
        MANAGER.setPosition(
            sf::Vector2f(
                father->getPos().x + (1 - 2 * (number == Tower::upgradedTower)) * (MANAGER.getSize().x/2 + father->getSize().x/2),
                father->getPos().y
            )
        );

        TEXT_PRICE.setFont(FONT);
        TEXT_PRICE.setCharacterSize(16);
        TEXT_PRICE.setFillColor(sf::Color::White);
        TEXT_PRICE.setString("000");
        sf::FloatRect textBounds = TEXT_PRICE.getLocalBounds();
        TEXT_PRICE.setOrigin(
            textBounds.left + textBounds.width / 2.f,
            textBounds.top + textBounds.height / 2.f
        );

        OBJ_PRICE.setFillColor(sf::Color(80, 160, 80, 180));
        OBJ_PRICE.setSize(MANAGER.getSize() - sf::Vector2f(0, MANAGER.getSize().y)/2.5f);
        OBJ_PRICE.setOrigin(OBJ_PRICE.getSize() / 2.f);
        OBJ_PRICE.setPosition(
            sf::Vector2f(
                MANAGER.getPosition().x,
                MANAGER.getPosition().y + MANAGER.getSize().y / 2.f + OBJ_PRICE.getSize().y / 2.f
            )
        );
        OBJ_PRICE.setOutlineThickness(3.f);
        OBJ_PRICE.setOutlineColor(sf::Color::Black);

        TEXT_PRICE.setPosition(
            OBJ_PRICE.getPosition().x,
            OBJ_PRICE.getPosition().y
        );
    }
    STATE = unselectFather;

}

void TowerManager::DownCell::draw(sf::RenderWindow* window) {
    switch (STATE)
    {
    case TowerManager::DownCell::unselectFather:
        return;
        break;
    case TowerManager::DownCell::selectFather:
        if(NUMBER <= Tower::kaspersky)
            window->draw(MANAGER);
        break;
    case TowerManager::DownCell::EnumCellState::selectBuy:
        if (NUMBER <= Tower::kaspersky) {
            window->draw(MANAGER);
            window->draw(DESC);
            window->draw(TEXT_DESC);
        }
        break;
    case TowerManager::DownCell::existTower:
        return;
        break;
    case TowerManager::DownCell::selectFatherTower:
        if (NUMBER > Tower::kaspersky) {
            window->draw(MANAGER);
            window->draw(OBJ_PRICE);
            window->draw(TEXT_PRICE);
        }
        return;
        break;
    default:
        return;
        break;
    }
}

void TowerManager::DownCell::select(bool isFather)
{
    STATE = (isFather)?(selectFather):(DownCell::EnumCellState::selectBuy);
}

void TowerManager::DownCell::unselect()
{
    STATE = unselectFather;
}

bool TowerManager::DownCell::tryBuy()
{
    unsigned towerPrice = (unsigned)JSONSettings["TOWER"][towerTypes[NUMBER]]["price"];
    if (MONEY >= towerPrice)
    {
        MONEY -= towerPrice;
        STATE = existTower;
        FATHER->addTower(
            new Tower(NUMBER, FATHER->STACK, FATHER->OBJ.getPosition())
        );
        FATHER->STACK->add(FATHER->TOWER);
        return 1;
    }
    return 0;
}

sf::RectangleShape* TowerManager::DownCell::getCellShapePtr()
{
    return &MANAGER;
}

sf::RectangleShape* TowerManager::DownCell::getDescShapePtr()
{
    return &DESC;
}

//=====================================VIDEO_PLAYER============================

bool VideoPlayer::playVideo(const std::wstring& path) {
    stopVideo();
    CoInitialize(nullptr);

    HRESULT hr = CoCreateInstance(CLSID_FilterGraph, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&graph));
    if (FAILED(hr)) return false;

    hr = graph->QueryInterface(IID_PPV_ARGS(&control));
    hr = graph->QueryInterface(IID_PPV_ARGS(&events));

    // пробуем добавить звук
    IBaseFilter* pAudioRenderer = nullptr;
    hr = CoCreateInstance(CLSID_DSoundRender, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pAudioRenderer));
    if (SUCCEEDED(hr)) graph->AddFilter(pAudioRenderer, L"Audio Renderer");

    // Загружаем файл
    hr = graph->RenderFile(path.c_str(), nullptr);
    if (FAILED(hr)) {
        std::wcerr << L"Ошибка загрузки видео: " << path << std::endl;
        stopVideo();
        return false;
    }

    // Настройка видеоокна
    IVideoWindow* videoWindow = nullptr;
    if (SUCCEEDED(graph->QueryInterface(IID_PPV_ARGS(&videoWindow)))) {
        videoWindow->put_WindowStyle(WS_OVERLAPPEDWINDOW | WS_VISIBLE);
        videoWindow->SetWindowPosition(0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)); // на весь экран
        videoWindow->put_FullScreenMode(OATRUE);
        videoWindow->Release();
    }

    // Запуск
    control->Run();
    isPlayingFlag = true;
    return true;
}

VideoPlayer::VideoStatus VideoPlayer::checkVideoStatus() {
    if (!isPlayingFlag) return VIDEO_NOT_STARTED;
    if (!events) return VIDEO_CLOSED;

    long evCode = 0;
    LONG_PTR p1 = 0, p2 = 0;
    if (events->GetEvent(&evCode, &p1, &p2, 0) == S_OK) {
        if (evCode == EC_COMPLETE) {
            stopVideo();
            return VIDEO_FINISHED;
        }
        events->FreeEventParams(evCode, p1, p2);
    }
    return VIDEO_RUNNING;
}

void VideoPlayer::stopVideo() {
    if (control) control->Stop();
    if (events) {
        events->Release();
        events = nullptr;
    }
    if (control) {
        control->Release();
        control = nullptr;
    }
    if (graph) {
        graph->Release();
        graph = nullptr;
    }
    if (isPlayingFlag) {
        CoUninitialize();
        isPlayingFlag = false;
    }
    sendAltTab();
}



//=====================================ОТДЕЛЬНЫЕ ФУНКЦИИ=======================

unsigned getSummArray(unsigned* array, uint8_t length)
{
	unsigned sum = 0;
	for (uint8_t i = 0; i < length; i++) {
		sum += array[i];
	}
	return sum;
}

bool mouseNearPoint(sf::Vector2f point, float distance, sf::RenderWindow* window)
{
	return getDistance((sf::Vector2f)sf::Mouse::getPosition(*window), point) <= distance;
}

bool mouseInButton(sf::RectangleShape* button, sf::RenderWindow* window)
{
	return isPointIntoShape((sf::Vector2f)sf::Mouse::getPosition(*window), *button);
}

bool vatchAD(VideoPlayer* player)
{
    VideoPlayer::VideoStatus videoStatus = player->checkVideoStatus();
    if (videoStatus == VideoPlayer::VIDEO_NOT_STARTED)
    {
        std::string videoFile = JSONSettings["GAME"]["videoAD"];
        std::wstring videoPath = std::wstring(videoFile.begin(), videoFile.end());
        player->playVideo(videoPath);
    }
    if (videoStatus != VideoPlayer::VIDEO_RUNNING && videoStatus != VideoPlayer::VIDEO_NOT_STARTED)
        player->stopVideo();
    return videoStatus == VideoPlayer::VIDEO_RUNNING;
}

void sendAltTab() {
    INPUT inputs[4] = {};

    // ALT down
    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = VK_MENU;

    // TAB down
    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.wVk = VK_TAB;

    // TAB up
    inputs[2].type = INPUT_KEYBOARD;
    inputs[2].ki.wVk = VK_TAB;
    inputs[2].ki.dwFlags = KEYEVENTF_KEYUP;

    // ALT up
    inputs[3].type = INPUT_KEYBOARD;
    inputs[3].ki.wVk = VK_MENU;
    inputs[3].ki.dwFlags = KEYEVENTF_KEYUP;

    SendInput(4, inputs, sizeof(INPUT));
}

void writeScore(OBJStack* stack)
{
    float totalDamage = 0;
    unsigned totalTime = 0;
    for (uint8_t i = 0; i <= LEVEL - 1; i++)
        totalTime += (unsigned)JSONSettings["GAME"]["roundTimeSec"][i];
    totalTime -= TIME / JSONSettings["GENERAL"]["framerate"];

}
