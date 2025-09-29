# Руководство по nlohmann/json для разработки игр

## Оглавление
1. [Подключение библиотеки](#подключение-библиотеки)
2. [Создание JSON объектов](#создание-json-объектов)
3. [Чтение данных из JSON](#чтение-данных-из-json)
4. [Запись данных в JSON](#запись-данных-в-json)
5. [Работа с файлами](#работа-с-файлами)
6. [Сохранение состояния игры](#сохранение-состояния-игры)
7. [Загрузка настроек игры](#загрузка-настроек-игры)
8. [Работа с инвентарем](#работа-с-инвентарем)
9. [Обработка ошибок](#обработка-ошибок)

## Подключение библиотеки

```cpp
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "json.hpp"

using json = nlohmann::json;

// Базовые переменные для примера
std::string player_name = "Hero";
int current_health = 100;
float player_x = 10.5f, player_y = 20.3f, player_z = 5.0f;
std::vector<std::string> player_inventory = {"sword", "potion"};
```

## Создание JSON объектов

```cpp
#include <iostream>
#include <vector>
#include <string>
#include "json.hpp"

using json = nlohmann::json;

int main() {
    // Базовые переменные игры
    std::string player_name = "Warrior";
    int player_level = 5;
    std::vector<float> player_position = {15.5f, 22.1f, 3.0f};
    std::vector<std::string> inventory = {"axe", "health_potion", "key"};
    
    // Создание JSON объекта
    json game_data = {
        {"player", {
            {"name", player_name},
            {"level", player_level},
            {"health", 100},
            {"position", player_position},
            {"inventory", inventory}
        }},
        {"game_state", {
            {"current_level", 3},
            {"score", 12500},
            {"time_played", 3600.5}
        }}
    };
    
    std::cout << "Создан JSON: " << game_data.dump(2) << std::endl;
    return 0;
}
```

## Чтение данных из JSON

```cpp
#include <iostream>
#include <fstream>
#include "json.hpp"

using json = nlohmann::json;

int main() {
    // Пример JSON строки (обычно загружается из файла)
    std::string json_str = R"({
        "player": {
            "name": "Mage",
            "level": 8,
            "health": 75,
            "position": [5.0, 10.5, 2.0],
            "inventory": ["staff", "mana_potion", "scroll"]
        }
    })";
    
    // Парсинг JSON
    json game_data = json::parse(json_str);
    
    // Чтение значений
    std::string name = game_data["player"]["name"];
    int level = game_data["player"]["level"];
    int health = game_data["player"]["health"];
    float pos_x = game_data["player"]["position"][0];
    
    // Безопасное чтение с проверкой
    int coins = 0;
    if (game_data["player"].contains("coins")) {
        coins = game_data["player"]["coins"];
    }
    
    // Чтение с значением по умолчанию
    int mana = game_data["player"].value("mana", 100);
    
    std::cout << "Игрок: " << name << ", Уровень: " << level 
              << ", Здоровье: " << health << std::endl;
    
    return 0;
}
```

## Запись данных в JSON

```cpp
#include <iostream>
#include <vector>
#include "json.hpp"

using json = nlohmann::json;

int main() {
    // Базовые переменные игры
    std::string character_name = "Rogue";
    int character_level = 12;
    float character_health = 88.5f;
    std::vector<std::string> skills = {"stealth", "lockpicking", "archery"};
    
    // Создание и модификация JSON
    json character_data;
    
    // Установка значений
    character_data["name"] = character_name;
    character_data["level"] = character_level;
    character_data["health"] = character_health;
    character_data["skills"] = skills;
    
    // Добавление новых полей
    character_data["experience"] = 12500;
    character_data["alive"] = true;
    
    // Модификация массива
    character_data["skills"].push_back("dagger_mastery");
    
    // Создание вложенного объекта
    character_data["equipment"] = {
        {"weapon", "dagger"},
        {"armor", "leather"},
        {"accessory", "cloak"}
    };
    
    std::cout << "Обновленный JSON: " << character_data.dump(2) << std::endl;
    return 0;
}
```

## Работа с файлами

```cpp
#include <iostream>
#include <fstream>
#include "json.hpp"

using json = nlohmann::json;

int main() {
    // Данные для сохранения
    json save_data = {
        {"player", {
            {"name", "Archer"},
            {"level", 7},
            {"position", {45.2f, 33.1f, 12.5f}}
        }},
        {"world", {
            {"current_zone", "forest"},
            {"time", 15.75f}
        }}
    };
    
    // Сохранение в файл
    std::ofstream save_file("game_save.json");
    if (save_file.is_open()) {
        save_file << save_data.dump(4); // 4 - отступ для читаемости
        save_file.close();
        std::cout << "Игра сохранена!" << std::endl;
    }
    
    // Загрузка из файла
    std::ifstream load_file("game_save.json");
    if (load_file.is_open()) {
        json loaded_data = json::parse(load_file);
        load_file.close();
        
        std::string name = loaded_data["player"]["name"];
        std::string zone = loaded_data["world"]["current_zone"];
        std::cout << "Загружен игрок: " << name << " в зоне: " << zone << std::endl;
    }
    
    return 0;
}
```

## Сохранение состояния игры

```cpp
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include "json.hpp"

using json = nlohmann::json;

// Глобальные переменные игры
std::string player_name = "Knight";
int current_health = 85;
int current_level = 6;
float player_pos[3] = {100.5f, 50.2f, 10.0f};
std::vector<std::string> inventory = {"sword", "shield", "health_potion", "key"};
std::map<std::string, bool> completed_quests = {{"intro", true}, {"goblins", true}, {"dragon", false}};

json create_save_data() {
    json save_data;
    
    // Сохранение данных игрока
    save_data["player"] = {
        {"name", player_name},
        {"health", current_health},
        {"level", current_level},
        {"position", {player_pos[0], player_pos[1], player_pos[2]}},
        {"inventory", inventory}
    };
    
    // Сохранение квестов
    json quests_json;
    for (const auto& quest : completed_quests) {
        quests_json[quest.first] = quest.second;
    }
    save_data["quests"] = quests_json;
    
    // Сохранение игрового мира
    save_data["world_state"] = {
        {"current_time", 14.5},
        {"weather", "sunny"},
        {"difficulty", "normal"}
    };
    
    return save_data;
}

void save_game() {
    json save_data = create_save_data();
    
    std::ofstream file("savegame.json");
    if (file.is_open()) {
        file << save_data.dump(4);
        file.close();
        std::cout << "Игра успешно сохранена!" << std::endl;
    }
}

int main() {
    save_game();
    return 0;
}
```

## Загрузка настроек игры

```cpp
#include <iostream>
#include <fstream>
#include "json.hpp"

using json = nlohmann::json;

// Переменные настроек
int resolution_x = 1920;
int resolution_y = 1080;
float music_volume = 0.8f;
float sfx_volume = 1.0f;
bool fullscreen = false;
std::string language = "english";

void create_default_settings() {
    json default_settings = {
        {"graphics", {
            {"resolution_x", 1920},
            {"resolution_y", 1080},
            {"fullscreen", false},
            {"vsync", true}
        }},
        {"audio", {
            {"music_volume", 0.8},
            {"sfx_volume", 1.0},
            {"master_volume", 1.0}
        }},
        {"gameplay", {
            {"language", "english"},
            {"difficulty", "normal"},
            {"subtitles", true}
        }}
    };
    
    std::ofstream file("settings.json");
    file << default_settings.dump(4);
    file.close();
}

bool load_settings() {
    try {
        std::ifstream file("settings.json");
        if (!file.is_open()) {
            std::cout << "Файл настроек не найден, создаем по умолчанию" << std::endl;
            create_default_settings();
            return false;
        }
        
        json settings = json::parse(file);
        file.close();
        
        // Загрузка графических настроек
        resolution_x = settings["graphics"]["resolution_x"];
        resolution_y = settings["graphics"]["resolution_y"];
        fullscreen = settings["graphics"]["fullscreen"];
        
        // Загрузка аудио настроек
        music_volume = settings["audio"]["music_volume"];
        sfx_volume = settings["audio"]["sfx_volume"];
        
        // Загрузка игровых настроек
        language = settings["gameplay"]["language"];
        
        std::cout << "Настройки загружены успешно!" << std::endl;
        return true;
        
    } catch (const std::exception& e) {
        std::cout << "Ошибка загрузки настроек: " << e.what() << std::endl;
        create_default_settings();
        return false;
    }
}

int main() {
    if (load_settings()) {
        std::cout << "Разрешение: " << resolution_x << "x" << resolution_y << std::endl;
        std::cout << "Громкость музыки: " << music_volume << std::endl;
        std::cout << "Язык: " << language << std::endl;
    }
    return 0;
}
```

## Работа с инвентарем

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include "json.hpp"

using json = nlohmann::json;

// Глобальный JSON с данными игрока
json player_data = {
    {"name", "Adventurer"},
    {"inventory", {"sword", "potion", "bread", "torch"}},
    {"equipped", {
        {"weapon", "sword"},
        {"armor", "leather"}
    }}
};

void add_item(const std::string& item) {
    player_data["inventory"].push_back(item);
    std::cout << "Добавлен предмет: " << item << std::endl;
}

void remove_item(const std::string& item) {
    auto& inventory = player_data["inventory"];
    auto it = std::find(inventory.begin(), inventory.end(), item);
    if (it != inventory.end()) {
        inventory.erase(it);
        std::cout << "Удален предмет: " << item << std::endl;
    } else {
        std::cout << "Предмет не найден: " << item << std::endl;
    }
}

bool has_item(const std::string& item) {
    const auto& inventory = player_data["inventory"];
    return std::find(inventory.begin(), inventory.end(), item) != inventory.end();
}

void equip_item(const std::string& item_type, const std::string& item) {
    if (has_item(item)) {
        player_data["equipped"][item_type] = item;
        std::cout << "Экипирован: " << item << " в слот " << item_type << std::endl;
    } else {
        std::cout << "Нельзя экипировать: " << item << " (нет в инвентаре)" << std::endl;
    }
}

void print_inventory() {
    std::cout << "Инвентарь игрока " << player_data["name"] << ":" << std::endl;
    for (const auto& item : player_data["inventory"]) {
        std::cout << " - " << item << std::endl;
    }
}

int main() {
    print_inventory();
    add_item("gold_key");
    remove_item("bread");
    equip_item("weapon", "axe"); // Не сработает, нет в инвентаре
    equip_item("weapon", "sword");
    print_inventory();
    
    return 0;
}
```

## Обработка ошибок

```cpp
#include <iostream>
#include <fstream>
#include "json.hpp"

using json = nlohmann::json;

void safe_json_operations() {
    // Пример проблемного JSON
    std::string problematic_json = R"({
        "player": {
            "name": "Test",
            "level": "ten"  // Ошибка: строка вместо числа
        }
    })";
    
    try {
        // Парсинг JSON
        json data = json::parse(problematic_json);
        
        // Попытка прочитать число из строки
        int level = data["player"]["level"]; // Выбросит исключение
        
        std::cout << "Уровень: " << level << std::endl;
        
    } catch (const json::parse_error& e) {
        std::cout << "Ошибка парсинга JSON: " << e.what() << std::endl;
    } catch (const json::type_error& e) {
        std::cout << "Ошибка типа данных: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Общая ошибка: " << e.what() << std::endl;
    }
}

void safe_file_operations() {
    try {
        std::ifstream file("nonexistent.json");
        if (!file.is_open()) {
            throw std::runtime_error("Не удалось открыть файл");
        }
        
        json data = json::parse(file);
        file.close();
        
    } catch (const std::exception& e) {
        std::cout << "Ошибка работы с файлом: " << e.what() << std::endl;
    }
}

int main() {
    safe_json_operations();
    safe_file_operations();
    return 0;
}
```