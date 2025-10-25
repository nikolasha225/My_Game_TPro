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

//==============================TOWER_MANAGER==============================







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

bool mouseInButton(sf::RectangleShape button, sf::RenderWindow* window)
{
	return isPointIntoShape((sf::Vector2f)sf::Mouse::getPosition(*window), button);
}

