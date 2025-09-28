# Руководство по SFML 2.6.2 - 2D Графика и работа с объектами

## Оглавление

1. [Базовое окно и игровой цикл](#базовое-окно-и-игровой-цикл)
2. [Стили окон и настройки](#стили-окон-и-настройки)
3. [Система координат и трансформации](#система-координат-и-трансформации)
4. [Текстуры и спрайты](#текстуры-и-спрайты)
5. [Геометрические фигуры](#геометрические-фигуры)
6. [Работа с цветами и прозрачностью](#работа-с-цветами-и-прозрачностью)
7. [Текст и шрифты](#текст-и-шрифты-2d)
8. [Камеры и виды](#камеры-и-виды)
9. [Анимации и движение](#анимации-и-движение)
10. [Коллизии и взаимодействия](#коллизии-и-взаимодействия)
11. [Группировка и управление объектами](#группировка-и-управление-объектами)
12. [Оптимизация отрисовки](#оптимизация-отрисовки)

## Базовое окно и игровой цикл

```cpp
#include <SFML/Graphics.hpp>

int main() {
    // =========================================================================
    // СОЗДАНИЕ ОКНА
    // =========================================================================
    
    // sf::VideoMode(width, height) - создает видеорежим с указанным разрешением
    // Можно также указать битность: sf::VideoMode(800, 600, 32) - 32 бита на пиксель
    sf::VideoMode videoMode(800, 600);
    
    // Проверка валидности видеорежима (поддерживается ли системой)
    if (!videoMode.isValid()) {
        // Если режим не поддерживается, используем режим рабочего стола
        videoMode = sf::VideoMode::getDesktopMode();
    }
    
    // Создание окна
    // Параметры: видеорежим, заголовок, стиль, настройки контекста
    sf::RenderWindow window(videoMode, "SFML 2D Game", sf::Style::Default);
    
    // =========================================================================
    // НАСТРОЙКИ ОКНА
    // =========================================================================
    
    // Установка ограничения частоты кадров (0 = без ограничения)
    window.setFramerateLimit(60); // Ограничить 60 FPS
    
    // Включение вертикальной синхронизации (VSync)
    // Синхронизирует FPS с частотой обновления монитора, убирает разрывы кадров
    // window.setVerticalSyncEnabled(true);
    
    // Позиционирование окна на экране
    // window.setPosition(sf::Vector2i(100, 50)); // Позиция от левого верхнего угла
    
    // Установка иконки окна (файл должен существовать)
    /*
    sf::Image icon;
    if (icon.loadFromFile("assets/icon.png")) {
        window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    }
    */
    
    // =========================================================================
    // ИГРОВОЙ ЦИКЛ
    // =========================================================================
    
    // Создание часов для измерения времени между кадрами
    sf::Clock clock;
    
    // Основной игровой цикл - выполняется пока окно открыто
    while (window.isOpen()) {
        // ======================
        // РАСЧЕТ ВРЕМЕНИ
        // ======================
        
        // restart() возвращает время с последнего вызова и перезапускает часы
        // asSeconds() конвертирует время в секунды (float)
        float deltaTime = clock.restart().asSeconds();
        
        // ======================
        // ОБРАБОТКА СОБЫТИЙ
        // ======================
        
        sf::Event event;
        // pollEvent() получает следующее событие из очереди, если оно есть
        while (window.pollEvent(event)) {
            // Закрытие окна (крестик)
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            
            // Нажатие клавиши
            if (event.type == sf::Event::KeyPressed) {
                // Закрытие по Escape
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
                
                // Переключение полноэкранного режима по F11
                if (event.key.code == sf::Keyboard::F11) {
                    // Реализация будет показана далее
                }
            }
            
            // Изменение размера окна
            if (event.type == sf::Event::Resized) {
                // Обновление вьюпорта под новый размер
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
            }
            
            // Потеря и получение фокуса
            if (event.type == sf::Event::LostFocus) {
                // Игра может быть приостановлена
            }
            if (event.type == sf::Event::GainedFocus) {
                // Возобновление игры
            }
        }
        
        // ======================
        // ОБНОВЛЕНИЕ ИГРЫ
        // ======================
        
        // Здесь обновляется состояние игровых объектов
        // update(deltaTime);
        
        // ======================
        // ОТРИСОВКА
        // ======================
        
        // Очистка экрана цветом (R, G, B) - темно-синий
        window.clear(sf::Color(30, 30, 60));
        
        // --- Отрисовка игровых объектов ---
        // window.draw(sprite);
        // window.draw(shape);
        // window.draw(text);
        
        // Отображение всего нарисованного на экран
        window.display();
    }
    
    return 0;
}
```

## Стили окон и настройки

```cpp
#include <SFML/Graphics.hpp>

void demonstrateWindowStyles() {
    // =========================================================================
    // ДОСТУПНЫЕ СТИЛИ ОКОН
    // =========================================================================
    
    // sf::Style::None - окно без рамки и заголовка
    // Используется для кастомных интерфейсов или splash-экранов
    sf::RenderWindow noneWindow(sf::VideoMode(400, 300), "No Style", sf::Style::None);
    
    // sf::Style::Titlebar - окно только с заголовком и кнопкой закрытия
    // Нельзя изменять размер, нельзя разворачивать
    sf::RenderWindow titlebarWindow(sf::VideoMode(400, 300), "Titlebar Only", sf::Style::Titlebar);
    
    // sf::Style::Resize - окно с рамкой для изменения размера
    // Имеет кнопки свернуть/развернуть/закрыть
    sf::RenderWindow resizeWindow(sf::VideoMode(400, 300), "Resizable", sf::Style::Resize);
    
    // sf::Style::Close - окно только с кнопкой закрытия
    // Без заголовка, но с рамкой
    sf::RenderWindow closeWindow(sf::VideoMode(400, 300), "Close Only", sf::Style::Close);
    
    // sf::Style::Fullscreen - полноэкранный режим
    // Занимает весь экран, обычно с самым высоким поддерживаемым разрешением
    // sf::RenderWindow fullscreenWindow(sf::VideoMode::getDesktopMode(), "Fullscreen", sf::Style::Fullscreen);
    
    // sf::Style::Default - комбинация Titlebar | Resize | Close
    // Стандартное окно Windows/Linux/macOS
    sf::RenderWindow defaultWindow(sf::VideoMode(400, 300), "Default", sf::Style::Default);
    
    // =========================================================================
    // КОМБИНИРОВАНИЕ СТИЛЕЙ
    // =========================================================================
    
    // Стили можно комбинировать через побитовое ИЛИ |
    sf::Uint32 customStyle = sf::Style::Titlebar | sf::Style::Close;
    sf::RenderWindow customWindow(sf::VideoMode(400, 300), "Custom Style", customStyle);
    
    // =========================================================================
    // НАСТРОЙКИ КОНТЕКСТА OPENGL
    // =========================================================================
    
    sf::ContextSettings settings;
    
    // settings.depthBits - количество бит в буфере глубины (для 3D)
    // 0 - отключить буфер глубины (экономит память для 2D игр)
    settings.depthBits = 0; // Для 2D игр можно отключить
    
    // settings.stencilBits - количество бит в трафаретном буфере
    // Используется для сложных масок и эффектов
    settings.stencilBits = 0; // Для простых 2D игр не нужен
    
    // settings.antialiasingLevel - уровень сглаживания (0-16)
    // Убирает "лесенку" на диагональных линиях
    settings.antialiasingLevel = 4; // 4x сглаживание
    
    // settings.majorVersion и settings.minorVersion - версия OpenGL
    settings.majorVersion = 3;
    settings.minorVersion = 0;
    
    // settings.attributeFlags - дополнительные флаги контекста
    settings.attributeFlags = sf::ContextSettings::Default;
    // Доступные флаги:
    // - Core: только core функционал OpenGL (без deprecated функций)
    // - Debug: контекст с отладочной информацией
    
    // Создание окна с кастомными настройками
    sf::RenderWindow advancedWindow(
        sf::VideoMode(800, 600), 
        "Advanced Settings", 
        sf::Style::Default, 
        settings
    );
    
    // =========================================================================
    // ПРОВЕРКА ДОСТУПНЫХ ВИДЕОРЕЖИМОВ
    // =========================================================================
    
    // Получение списка всех поддерживаемых полноэкранных режимов
    std::vector<sf::VideoMode> fullscreenModes = sf::VideoMode::getFullscreenModes();
    
    // Вывод доступных режимов (для отладки)
    for (size_t i = 0; i < fullscreenModes.size(); ++i) {
        const sf::VideoMode& mode = fullscreenModes[i];
        // width x height : bitsPerPixel bits
        // Например: 1920x1080 : 32 bits
    }
    
    // Получение текущего режима рабочего стола
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    
    // Проверка поддержки конкретного режима
    sf::VideoMode testMode(1366, 768, 32);
    if (testMode.isValid()) {
        // Режим поддерживается системой
    }
}

// =============================================================================
// ФУНКЦИЯ ПЕРЕКЛЮЧЕНИЯ ПОЛНОЭКРАННОГО РЕЖИМА
// =============================================================================

void toggleFullscreen(sf::RenderWindow& window, bool& isFullscreen) {
    // Сохраняем текущие настройки
    sf::ContextSettings settings = window.getSettings();
    
    if (isFullscreen) {
        // Переход в оконный режим
        window.create(sf::VideoMode(1024, 768), "My Game", sf::Style::Default, settings);
    } else {
        // Переход в полноэкранный режим
        // Используем текущий режим рабочего стола для лучшей совместимости
        window.create(sf::VideoMode::getDesktopMode(), "My Game", sf::Style::Fullscreen, settings);
    }
    
    isFullscreen = !isFullscreen;
    
    // Центрируем окно в оконном режиме
    if (!isFullscreen) {
        sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
        sf::Vector2u windowSize = window.getSize();
        window.setPosition(sf::Vector2i(
            (desktop.width - windowSize.x) / 2,
            (desktop.height - windowSize.y) / 2
        ));
    }
}
```

## Система координат и трансформации

```cpp
#include <SFML/Graphics.hpp>

void demonstrateTransformations() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Transformations");
    
    // Создание простого спрайта для демонстрации
    sf::Texture texture;
    texture.create(100, 100); // Создаем пустую текстуру 100x100
    
    sf::Sprite sprite(texture);
    sprite.setColor(sf::Color::Red); // Закрашиваем красным для видимости
    
    // =========================================================================
    // СИСТЕМА КООРДИНАТ SFML
    // =========================================================================
    
    // В SFML система координат:
    // - Начало (0,0) в ЛЕВОМ ВЕРХНЕМ углу
    // - Ось X направлена ВПРАВО
    // - Ось Y направлена ВНИЗ
    // Это отличается от математической системы координат!
    
    // =========================================================================
    // ПОЗИЦИОНИРОВАНИЕ
    // =========================================================================
    
    // setPosition(x, y) - абсолютная позиция
    sprite.setPosition(100, 100); // Позиция 100px от левого края, 100px от верха
    
    // move(offsetX, offsetY) - относительное перемещение
    sprite.move(50, 25); // Теперь позиция (150, 125)
    
    // =========================================================================
    // ЦЕНТР ПРЕОБРАЗОВАНИЙ (ORIGIN)
    // =========================================================================
    
    // setOrigin(x, y) устанавливает точку, относительно которой применяются трансформации
    // По умолчанию origin = (0, 0) - левый верхний угол
    
    // Центрирование origin
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2, bounds.height / 2); // Центр спрайта
    
    // Установка origin в разные точки
    // sprite.setOrigin(0, 0);                    // Левый верхний угол (по умолчанию)
    // sprite.setOrigin(bounds.width, 0);         // Правый верхний угол
    // sprite.setOrigin(0, bounds.height);        // Левый нижний угол
    // sprite.setOrigin(bounds.width, bounds.height); // Правый нижний угол
    
    // =========================================================================
    // ВРАЩЕНИЕ
    // =========================================================================
    
    // setRotation(angle) - абсолютный угол в градусах
    sprite.setRotation(45.f); // Поворот на 45 градусов
    
    // rotate(angle) - относительный поворот
    sprite.rotate(10.f); // Дополнительный поворот на 10 градусов (итого 55)
    
    // =========================================================================
    // МАСШТАБИРОВАНИЕ
    // =========================================================================
    
    // setScale(factorX, factorY) - абсолютный масштаб
    sprite.setScale(1.5f, 1.5f); // Увеличение в 1.5 раза
    
    // scale(factorX, factorY) - относительное масштабирование
    sprite.scale(0.5f, 0.5f); // Уменьшение в 2 раза (итого 0.75 от оригинала)
    
    // Неравномерное масштабирование
    // sprite.setScale(2.0f, 1.0f); // Растяжение по X в 2 раза
    
    // =========================================================================
    // ПОЛУЧЕНИЕ ТРАНСФОРМАЦИЙ
    // =========================================================================
    
    sf::Vector2f position = sprite.getPosition();     // Текущая позиция
    float rotation = sprite.getRotation();            // Текущий угол
    sf::Vector2f scale = sprite.getScale();           // Текущий масштаб
    sf::Vector2f origin = sprite.getOrigin();         // Текущий origin
    
    // =========================================================================
    // ГЛОБАЛЬНЫЕ И ЛОКАЛЬНЫЕ ГРАНИЦЫ
    // =========================================================================
    
    // getLocalBounds() - границы в локальной системе координат (до трансформаций)
    sf::FloatRect localBounds = sprite.getLocalBounds();
    // localBounds.left, localBounds.top - всегда (0,0) для спрайтов
    // localBounds.width, localBounds.height - размеры текстуры
    
    // getGlobalBounds() - границы в глобальной системе координат (после трансформаций)
    sf::FloatRect globalBounds = sprite.getGlobalBounds();
    // Учитывает позицию, вращение, масштаб и origin
    
    // =========================================================================
    // ПРЕОБРАЗОВАНИЕ КООРДИНАТ
    // =========================================================================
    
    // Преобразование координат мыши в игровые координаты
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);
    
    // Обратное преобразование
    sf::Vector2i screenPos = window.mapCoordsToPixel(worldPos);
    
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }
        
        // Анимация вращения
        sprite.rotate(1.0f); // Поворот на 1 градус за кадр
        
        window.clear();
        window.draw(sprite);
        window.display();
    }
}
```

## Текстуры и спрайты

```cpp
#include <SFML/Graphics.hpp>
#include <iostream>

void demonstrateTexturesAndSprites() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Textures and Sprites");
    
    // =========================================================================
    // СОЗДАНИЕ И ЗАГРУЗКА ТЕКСТУР
    // =========================================================================
    
    sf::Texture texture1, texture2, texture3;
    
    // Загрузка текстуры из файла
    if (!texture1.loadFromFile("assets/player.png")) {
        std::cerr << "Failed to load player.png" << std::endl;
        // Создаем fallback текстуру
        sf::Image fallback;
        fallback.create(64, 64, sf::Color::Magenta); // Пурпурный - цвет ошибки
        texture1.loadFromImage(fallback);
    }
    
    // Загрузка текстуры из памяти
    /*
    std::vector<sf::Uint8> pixelData = loadPixelsFromNetwork();
    if (!texture2.loadFromMemory(pixelData.data(), pixelData.size())) {
        std::cerr << "Failed to load texture from memory" << std::endl;
    }
    */
    
    // Создание текстуры программно
    sf::Image proceduralImage;
    proceduralImage.create(128, 128, sf::Color::Blue);
    // Рисуем красный круг
    for (unsigned int y = 0; y < 128; ++y) {
        for (unsigned int x = 0; x < 128; ++x) {
            float dx = x - 64.0f;
            float dy = y - 64.0f;
            if (dx*dx + dy*dy <= 40.0f * 40.0f) { // Круг радиусом 40
                proceduralImage.setPixel(x, y, sf::Color::Red);
            }
        }
    }
    texture3.loadFromImage(proceduralImage);
    
    // =========================================================================
    // НАСТРОЙКИ ТЕКСТУР
    // =========================================================================
    
    // setSmooth(true/false) - включение/выключение сглаживания
    texture1.setSmooth(true);  // Для высокодетализированных текстур
    texture2.setSmooth(false); // Для пиксель-арта (сохраняет четкие пиксели)
    
    // setRepeated(true/false) - повторение текстуры
    texture1.setRepeated(true); // Позволяет использовать текстуру как паттерн
    
    // =========================================================================
    // СОЗДАНИЕ СПРАЙТОВ
    // =========================================================================
    
    sf::Sprite sprite1(texture1); // Спрайт с текстурой player.png
    sf::Sprite sprite2(texture3); // Спрайт с программно созданной текстурой
    
    // Настройка спрайтов
    sprite1.setPosition(100, 100);
    sprite2.setPosition(300, 100);
    
    // =========================================================================
    // РАБОТА С ТЕКСТУРНЫМИ КООРДИНАТАМИ
    // =========================================================================
    
    // setTextureRect(rect) - установка области текстуры для отображения
    // sf::IntRect(left, top, width, height)
    sprite1.setTextureRect(sf::IntRect(0, 0, 32, 32)); // Показываем только 32x32 область
    
    // Для анимации можно менять textureRect
    int currentFrame = 0;
    int frameWidth = 32;
    int frameHeight = 32;
    // sprite1.setTextureRect(sf::IntRect(currentFrame * frameWidth, 0, frameWidth, frameHeight));
    
    // =========================================================================
    // ЦВЕТ И ПРОЗРАЧНОСТЬ СПРАЙТОВ
    // =========================================================================
    
    // setColor(color) - установка цвета (умножается на текстуру)
    sprite1.setColor(sf::Color(255, 255, 255, 128)); // Полупрозрачный белый
    
    // Разные цветовые эффекты:
    // sprite1.setColor(sf::Color::Red);      // Красный оттенок
    // sprite1.setColor(sf::Color(255, 255, 255, 100)); // Полупрозрачность
    // sprite1.setColor(sf::Color(128, 128, 128)); // Затемнение
    
    // =========================================================================
    // ПОВТОРЕНИЕ ТЕКСТУР
    // =========================================================================
    
    // Создание большого спрайта с повторяющейся текстурой
    sf::Sprite backgroundSprite(texture1); // texture1 должна быть setRepeated(true)
    backgroundSprite.setTextureRect(sf::IntRect(0, 0, 800, 600)); // Растягиваем на все окно
    
    // =========================================================================
    // ОБНОВЛЕНИЕ ТЕКСТУР В РЕАЛЬНОМ ВРЕМЕНИ
    // =========================================================================
    
    // Создание динамической текстуры
    sf::Texture dynamicTexture;
    dynamicTexture.create(64, 64);
    
    sf::Sprite dynamicSprite(dynamicTexture);
    dynamicSprite.setPosition(500, 100);
    
    // Обновление текстуры каждый кадр
    sf::Image dynamicImage;
    dynamicImage.create(64, 64);
    
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }
        
        // Обновление динамической текстуры
        for (unsigned int y = 0; y < 64; ++y) {
            for (unsigned int x = 0; x < 64; ++x) {
                // Создаем анимированный шум
                sf::Uint8 noise = rand() % 256;
                dynamicImage.setPixel(x, y, sf::Color(noise, noise, noise));
            }
        }
        dynamicTexture.update(dynamicImage);
        
        window.clear();
        window.draw(backgroundSprite); // Фон
        window.draw(sprite1);          // Основной спрайт
        window.draw(sprite2);          // Программный спрайт
        window.draw(dynamicSprite);    // Динамический спрайт
        window.display();
    }
}

// =============================================================================
// КЛАСС ДЛЯ РАБОТЫ СО SPRITESHEET (ЛИСТ СПРАЙТОВ)
// =============================================================================

class SpriteSheet {
private:
    sf::Texture texture;
    sf::Vector2i frameSize;
    int frameCount;
    int currentFrame;
    float animationSpeed;
    sf::Clock animationClock;
    
public:
    SpriteSheet() : currentFrame(0), animationSpeed(0.1f) {}
    
    bool loadFromFile(const std::string& filename, const sf::Vector2i& frameSize, int frameCount) {
        if (!texture.loadFromFile(filename)) {
            return false;
        }
        this->frameSize = frameSize;
        this->frameCount = frameCount;
        return true;
    }
    
    void setAnimationSpeed(float speed) {
        animationSpeed = speed;
    }
    
    void update() {
        // Смена кадра по времени
        if (animationClock.getElapsedTime().asSeconds() >= animationSpeed) {
            currentFrame = (currentFrame + 1) % frameCount;
            animationClock.restart();
        }
    }
    
    void applyToSprite(sf::Sprite& sprite) {
        int frameX = (currentFrame * frameSize.x) % texture.getSize().x;
        int frameY = ((currentFrame * frameSize.x) / texture.getSize().x) * frameSize.y;
        
        sprite.setTexture(texture);
        sprite.setTextureRect(sf::IntRect(frameX, frameY, frameSize.x, frameSize.y));
    }
};
```
## Геометрические фигуры

```cpp
#include <SFML/Graphics.hpp>

void demonstrateShapes() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "2D Shapes");
    
    // =========================================================================
    // ПРЯМОУГОЛЬНИК (RectangleShape)
    // =========================================================================
    
    sf::RectangleShape rectangle(sf::Vector2f(150, 100)); // width, height
    
    // Основные настройки прямоугольника
    rectangle.setPosition(100, 100);
    rectangle.setFillColor(sf::Color(255, 100, 100));     // Цвет заливки
    rectangle.setOutlineColor(sf::Color::White);          // Цвет контура
    rectangle.setOutlineThickness(3.0f);                  // Толщина контура
    
    // Скругленные углы
    rectangle.setSize(sf::Vector2f(120, 80));
    rectangle.setOutlineThickness(2.0f);
    
    // =========================================================================
    // КРУГ (CircleShape)
    // =========================================================================
    
    sf::CircleShape circle(50.0f); // Радиус 50 пикселей
    
    circle.setPosition(300, 100);
    circle.setFillColor(sf::Color(100, 255, 100));
    circle.setOutlineColor(sf::Color::White);
    circle.setOutlineThickness(2.0f);
    
    // Количество сторон (качество аппроксимации круга)
    circle.setPointCount(32); // По умолчанию 30, больше = более гладкий круг
    
    // Можно создавать многоугольники, меняя количество сторон:
    // circle.setPointCount(3);  // Треугольник
    // circle.setPointCount(4);  // Квадрат
    // circle.setPointCount(5);  // Пятиугольник
    // circle.setPointCount(6);  // Шестиугольник
    // circle.setPointCount(8);  // Восьмиугольник
    
    // =========================================================================
    // МНОГОУГОЛЬНИК (ConvexShape)
    // =========================================================================
    
    sf::ConvexShape polygon;
    
    // Установка количества точек
    polygon.setPointCount(5); // Пятиугольник
    
    // Установка координат каждой точки (относительно центра)
    polygon.setPoint(0, sf::Vector2f(0, -50));    // Верхняя точка
    polygon.setPoint(1, sf::Vector2f(40, -20));   // Правая верхняя
    polygon.setPoint(2, sf::Vector2f(25, 40));    // Правая нижняя
    polygon.setPoint(3, sf::Vector2f(-25, 40));   // Левая нижняя
    polygon.setPoint(4, sf::Vector2f(-40, -20));  // Левая верхняя
    
    polygon.setPosition(500, 150);
    polygon.setFillColor(sf::Color(100, 100, 255));
    polygon.setOutlineColor(sf::Color::White);
    polygon.setOutlineThickness(2.0f);
    
    // =========================================================================
    // ЛИНИЯ (используя RectangleShape)
    // =========================================================================
    
    sf::RectangleShape line(sf::Vector2f(200, 3)); // Длина 200, толщина 3
    
    line.setPosition(100, 300);
    line.setFillColor(sf::Color::Yellow);
    line.setRotation(30.0f); // Поворот линии на 30 градусов
    
    // =========================================================================
    // ТРЕУГОЛЬНИК (используя ConvexShape)
    // =========================================================================
    
    sf::ConvexShape triangle;
    triangle.setPointCount(3);
    triangle.setPoint(0, sf::Vector2f(0, -40));   // Верхняя точка
    triangle.setPoint(1, sf::Vector2f(35, 40));   // Правая нижняя
    triangle.setPoint(2, sf::Vector2f(-35, 40));  // Левая нижняя
    
    triangle.setPosition(400, 300);
    triangle.setFillColor(sf::Color(255, 200, 100));
    
    // =========================================================================
    // ЗВЕЗДА (используя ConvexShape)
    // =========================================================================
    
    sf::ConvexShape star;
    star.setPointCount(10); // 5 внешних + 5 внутренних точек
    
    // Внешние точки звезды
    for (int i = 0; i < 5; ++i) {
        float angle = i * 72.0f * 3.14159f / 180.0f; // 72 градуса между лучами
        star.setPoint(i, sf::Vector2f(50 * cos(angle), 50 * sin(angle)));
    }
    
    // Внутренние точки звезды
    for (int i = 0; i < 5; ++i) {
        float angle = (i * 72.0f + 36.0f) * 3.14159f / 180.0f; // Смещение на 36 градусов
        star.setPoint(i + 5, sf::Vector2f(20 * cos(angle), 20 * sin(angle)));
    }
    
    star.setPosition(600, 300);
    star.setFillColor(sf::Color(255, 255, 100));
    
    // =========================================================================
    // ГРАДИЕНТ (создание через текстуру)
    // =========================================================================
    
    sf::Texture gradientTexture;
    sf::Image gradientImage;
    gradientImage.create(256, 256);
    
    // Создание градиента от синего к красному
    for (unsigned int y = 0; y < 256; ++y) {
        for (unsigned int x = 0; x < 256; ++x) {
            sf::Uint8 red = x;    // Красный увеличивается слева направо
            sf::Uint8 green = 0;
            sf::Uint8 blue = 255 - x; // Синий уменьшается слева направо
            gradientImage.setPixel(x, y, sf::Color(red, green, blue));
        }
    }
    gradientTexture.loadFromImage(gradientImage);
    
    sf::Sprite gradientSprite(gradientTexture);
    gradientSprite.setPosition(100, 400);
    gradientSprite.setScale(2.0f, 1.0f); // Растягиваем по горизонтали
    
    // =========================================================================
    // ТРАНСФОРМАЦИИ ФИГУР
    // =========================================================================
    
    // Все фигуры поддерживают те же трансформации, что и спрайты:
    rectangle.setRotation(45.0f);     // Поворот на 45 градусов
    circle.setScale(1.5f, 0.8f);      // Неравномерное масштабирование
    
    // Установка origin (центра преобразований)
    sf::FloatRect rectBounds = rectangle.getLocalBounds();
    rectangle.setOrigin(rectBounds.width / 2, rectBounds.height / 2);
    
    // Анимация
    sf::Clock animationClock;
    
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }
        
        // Анимация вращения
        float time = animationClock.getElapsedTime().asSeconds();
        rectangle.setRotation(time * 45.0f); // 45 градусов в секунду
        circle.setRotation(-time * 30.0f);   // -30 градусов в секунду
        
        // Пульсация масштаба
        float pulse = sin(time * 3.0f) * 0.2f + 1.0f;
        polygon.setScale(pulse, pulse);
        
        // Движение по синусоиде
        triangle.setPosition(400, 300 + sin(time * 2.0f) * 50.0f);
        
        window.clear(sf::Color(40, 40, 60));
        
        // Отрисовка всех фигур
        window.draw(rectangle);
        window.draw(circle);
        window.draw(polygon);
        window.draw(line);
        window.draw(triangle);
        window.draw(star);
        window.draw(gradientSprite);
        
        window.display();
    }
}

// =============================================================================
// КЛАСС ДЛЯ СОЗДАНИЯ КНОПКИ
// =============================================================================

class Button {
private:
    sf::RectangleShape shape;
    sf::Text text;
    sf::Color normalColor;
    sf::Color hoverColor;
    sf::Color pressedColor;
    bool isHovered;
    bool isPressed;
    
public:
    Button(const sf::Vector2f& size, const sf::Font& font, const std::string& buttonText) {
        // Настройка формы кнопки
        shape.setSize(size);
        shape.setFillColor(sf::Color(70, 70, 200));
        shape.setOutlineColor(sf::Color::White);
        shape.setOutlineThickness(2.0f);
        
        // Настройка текста
        text.setFont(font);
        text.setString(buttonText);
        text.setCharacterSize(20);
        text.setFillColor(sf::Color::White);
        
        // Центрирование текста
        sf::FloatRect textBounds = text.getLocalBounds();
        text.setOrigin(textBounds.width / 2, textBounds.height / 2);
        
        // Цвета состояний
        normalColor = sf::Color(70, 70, 200);
        hoverColor = sf::Color(100, 100, 230);
        pressedColor = sf::Color(50, 50, 180);
        
        isHovered = false;
        isPressed = false;
    }
    
    void setPosition(const sf::Vector2f& position) {
        shape.setPosition(position);
        
        // Центрирование текста относительно кнопки
        sf::FloatRect bounds = shape.getGlobalBounds();
        text.setPosition(bounds.left + bounds.width / 2, bounds.top + bounds.height / 2);
    }
    
    void update(const sf::Vector2f& mousePos) {
        sf::FloatRect bounds = shape.getGlobalBounds();
        
        // Проверка наведения мыши
        isHovered = bounds.contains(mousePos);
        
        // Изменение цвета в зависимости от состояния
        if (isPressed) {
            shape.setFillColor(pressedColor);
        } else if (isHovered) {
            shape.setFillColor(hoverColor);
        } else {
            shape.setFillColor(normalColor);
        }
    }
    
    bool isMouseOver() const { return isHovered; }
    
    void setPressed(bool pressed) { isPressed = pressed; }
    
    void draw(sf::RenderWindow& window) {
        window.draw(shape);
        window.draw(text);
    }
    
    sf::FloatRect getGlobalBounds() const {
        return shape.getGlobalBounds();
    }
};
```

## Работа с цветами и прозрачностью

```cpp
#include <SFML/Graphics.hpp>

void demonstrateColorsAndTransparency() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Colors and Transparency");
    
    // =========================================================================
    // СОЗДАНИЕ ЦВЕТОВ
    // =========================================================================
    
    // Предопределенные цвета
    sf::Color red = sf::Color::Red;
    sf::Color green = sf::Color::Green;
    sf::Color blue = sf::Color::Blue;
    sf::Color white = sf::Color::White;
    sf::Color black = sf::Color::Black;
    sf::Color transparent = sf::Color::Transparent; // Полностью прозрачный
    
    // Создание цветов из компонентов (R, G, B, A)
    sf::Color customColor1(255, 128, 0);        // Оранжевый (A=255 по умолчанию)
    sf::Color customColor2(100, 200, 150, 128); // Зеленоватый с полупрозрачностью
    
    // Создание цветов из HEX значений
    sf::Color hexColor1(0xFF6A00FF); // Оранжевый
    sf::Color hexColor2(0x6496C880); // Полупрозрачный синий
    
    // =========================================================================
    // ПРОЗРАЧНОСТЬ (ALPHA CHANNEL)
    // =========================================================================
    
    // Alpha = 255 - полностью непрозрачный
    // Alpha = 0 - полностью прозрачный
    // Alpha = 128 - 50% прозрачность
    
    sf::RectangleShape opaqueRect(sf::Vector2f(150, 150));
    opaqueRect.setPosition(50, 50);
    opaqueRect.setFillColor(sf::Color(255, 0, 0, 255)); // Красный непрозрачный
    
    sf::RectangleShape semiTransparentRect(sf::Vector2f(150, 150));
    semiTransparentRect.setPosition(100, 100); // Частично перекрывает первый
    semiTransparentRect.setFillColor(sf::Color(0, 255, 0, 128)); // Зеленый 50% прозрачность
    
    sf::RectangleShape transparentRect(sf::Vector2f(150, 150));
    transparentRect.setPosition(150, 150);
    transparentRect.setFillColor(sf::Color(0, 0, 255, 64)); // Синий 25% прозрачность
    
    // =========================================================================
    // СМЕШИВАНИЕ ЦВЕТОВ
    // =========================================================================
    
    // Функция для линейной интерполяции цветов
    auto lerpColor = [](const sf::Color& a, const sf::Color& b, float t) -> sf::Color {
        return sf::Color(
            a.r + (b.r - a.r) * t,
            a.g + (b.g - a.g) * t,
            a.b + (b.b - a.b) * t,
            a.a + (b.a - a.a) * t
        );
    };
    
    // Создание градиента
    std::vector<sf::RectangleShape> gradientBars;
    int barCount = 10;
    sf::Color startColor(255, 0, 0);   // Красный
    sf::Color endColor(0, 0, 255);     // Синий
    
    for (int i = 0; i < barCount; ++i) {
        sf::RectangleShape bar(sf::Vector2f(40, 200));
        bar.setPosition(300 + i * 45, 50);
        
        float t = static_cast<float>(i) / (barCount - 1);
        bar.setFillColor(lerpColor(startColor, endColor, t));
        
        gradientBars.push_back(bar);
    }
    
    // =========================================================================
    // ЦВЕТОВЫЕ ЭФФЕКТЫ НА СПРАЙТАХ
    // =========================================================================
    
    sf::Texture texture;
    texture.create(100, 100); // Создаем тестовую текстуру
    
    // Обычный спрайт
    sf::Sprite normalSprite(texture);
    normalSprite.setPosition(50, 300);
    normalSprite.setColor(sf::Color::White); // Без изменений
    
    // Затемненный спрайт
    sf::Sprite darkenedSprite(texture);
    darkenedSprite.setPosition(200, 300);
    darkenedSprite.setColor(sf::Color(128, 128, 128)); // 50% яркости
    
    // Оттененный спрайт
    sf::Sprite tintedSprite(texture);
    tintedSprite.setPosition(350, 300);
    tintedSprite.setColor(sf::Color(255, 200, 200)); // Красный оттенок
    
    // Полупрозрачный спрайт
    sf::Sprite transparentSprite(texture);
    transparentSprite.setPosition(500, 300);
    transparentSprite.setColor(sf::Color(255, 255, 255, 128)); // 50% прозрачность
    
    // =========================================================================
    // BLEND MODES (РЕЖИМЫ СМЕШИВАНИЯ)
    // =========================================================================
    
    // SFML автоматически использует Alpha blending для прозрачности
    // Но можно настроить вручную через states:
    
    sf::RenderStates states;
    
    // Доступные blend modes:
    // sf::BlendAlpha    - стандартное альфа-смешивание (по умолчанию)
    // sf::BlendAdd      - добавление цветов (для световых эффектов)
    // sf::BlendMultiply - умножение цветов (для теней)
    // sf::BlendNone     - без смешивания (замена пикселей)
    
    states.blendMode = sf::BlendAdd;
    
    // =========================================================================
    // ДЕМОНСТРАЦИЯ РАЗНЫХ BLEND MODES
    // =========================================================================
    
    // Фон для демонстрации blend modes
    sf::RectangleShape blendBackground(sf::Vector2f(200, 200));
    blendBackground.setPosition(550, 50);
    blendBackground.setFillColor(sf::Color(50, 50, 100));
    
    // Объекты для смешивания
    sf::CircleShape blendCircle1(60);
    blendCircle1.setPosition(570, 70);
    blendCircle1.setFillColor(sf::Color(255, 100, 100, 150)); // Красный полупрозрачный
    
    sf::CircleShape blendCircle2(60);
    blendCircle2.setPosition(630, 130);
    blendCircle2.setFillColor(sf::Color(100, 100, 255, 150)); // Синий полупрозрачный
    
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }
        
        // Анимация прозрачности
        float time = std::sin(animationClock.getElapsedTime().asSeconds() * 2.0f) * 0.5f + 0.5f;
        sf::Uint8 alpha = static_cast<sf::Uint8>(time * 255);
        semiTransparentRect.setFillColor(sf::Color(0, 255, 0, alpha));
        
        window.clear(sf::Color(30, 30, 30));
        
        // Отрисовка прозрачных прямоугольников
        window.draw(opaqueRect);
        window.draw(semiTransparentRect);
        window.draw(transparentRect);
        
        // Отрисовка градиента
        for (auto& bar : gradientBars) {
            window.draw(bar);
        }
        
        // Отрисовка спрайтов с разными цветовыми эффектами
        window.draw(normalSprite);
        window.draw(darkenedSprite);
        window.draw(tintedSprite);
        window.draw(transparentSprite);
        
        // Демонстрация blend modes
        window.draw(blendBackground);
        window.draw(blendCircle1);
        window.draw(blendCircle2);
        
        window.display();
    }
}

// =============================================================================
// КЛАСС ДЛЯ ЦВЕТОВЫХ АНИМАЦИЙ
// =============================================================================

class ColorAnimator {
private:
    sf::Clock clock;
    std::vector<sf::Color> colors;
    float duration;
    bool loop;
    
public:
    ColorAnimator(const std::vector<sf::Color>& colorSequence, float animDuration, bool shouldLoop = true)
        : colors(colorSequence), duration(animDuration), loop(shouldLoop) {}
    
    sf::Color getCurrentColor() {
        if (colors.empty()) return sf::Color::White;
        if (colors.size() == 1) return colors[0];
        
        float time = clock.getElapsedTime().asSeconds();
        
        if (!loop && time >= duration) {
            return colors.back();
        }
        
        float normalizedTime = std::fmod(time, duration) / duration;
        float segmentDuration = 1.0f / (colors.size() - 1);
        int segment = static_cast<int>(normalizedTime / segmentDuration);
        float segmentTime = (normalizedTime - segment * segmentDuration) / segmentDuration;
        
        segment = std::min(segment, static_cast<int>(colors.size() - 2));
        
        return sf::Color(
            colors[segment].r + (colors[segment + 1].r - colors[segment].r) * segmentTime,
            colors[segment].g + (colors[segment + 1].g - colors[segment].g) * segmentTime,
            colors[segment].b + (colors[segment + 1].b - colors[segment].b) * segmentTime,
            colors[segment].a + (colors[segment + 1].a - colors[segment].a) * segmentTime
        );
    }
    
    void restart() {
        clock.restart();
    }
};
```
## Текст и шрифты 2D

```cpp
#include <SFML/Graphics.hpp>
#include <sstream>
#include <iomanip>

void demonstrateTextAndFonts() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Text and Fonts");
    
    // =========================================================================
    // ЗАГРУЗКА ШРИФТОВ
    // =========================================================================
    
    sf::Font arialFont, customFont;
    
    // Попытка загрузки шрифта Arial (обычно есть в системе)
    if (!arialFont.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        // Альтернативные пути для разных ОС
        if (!arialFont.loadFromFile("/usr/share/fonts/truetype/freefont/FreeSans.ttf")) {
            std::cerr << "Failed to load Arial font" << std::endl;
            // Можно создать базовый шрифт программно или использовать другой
        }
    }
    
    // Загрузка кастомного шрифта из папки assets
    if (!customFont.loadFromFile("assets/fonts/game_font.ttf")) {
        std::cerr << "Failed to load custom font" << std::endl;
        // Используем Arial как fallback
        customFont = arialFont;
    }
    
    // =========================================================================
    // СОЗДАНИЕ ТЕКСТОВЫХ ОБЪЕКТОВ
    // =========================================================================
    
    // Основной текст
    sf::Text mainText;
    mainText.setFont(arialFont);                  // Установка шрифта
    mainText.setString("Hello SFML!");           // Текст для отображения
    mainText.setCharacterSize(36);               // Размер в пикселях
    mainText.setFillColor(sf::Color::White);     // Цвет текста
    mainText.setPosition(50, 50);                // Позиция
    
    // Текст с контуром
    sf::Text outlinedText;
    outlinedText.setFont(customFont);
    outlinedText.setString("Outlined Text");
    outlinedText.setCharacterSize(48);
    outlinedText.setFillColor(sf::Color::Yellow);       // Цвет заливки
    outlinedText.setOutlineColor(sf::Color::Black);     // Цвет контура
    outlinedText.setOutlineThickness(2.0f);             // Толщина контура
    outlinedText.setPosition(50, 120);
    
    // =========================================================================
    // СТИЛИ ТЕКСТА
    // =========================================================================
    
    sf::Text styledText;
    styledText.setFont(arialFont);
    styledText.setString("Styled Text - Bold & Underlined");
    styledText.setCharacterSize(24);
    styledText.setFillColor(sf::Color::Green);
    
    // Комбинирование стилей через побитовое ИЛИ
    styledText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    styledText.setPosition(50, 200);
    
    // Доступные стили:
    // sf::Text::Regular    - обычный текст (по умолчанию)
    // sf::Text::Bold       - жирный текст
    // sf::Text::Italic     - курсив
    // sf::Text::Underlined - подчеркнутый текст
    // sf::Text::StrikeThrough - зачеркнутый текст
    
    // =========================================================================
    // ВЫРАВНИВАНИЕ ТЕКСТА
    // =========================================================================
    
    sf::Text centeredText;
    centeredText.setFont(arialFont);
    centeredText.setString("Centered Text");
    centeredText.setCharacterSize(32);
    centeredText.setFillColor(sf::Color::Cyan);
    
    // Центрирование текста
    sf::FloatRect textBounds = centeredText.getLocalBounds();
    centeredText.setOrigin(textBounds.width / 2, textBounds.height / 2);
    centeredText.setPosition(400, 280);
    
    // Текст выровненный по правому краю
    sf::Text rightAlignedText;
    rightAlignedText.setFont(arialFont);
    rightAlignedText.setString("Right Aligned");
    rightAlignedText.setCharacterSize(24);
    rightAlignedText.setFillColor(sf::Color::Magenta);
    
    textBounds = rightAlignedText.getLocalBounds();
    rightAlignedText.setOrigin(textBounds.width, 0); // Выравнивание по правому краю
    rightAlignedText.setPosition(750, 320);
    
    // =========================================================================
    // РАБОТА С МНОГОСТРОЧНЫМ ТЕКСТОМ
    // =========================================================================
    
    class MultiLineText {
    private:
        std::vector<sf::Text> lines;
        sf::Font& font;
        float lineHeight;
        
    public:
        MultiLineText(sf::Font& textFont, float lineSpacing = 1.5f) 
            : font(textFont), lineHeight(lineSpacing) {}
        
        void setString(const std::string& text, unsigned int characterSize, const sf::Color& color) {
            lines.clear();
            
            std::istringstream iss(text);
            std::string line;
            float y = 0;
            
            while (std::getline(iss, line)) {
                sf::Text textLine;
                textLine.setFont(font);
                textLine.setString(line);
                textLine.setCharacterSize(characterSize);
                textLine.setFillColor(color);
                textLine.setPosition(0, y);
                
                lines.push_back(textLine);
                y += characterSize * lineHeight;
            }
        }
        
        void setPosition(float x, float y) {
            for (auto& line : lines) {
                line.setPosition(x, y);
                y += line.getCharacterSize() * lineHeight;
            }
        }
        
        void draw(sf::RenderWindow& window) {
            for (auto& line : lines) {
                window.draw(line);
            }
        }
    };
    
    MultiLineText multiText(arialFont);
    multiText.setString("First line\nSecond line\nThird line with more text", 20, sf::Color::White);
    multiText.setPosition(50, 350);
    
    // =========================================================================
    // ТЕКСТ С ПЕРЕМЕННЫМИ ДАННЫМИ (FPS СЧЕТЧИК)
    // =========================================================================
    
    sf::Text fpsText;
    fpsText.setFont(arialFont);
    fpsText.setCharacterSize(18);
    fpsText.setFillColor(sf::Color::Green);
    fpsText.setPosition(10, 10);
    
    sf::Clock fpsClock;
    int frameCount = 0;
    float fpsTime = 0;
    
    // =========================================================================
    // ЭФФЕКТЫ ТЕКСТА (ПУЛЬСАЦИЯ, МЕРЦАНИЕ)
    // =========================================================================
    
    sf::Text animatedText;
    animatedText.setFont(customFont);
    animatedText.setString("Animated Text!");
    animatedText.setCharacterSize(40);
    animatedText.setFillColor(sf::Color::Red);
    
    // Центрирование анимированного текста
    textBounds = animatedText.getLocalBounds();
    animatedText.setOrigin(textBounds.width / 2, textBounds.height / 2);
    animatedText.setPosition(400, 450);
    
    // =========================================================================
    // ИЗМЕРЕНИЕ ТЕКСТА И РАБОТА С ГЛИФАМИ
    // =========================================================================
    
    // Получение информации о шрифте
    unsigned int fontSize = 24;
    int lineSpacing = arialFont.getLineSpacing(fontSize);
    float underlinePosition = arialFont.getUnderlinePosition(fontSize);
    float underlineThickness = arialFont.getUnderlineThickness(fontSize);
    
    // Получение глифа (визуального представления символа)
    sf::Uint32 character = 'A';
    const sf::Glyph& glyph = arialFont.getGlyph(character, fontSize, false);
    
    // glyph содержит:
    // - bounds: границы глифа
    // - textureRect: область в текстуре шрифта
    // - advance: расстояние до следующего символа
    
    // Кернинг (расстояние между конкретными символами)
    float avKerning = arialFont.getKerning('A', 'V', fontSize);
    float waKerning = arialFont.getKerning('W', 'A', fontSize);
    
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }
        
        // ======================
        // ОБНОВЛЕНИЕ FPS СЧЕТЧИКА
        // ======================
        
        frameCount++;
        float deltaTime = fpsClock.restart().asSeconds();
        fpsTime += deltaTime;
        
        if (fpsTime >= 0.1f) { // Обновляем FPS каждые 0.1 секунды
            float fps = frameCount / fpsTime;
            frameCount = 0;
            fpsTime = 0;
            
            std::stringstream ss;
            ss << "FPS: " << std::fixed << std::setprecision(1) << fps;
            fpsText.setString(ss.str());
        }
        
        // ======================
        // АНИМАЦИЯ ТЕКСТА
        // ======================
        
        float time = animationClock.getElapsedTime().asSeconds();
        
        // Пульсация масштаба
        float scale = 1.0f + sin(time * 3.0f) * 0.2f;
        animatedText.setScale(scale, scale);
        
        // Изменение цвета
        sf::Uint8 red = static_cast<sf::Uint8>(sin(time * 2.0f) * 127 + 128);
        sf::Uint8 green = static_cast<sf::Uint8>(cos(time * 1.5f) * 127 + 128);
        animatedText.setFillColor(sf::Color(red, green, 255));
        
        // Вращение
        animatedText.setRotation(sin(time) * 10.0f);
        
        window.clear(sf::Color(30, 30, 50));
        
        // Отрисовка всех текстовых элементов
        window.draw(fpsText);
        window.draw(mainText);
        window.draw(outlinedText);
        window.draw(styledText);
        window.draw(centeredText);
        window.draw(rightAlignedText);
        multiText.draw(window);
        window.draw(animatedText);
        
        window.display();
    }
}

// =============================================================================
// КЛАСС ДЛЯ ТИПОГРАФИЧЕСКИХ ЭФФЕКТОВ
// =============================================================================

class TypographyEffects {
private:
    sf::Text baseText;
    std::vector<sf::Text> shadowLayers;
    float shadowOffset;
    sf::Color shadowColor;
    
public:
    TypographyEffects(const sf::Font& font, const std::string& text, unsigned int characterSize) {
        baseText.setFont(font);
        baseText.setString(text);
        baseText.setCharacterSize(characterSize);
        baseText.setFillColor(sf::Color::White);
        
        shadowOffset = 2.0f;
        shadowColor = sf::Color(0, 0, 0, 128);
        
        createShadowEffect();
    }
    
    void createShadowEffect(int layers = 3) {
        shadowLayers.clear();
        
        for (int i = layers; i > 0; --i) {
            sf::Text shadow = baseText;
            float offset = shadowOffset * i;
            shadow.setPosition(offset, offset);
            shadow.setFillColor(sf::Color(
                shadowColor.r,
                shadowColor.g,
                shadowColor.b,
                shadowColor.a / i
            ));
            shadowLayers.push_back(shadow);
        }
    }
    
    void setPosition(float x, float y) {
        baseText.setPosition(x, y);
        
        for (size_t i = 0; i < shadowLayers.size(); ++i) {
            float offset = shadowOffset * (i + 1);
            shadowLayers[i].setPosition(x + offset, y + offset);
        }
    }
    
    void setString(const std::string& text) {
        baseText.setString(text);
        createShadowEffect();
    }
    
    void draw(sf::RenderWindow& window) {
        // Сначала рисуем тени (от самой дальней к ближайшей)
        for (auto it = shadowLayers.rbegin(); it != shadowLayers.rend(); ++it) {
            window.draw(*it);
        }
        // Затем основной текст
        window.draw(baseText);
    }
    
    void setShadowColor(const sf::Color& color) {
        shadowColor = color;
        createShadowEffect();
    }
    
    void setShadowOffset(float offset) {
        shadowOffset = offset;
        createShadowEffect();
    }
};

// =============================================================================
// КЛАСС ДЛЯ БЕГУЩЕЙ СТРОКИ (MARQUEE)
// =============================================================================

class MarqueeText {
private:
    sf::Text text;
    float speed;
    float position;
    float containerWidth;
    bool loop;
    
public:
    MarqueeText(const sf::Font& font, const std::string& content, 
                unsigned int characterSize, float containerWidth, bool shouldLoop = true)
        : containerWidth(containerWidth), loop(shouldLoop) {
        
        text.setFont(font);
        text.setString(content);
        text.setCharacterSize(characterSize);
        text.setFillColor(sf::Color::White);
        
        speed = 50.0f; // пикселей в секунду
        position = containerWidth; // Начинаем за правым краем
    }
    
    void update(float deltaTime) {
        position -= speed * deltaTime;
        
        float textWidth = text.getLocalBounds().width;
        
        if (position + textWidth < 0) {
            if (loop) {
                position = containerWidth;
            } else {
                position = -textWidth;
            }
        }
    }
    
    void setPosition(float y) {
        text.setPosition(position, y);
    }
    
    void draw(sf::RenderWindow& window) {
        window.draw(text);
    }
    
    void setSpeed(float newSpeed) {
        speed = newSpeed;
    }
    
    void setColor(const sf::Color& color) {
        text.setFillColor(color);
    }
};
```

## Камеры и виды

```cpp
#include <SFML/Graphics.hpp>

void demonstrateCamerasAndViews() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Cameras and Views");
    
    // =========================================================================
    // СОЗДАНИЕ ВИДОВ (VIEWS)
    // =========================================================================
    
    // Вид по умолчанию (соответствует размерам окна)
    sf::View defaultView = window.getDefaultView();
    
    // Создание кастомного вида
    // sf::View(center, size) - центр и размер области обзора
    sf::View gameView(sf::Vector2f(400, 300), sf::Vector2f(800, 600));
    
    // Создание вида из прямоугольника
    // sf::FloatRect(left, top, width, height)
    sf::View fixedView(sf::FloatRect(0, 0, 800, 600));
    
    // =========================================================================
    // НАСТРОЙКИ ВИДА
    // =========================================================================
    
    // Центр камеры (за кем следует камера)
    gameView.setCenter(400, 300);
    
    // Размер области обзора (zoom level)
    gameView.setSize(800, 600); // 1x zoom
    
    // Поворот вида (в градусах)
    gameView.setRotation(0.0f);
    
    // Область вьюпорта (нормализованные координаты 0-1)
    // sf::FloatRect(left, top, width, height)
    gameView.setViewport(sf::FloatRect(0.0f, 0.0f, 1.0f, 1.0f)); // Весь экран
    
    // =========================================================================
    // СОЗДАНИЕ ТЕСТОВОЙ СЦЕНЫ
    // =========================================================================
    
    std::vector<sf::CircleShape> objects;
    sf::Color colors[] = {
        sf::Color::Red, sf::Color::Green, sf::Color::Blue,
        sf::Color::Yellow, sf::Color::Magenta, sf::Color::Cyan
    };
    
    // Создание случайных объектов на сцене
    for (int i = 0; i < 20; ++i) {
        sf::CircleShape obj(20.0f);
        obj.setPosition(rand() % 1600, rand() % 1200); // Большая сцена
        obj.setFillColor(colors[rand() % 6]);
        obj.setOutlineColor(sf::Color::White);
        obj.setOutlineThickness(2.0f);
        objects.push_back(obj);
    }
    
    // Игрок (объект за которым следует камера)
    sf::CircleShape player(30.0f);
    player.setFillColor(sf::Color::White);
    player.setOutlineColor(sf::Color::Black);
    player.setOutlineThickness(3.0f);
    player.setPosition(400, 300);
    
    // =========================================================================
    // УПРАВЛЕНИЕ КАМЕРОЙ
    // =========================================================================
    
    float cameraSpeed = 300.0f; // пикселей в секунду
    float zoomSpeed = 1.0f;     // коэффициент масштабирования в секунду
    float currentZoom = 1.0f;
    float minZoom = 0.1f;
    float maxZoom = 3.0f;
    
    // =========================================================================
    // РАЗДЕЛЕННЫЙ ЭКРАН (SPLIT-SCREEN)
    // =========================================================================
    
    sf::View leftView(sf::Vector2f(200, 300), sf::Vector2f(400, 600));
    leftView.setViewport(sf::FloatRect(0.0f, 0.0f, 0.5f, 1.0f)); // Левая половина
    
    sf::View rightView(sf::Vector2f(600, 300), sf::Vector2f(400, 600));
    rightView.setViewport(sf::FloatRect(0.5f, 0.0f, 0.5f, 1.0f)); // Правая половина
    
    // =========================================================================
    // MINIMAP ВИД
    // =========================================================================
    
    sf::View minimapView(sf::Vector2f(800, 600), sf::Vector2f(1600, 1200));
    minimapView.setViewport(sf::FloatRect(0.75f, 0.0f, 0.25f, 0.25f)); // Правый верхний угол
    
    // =========================================================================
    // ИНТЕРФЕЙС ПОЛЬЗОВАТЕЛЯ (UI) ВИД
    // =========================================================================
    
    sf::View uiView = window.getDefaultView(); // Не зависит от игровой камеры
    
    // Элементы UI
    sf::RectangleShape healthBar(sf::Vector2f(200, 20));
    healthBar.setFillColor(sf::Color::Red);
    healthBar.setPosition(20, 20);
    
    sf::Text uiText;
    sf::Font font;
    // font.loadFromFile(...);
    uiText.setFont(font);
    uiText.setString("Health: 100%");
    uiText.setCharacterSize(18);
    uiText.setPosition(20, 45);
    
    bool useSplitScreen = false;
    bool showMinimap = true;
    
    sf::Clock clock;
    
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            
            // Обработка изменения размера окна
            if (event.type == sf::Event::Resized) {
                // Обновление вьюпорта основного вида
                gameView.setSize(event.size.width, event.size.height);
                gameView.setCenter(event.size.width / 2, event.size.height / 2);
                
                // Обновление UI вида
                uiView.setSize(event.size.width, event.size.height);
                uiView.setCenter(event.size.width / 2, event.size.height / 2);
            }
            
            // Обработка прокрутки колесика для зума
            if (event.type == sf::Event::MouseWheelScrolled) {
                if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
                    float zoomFactor = (event.mouseWheelScroll.delta > 0) ? 0.9f : 1.1f;
                    currentZoom *= zoomFactor;
                    currentZoom = std::max(minZoom, std::min(maxZoom, currentZoom));
                    gameView.setSize(800 * currentZoom, 600 * currentZoom);
                }
            }
            
            // Переключение режимов отображения
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::F1) {
                    useSplitScreen = !useSplitScreen;
                }
                if (event.key.code == sf::Keyboard::F2) {
                    showMinimap = !showMinimap;
                }
            }
        }
        
        // ======================
        // УПРАВЛЕНИЕ ИГРОКОМ
        // ======================
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            player.move(0, -cameraSpeed * deltaTime);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            player.move(0, cameraSpeed * deltaTime);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            player.move(-cameraSpeed * deltaTime, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            player.move(cameraSpeed * deltaTime, 0);
        }
        
        // Ограничение игрока в пределах сцены
        sf::Vector2f pos = player.getPosition();
        pos.x = std::max(0.0f, std::min(1540.0f, pos.x));
        pos.y = std::max(0.0f, std::min(1140.0f, pos.y));
        player.setPosition(pos);
        
        // ======================
        // ОБНОВЛЕНИЕ КАМЕРЫ
        // ======================
        
        // Камера следует за игроком с небольшим отставанием (smooth follow)
        sf::Vector2f targetPos = player.getPosition() + sf::Vector2f(15, 15);
        sf::Vector2f currentCenter = gameView.getCenter();
        sf::Vector2f newCenter = currentCenter + (targetPos - currentCenter) * 5.0f * deltaTime;
        gameView.setCenter(newCenter);
        
        // Обновление split-screen камер
        leftView.setCenter(newCenter.x - 200, newCenter.y);
        rightView.setCenter(newCenter.x + 200, newCenter.y);
        
        // ======================
        // ОТРИСОВКА
        // ======================
        
        window.clear(sf::Color(50, 50, 80));
        
        if (useSplitScreen) {
            // Режим разделенного экрана
            window.setView(leftView);
            for (const auto& obj : objects) window.draw(obj);
            window.draw(player);
            
            window.setView(rightView);
            for (const auto& obj : objects) window.draw(obj);
            window.draw(player);
        } else {
            // Обычный режим
            window.setView(gameView);
            for (const auto& obj : objects) window.draw(obj);
            window.draw(player);
        }
        
        // Minimap
        if (showMinimap) {
            window.setView(minimapView);
            for (const auto& obj : objects) {
                sf::CircleShape minimapObj(5.0f);
                minimapObj.setPosition(obj.getPosition());
                minimapObj.setFillColor(obj.getFillColor());
                window.draw(minimapObj);
            }
            
            sf::CircleShape minimapPlayer(8.0f);
            minimapPlayer.setPosition(player.getPosition());
            minimapPlayer.setFillColor(sf::Color::White);
            window.draw(minimapPlayer);
        }
        
        // UI элементы (всегда поверх всего)
        window.setView(uiView);
        window.draw(healthBar);
        window.draw(uiText);
        
        // Отображение информации о режимах
        sf::Text modeText;
        modeText.setFont(font);
        modeText.setCharacterSize(14);
        modeText.setFillColor(sf::Color::White);
        modeText.setPosition(20, 550);
        
        std::string modeInfo = "F1: Split-screen: " + std::string(useSplitScreen ? "ON" : "OFF") +
                              "\nF2: Minimap: " + std::string(showMinimap ? "ON" : "OFF") +
                              "\nMouse Wheel: Zoom";
        modeText.setString(modeInfo);
        window.draw(modeText);
        
        window.display();
    }
}

// =============================================================================
// КЛАСС КАМЕРЫ С РАСШИРЕННЫМИ ВОЗМОЖНОСТЯМИ
// =============================================================================

class GameCamera {
private:
    sf::View view;
    sf::Vector2f target;
    float followSpeed;
    float shakeIntensity;
    float shakeDuration;
    sf::Clock shakeClock;
    bool isShaking;
    
public:
    GameCamera(const sf::Vector2f& center, const sf::Vector2f& size) 
        : view(center, size), followSpeed(5.0f), shakeIntensity(0.0f), 
          shakeDuration(0.0f), isShaking(false) {}
    
    void update(float deltaTime) {
        sf::Vector2f currentCenter = view.getCenter();
        
        if (isShaking) {
            // Эффект тряски камеры
            float shakeTime = shakeClock.getElapsedTime().asSeconds();
            if (shakeTime < shakeDuration) {
                float progress = shakeTime / shakeDuration;
                float currentIntensity = shakeIntensity * (1.0f - progress);
                
                float offsetX = (rand() % 100 - 50) / 50.0f * currentIntensity;
                float offsetY = (rand() % 100 - 50) / 50.0f * currentIntensity;
                
                view.setCenter(target + sf::Vector2f(offsetX, offsetY));
            } else {
                isShaking = false;
                view.setCenter(target);
            }
        } else {
            // Плавное следование за целью
            sf::Vector2f newCenter = currentCenter + (target - currentCenter) * followSpeed * deltaTime;
            view.setCenter(newCenter);
        }
    }
    
    void setTarget(const sf::Vector2f& newTarget) {
        target = newTarget;
    }
    
    void setFollowSpeed(float speed) {
        followSpeed = speed;
    }
    
    void shake(float intensity, float duration) {
        shakeIntensity = intensity;
        shakeDuration = duration;
        isShaking = true;
        shakeClock.restart();
    }
    
    void zoom(float factor) {
        view.zoom(factor);
    }
    
    void setZoom(float level) {
        sf::Vector2f size = view.getSize();
        view.setSize(size / level);
    }
    
    void applyToWindow(sf::RenderWindow& window) {
        window.setView(view);
    }
    
    sf::Vector2f getWorldMousePos(const sf::RenderWindow& window) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        return window.mapPixelToCoords(mousePos, view);
    }
    
    bool isPointInView(const sf::Vector2f& point) {
        sf::FloatRect viewBounds(
            view.getCenter().x - view.getSize().x / 2,
            view.getCenter().y - view.getSize().y / 2,
            view.getSize().x,
            view.getSize().y
        );
        return viewBounds.contains(point);
    }
};
```
## Анимации и движение

```cpp
#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>

void demonstrateAnimationsAndMovement() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Animations and Movement");
    
    // =========================================================================
    // DELTA TIME - ОСНОВА ПЛАВНОГО ДВИЖЕНИЯ
    // =========================================================================
    
    sf::Clock gameClock;
    float deltaTime = 0.0f;
    
    // =========================================================================
    // ЛИНЕЙНОЕ ДВИЖЕНИЕ
    // =========================================================================
    
    sf::CircleShape linearObject(20.0f);
    linearObject.setFillColor(sf::Color::Red);
    linearObject.setPosition(100, 100);
    
    float linearSpeed = 100.0f; // пикселей в секунду
    sf::Vector2f linearVelocity(1.0f, 0.5f); // направление движения
    // Нормализация вектора скорости (опционально, для постоянной скорости)
    // float length = std::sqrt(linearVelocity.x * linearVelocity.x + linearVelocity.y * linearVelocity.y);
    // if (length > 0) linearVelocity /= length;
    
    // =========================================================================
    // СИНУСОИДАЛЬНОЕ ДВИЖЕНИЕ
    // =========================================================================
    
    sf::CircleShape sineObject(15.0f);
    sineObject.setFillColor(sf::Color::Green);
    float sineStartX = 200;
    float sineAmplitude = 50.0f;  // амплитуда колебаний
    float sineFrequency = 2.0f;   // частота колебаний
    
    // =========================================================================
    // КРУГОВОЕ ДВИЖЕНИЕ
    // =========================================================================
    
    sf::CircleShape circularObject(12.0f);
    circularObject.setFillColor(sf::Color::Blue);
    sf::Vector2f circleCenter(400, 200);
    float circleRadius = 80.0f;
    float circleSpeed = 1.5f; // радиан в секунду
    float circleAngle = 0.0f;
    
    // =========================================================================
    // ДВИЖЕНИЕ ПО КРИВОЙ БЕЗЬЕ
    // =========================================================================
    
    class BezierMover {
    private:
        sf::CircleShape object;
        std::vector<sf::Vector2f> controlPoints;
        float progress;
        float speed;
        
    public:
        BezierMover(float radius, const sf::Color& color, float moveSpeed = 0.3f) 
            : progress(0.0f), speed(moveSpeed) {
            object.setRadius(radius);
            object.setFillColor(color);
            object.setOrigin(radius, radius);
        }
        
        void setControlPoints(const std::vector<sf::Vector2f>& points) {
            controlPoints = points;
            if (!controlPoints.empty()) {
                object.setPosition(controlPoints[0]);
            }
        }
        
        void update(float deltaTime) {
            if (controlPoints.size() < 2) return;
            
            progress += speed * deltaTime;
            if (progress > 1.0f) progress = 0.0f;
            
            // Кривая Безье второго порядка (3 точки)
            if (controlPoints.size() == 3) {
                float t = progress;
                float u = 1 - t;
                
                sf::Vector2f position = 
                    controlPoints[0] * (u * u) +
                    controlPoints[1] * (2 * u * t) +
                    controlPoints[2] * (t * t);
                
                object.setPosition(position);
            }
        }
        
        void draw(sf::RenderWindow& window) {
            window.draw(object);
        }
        
        // Визуализация контрольных точек и кривой
        void drawDebug(sf::RenderWindow& window) {
            // Рисуем контрольные точки
            for (const auto& point : controlPoints) {
                sf::CircleShape cp(3.0f);
                cp.setFillColor(sf::Color::Yellow);
                cp.setOrigin(3.0f, 3.0f);
                cp.setPosition(point);
                window.draw(cp);
            }
            
            // Рисуем кривую Безье
            if (controlPoints.size() == 3) {
                sf::VertexArray curve(sf::LineStrip, 50);
                for (int i = 0; i < 50; ++i) {
                    float t = i / 49.0f;
                    float u = 1 - t;
                    
                    sf::Vector2f point = 
                        controlPoints[0] * (u * u) +
                        controlPoints[1] * (2 * u * t) +
                        controlPoints[2] * (t * t);
                    
                    curve[i].position = point;
                    curve[i].color = sf::Color(255, 255, 255, 100);
                }
                window.draw(curve);
            }
        }
    };
    
    BezierMover bezierObject(10.0f, sf::Color::Cyan, 0.5f);
    bezierObject.setControlPoints({
        sf::Vector2f(500, 100),
        sf::Vector2f(600, 300),
        sf::Vector2f(500, 400)
    });
    
    // =========================================================================
    // SPRITESHEET АНИМАЦИЯ
    // =========================================================================
    
    class SpriteSheetAnimation {
    private:
        sf::Sprite sprite;
        sf::Vector2i frameSize;
        int totalFrames;
        int currentFrame;
        float frameDuration;
        float currentTime;
        bool looping;
        
    public:
        SpriteSheetAnimation(const sf::Texture& texture, const sf::Vector2i& frameSize, 
                           int frameCount, float fps = 10.0f, bool loop = true)
            : frameSize(frameSize), totalFrames(frameCount), currentFrame(0),
              frameDuration(1.0f / fps), currentTime(0.0f), looping(loop) {
            sprite.setTexture(texture);
            updateFrame();
        }
        
        void update(float deltaTime) {
            currentTime += deltaTime;
            
            if (currentTime >= frameDuration) {
                currentTime = 0.0f;
                currentFrame++;
                
                if (currentFrame >= totalFrames) {
                    if (looping) {
                        currentFrame = 0;
                    } else {
                        currentFrame = totalFrames - 1;
                    }
                }
                
                updateFrame();
            }
        }
        
        void updateFrame() {
            int columns = sprite.getTexture()->getSize().x / frameSize.x;
            int row = currentFrame / columns;
            int column = currentFrame % columns;
            
            sprite.setTextureRect(sf::IntRect(
                column * frameSize.x,
                row * frameSize.y,
                frameSize.x,
                frameSize.y
            ));
        }
        
        void setPosition(const sf::Vector2f& position) {
            sprite.setPosition(position);
        }
        
        void draw(sf::RenderWindow& window) {
            window.draw(sprite);
        }
        
        void setLooping(bool loop) {
            looping = loop;
        }
        
        bool isFinished() const {
            return !looping && currentFrame == totalFrames - 1;
        }
    };
    
    // Создание тестовой текстуры для анимации
    sf::Texture animTexture;
    animTexture.create(128, 128); // Замените на реальный spritesheet
    
    SpriteSheetAnimation spriteAnimation(animTexture, sf::Vector2i(32, 32), 16, 12.0f);
    spriteAnimation.setPosition(300, 400);
    
    // =========================================================================
    // TWEENING АНИМАЦИИ
    // =========================================================================
    
    class Tween {
    private:
        float startValue;
        float endValue;
        float duration;
        float elapsed;
        bool finished;
        
    public:
        Tween(float start, float end, float time) 
            : startValue(start), endValue(end), duration(time), elapsed(0.0f), finished(false) {}
        
        float update(float deltaTime) {
            if (finished) return endValue;
            
            elapsed += deltaTime;
            float t = elapsed / duration;
            
            if (t >= 1.0f) {
                finished = true;
                return endValue;
            }
            
            // Линейная интерполяция
            return startValue + (endValue - startValue) * t;
            
            // Для других easing функций можно использовать:
            // return startValue + (endValue - startValue) * easeInOutQuad(t);
        }
        
        bool isFinished() const {
            return finished;
        }
        
        void restart() {
            elapsed = 0.0f;
            finished = false;
        }
        
    private:
        // Пример easing функции
        float easeInOutQuad(float t) {
            return t < 0.5f ? 2 * t * t : 1 - std::pow(-2 * t + 2, 2) / 2;
        }
    };
    
    sf::CircleShape tweenObject(25.0f);
    tweenObject.setFillColor(sf::Color::Magenta);
    Tween positionTween(100.0f, 700.0f, 3.0f);
    
    // =========================================================================
    // ФИЗИКА (ПРОСТАЯ)
    // =========================================================================
    
    class PhysicsObject {
    private:
        sf::CircleShape object;
        sf::Vector2f velocity;
        sf::Vector2f acceleration;
        float damping;
        
    public:
        PhysicsObject(float radius, const sf::Color& color) 
            : velocity(0, 0), acceleration(0, 0), damping(0.98f) {
            object.setRadius(radius);
            object.setFillColor(color);
            object.setOrigin(radius, radius);
        }
        
        void update(float deltaTime) {
            // Обновление скорости с учетом ускорения
            velocity += acceleration * deltaTime;
            
            // Обновление позиции
            object.move(velocity * deltaTime);
            
            // Затухание скорости
            velocity *= damping;
            
            // Сброс ускорения для следующего кадра
            acceleration = sf::Vector2f(0, 0);
        }
        
        void applyForce(const sf::Vector2f& force) {
            acceleration += force;
        }
        
        void setPosition(const sf::Vector2f& position) {
            object.setPosition(position);
        }
        
        sf::Vector2f getPosition() const {
            return object.getPosition();
        }
        
        void draw(sf::RenderWindow& window) {
            window.draw(object);
        }
        
        void bounceOffWalls(const sf::FloatRect& bounds) {
            sf::FloatRect objBounds = object.getGlobalBounds();
            
            if (objBounds.left < bounds.left || objBounds.left + objBounds.width > bounds.left + bounds.width) {
                velocity.x = -velocity.x * 0.8f; // Отскок с потерей энергии
            }
            if (objBounds.top < bounds.top || objBounds.top + objBounds.height > bounds.top + bounds.height) {
                velocity.y = -velocity.y * 0.8f;
            }
            
            // Корректировка позиции, чтобы не выходить за границы
            sf::Vector2f pos = object.getPosition();
            pos.x = std::max(bounds.left + object.getRadius(), std::min(bounds.left + bounds.width - object.getRadius(), pos.x));
            pos.y = std::max(bounds.top + object.getRadius(), std::min(bounds.top + bounds.height - object.getRadius(), pos.y));
            object.setPosition(pos);
        }
    };
    
    PhysicsObject physicsBall(20.0f, sf::Color::Yellow);
    physicsBall.setPosition(600, 100);
    physicsBall.applyForce(sf::Vector2f(50.0f, 100.0f));
    
    sf::FloatRect worldBounds(0, 0, 800, 600);
    
    while (window.isOpen()) {
        deltaTime = gameClock.restart().asSeconds();
        
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            
            // Применение силы при клике мыши
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
                    sf::Vector2f direction = mousePos - physicsBall.getPosition();
                    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
                    if (distance > 0) {
                        direction /= distance; // Нормализация
                        physicsBall.applyForce(direction * 500.0f);
                    }
                }
            }
        }
        
        // ======================
        // ОБНОВЛЕНИЕ ОБЪЕКТОВ
        // ======================
        
        // Линейное движение
        linearObject.move(linearVelocity * linearSpeed * deltaTime);
        
        // Проверка границ для линейного объекта
        sf::FloatRect linearBounds = linearObject.getGlobalBounds();
        if (linearBounds.left < 0 || linearBounds.left + linearBounds.width > 800) {
            linearVelocity.x = -linearVelocity.x;
        }
        if (linearBounds.top < 0 || linearBounds.top + linearBounds.height > 600) {
            linearVelocity.y = -linearVelocity.y;
        }
        
        // Синусоидальное движение
        float sineTime = gameClock.getElapsedTime().asSeconds();
        float sineY = 200 + std::sin(sineTime * sineFrequency) * sineAmplitude;
        sineObject.setPosition(sineStartX + sineTime * 50, sineY);
        
        // Круговое движение
        circleAngle += circleSpeed * deltaTime;
        float circularX = circleCenter.x + std::cos(circleAngle) * circleRadius;
        float circularY = circleCenter.y + std::sin(circleAngle) * circleRadius;
        circularObject.setPosition(circularX, circularY);
        
        // Движение по Безье
        bezierObject.update(deltaTime);
        
        // Spritesheet анимация
        spriteAnimation.update(deltaTime);
        
        // Tweening анимация
        float tweenX = positionTween.update(deltaTime);
        tweenObject.setPosition(tweenX, 500);
        if (positionTween.isFinished()) {
            positionTween = Tween(700.0f, 100.0f, 3.0f);
        }
        
        // Физика
        physicsBall.update(deltaTime);
        physicsBall.bounceOffWalls(worldBounds);
        
        // ======================
        // ОТРИСОВКА
        // ======================
        
        window.clear(sf::Color(20, 20, 40));
        
        // Отрисовка объектов
        window.draw(linearObject);
        window.draw(sineObject);
        window.draw(circularObject);
        bezierObject.drawDebug(window);
        bezierObject.draw(window);
        spriteAnimation.draw(window);
        window.draw(tweenObject);
        physicsBall.draw(window);
        
        // Отладочная информация
        sf::Font font;
        // font.loadFromFile(...);
        sf::Text infoText;
        infoText.setFont(font);
        infoText.setCharacterSize(14);
        infoText.setFillColor(sf::Color::White);
        infoText.setPosition(10, 10);
        
        std::string info = "Delta Time: " + std::to_string(deltaTime) + 
                          "\nFPS: " + std::to_string(1.0f / deltaTime) +
                          "\nClick to apply force to yellow ball";
        infoText.setString(info);
        window.draw(infoText);
        
        window.display();
    }
}

// =============================================================================
// КЛАСС ДЛЯ УПРАВЛЕНИЯ АНИМАЦИЯМИ
// =============================================================================

class AnimationManager {
private:
    struct Animation {
        std::function<void(float)> updateFunction;
        std::function<void(sf::RenderWindow&)> drawFunction;
        float duration;
        float elapsed;
        bool finished;
    };
    
    std::vector<Animation> animations;
    
public:
    void addAnimation(const std::function<void(float)>& update, 
                     const std::function<void(sf::RenderWindow&)>& draw,
                     float duration) {
        animations.push_back({update, draw, duration, 0.0f, false});
    }
    
    void update(float deltaTime) {
        for (auto& anim : animations) {
            if (!anim.finished) {
                anim.elapsed += deltaTime;
                anim.updateFunction(deltaTime);
                
                if (anim.elapsed >= anim.duration) {
                    anim.finished = true;
                }
            }
        }
        
        // Удаление завершенных анимаций
        animations.erase(
            std::remove_if(animations.begin(), animations.end(),
                [](const Animation& anim) { return anim.finished; }),
            animations.end()
        );
    }
    
    void draw(sf::RenderWindow& window) {
        for (auto& anim : animations) {
            anim.drawFunction(window);
        }
    }
    
    bool isEmpty() const {
        return animations.empty();
    }
    
    void clear() {
        animations.clear();
    }
};

// =============================================================================
// КЛАСС ДЛЯ PARTICLE SYSTEM
// =============================================================================

class ParticleSystem {
private:
    struct Particle {
        sf::Vector2f position;
        sf::Vector2f velocity;
        sf::Color color;
        float lifetime;
        float maxLifetime;
        float size;
    };
    
    std::vector<Particle> particles;
    sf::Vector2f emitterPosition;
    float emissionRate;
    float timeSinceLastEmission;
    
public:
    ParticleSystem(const sf::Vector2f& position, float rate = 10.0f) 
        : emitterPosition(position), emissionRate(rate), timeSinceLastEmission(0.0f) {}
    
    void update(float deltaTime) {
        timeSinceLastEmission += deltaTime;
        
        // Генерация новых частиц
        if (timeSinceLastEmission >= 1.0f / emissionRate) {
            emitParticle();
            timeSinceLastEmission = 0.0f;
        }
        
        // Обновление существующих частиц
        for (auto it = particles.begin(); it != particles.end(); ) {
            it->lifetime -= deltaTime;
            it->position += it->velocity * deltaTime;
            it->velocity.y += 98.0f * deltaTime; // гравитация
            
            if (it->lifetime <= 0.0f) {
                it = particles.erase(it);
            } else {
                ++it;
            }
        }
    }
    
    void emitParticle() {
        Particle p;
        p.position = emitterPosition;
        p.velocity = sf::Vector2f(
            (rand() % 100 - 50) / 10.0f,
            -(rand() % 100) / 10.0f
        );
        p.color = sf::Color(
            rand() % 255,
            rand() % 255,
            rand() % 255
        );
        p.maxLifetime = 1.0f + (rand() % 100) / 100.0f;
        p.lifetime = p.maxLifetime;
        p.size = 2.0f + (rand() % 8);
        
        particles.push_back(p);
    }
    
    void draw(sf::RenderWindow& window) {
        for (const auto& particle : particles) {
            float alpha = particle.lifetime / particle.maxLifetime * 255;
            sf::Color renderColor = particle.color;
            renderColor.a = static_cast<sf::Uint8>(alpha);
            
            sf::CircleShape particleShape(particle.size);
            particleShape.setFillColor(renderColor);
            particleShape.setOrigin(particle.size, particle.size);
            particleShape.setPosition(particle.position);
            
            window.draw(particleShape);
        }
    }
    
    void setEmitterPosition(const sf::Vector2f& position) {
        emitterPosition = position;
    }
    
    void setEmissionRate(float rate) {
        emissionRate = rate;
    }
};
```

## Коллизии и взаимодействия

```cpp
#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>

void demonstrateCollisionsAndInteractions() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Collisions and Interactions");
    
    // =========================================================================
    // ОСНОВНЫЕ ТИПЫ КОЛЛИЗИЙ
    // =========================================================================
    
    // AABB (Axis-Aligned Bounding Box) - прямоугольник, выровненный по осям
    // Circle - круговые коллизии
    // Point - проверка точки внутри объекта
    
    // =========================================================================
    // AABB КОЛЛИЗИИ
    // =========================================================================
    
    class AABBCollider {
    private:
        sf::FloatRect bounds;
        
    public:
        AABBCollider(const sf::FloatRect& rect) : bounds(rect) {}
        
        bool intersects(const AABBCollider& other) const {
            return bounds.intersects(other.bounds);
        }
        
        bool contains(const sf::Vector2f& point) const {
            return bounds.contains(point);
        }
        
        sf::FloatRect getBounds() const { return bounds; }
        void setBounds(const sf::FloatRect& newBounds) { bounds = newBounds; }
        void setPosition(const sf::Vector2f& position) { 
            bounds.left = position.x; 
            bounds.top = position.y; 
        }
    };
    
    // =========================================================================
    // CIRCLE КОЛЛИЗИИ
    // =========================================================================
    
    class CircleCollider {
    private:
        sf::Vector2f center;
        float radius;
        
    public:
        CircleCollider(const sf::Vector2f& center, float radius) 
            : center(center), radius(radius) {}
        
        bool intersects(const CircleCollider& other) const {
            float dx = center.x - other.center.x;
            float dy = center.y - other.center.y;
            float distance = std::sqrt(dx * dx + dy * dy);
            return distance <= (radius + other.radius);
        }
        
        bool intersects(const AABBCollider& box) const {
            // Ближайшая точка AABB к кругу
            float closestX = std::max(box.getBounds().left, 
                                     std::min(center.x, box.getBounds().left + box.getBounds().width));
            float closestY = std::max(box.getBounds().top, 
                                     std::min(center.y, box.getBounds().top + box.getBounds().height));
            
            // Расстояние от центра круга до ближайшей точки
            float dx = center.x - closestX;
            float dy = center.y - closestY;
            float distance = std::sqrt(dx * dx + dy * dy);
            
            return distance <= radius;
        }
        
        bool contains(const sf::Vector2f& point) const {
            float dx = point.x - center.x;
            float dy = point.y - center.y;
            return (dx * dx + dy * dy) <= (radius * radius);
        }
        
        sf::Vector2f getCenter() const { return center; }
        float getRadius() const { return radius; }
        void setCenter(const sf::Vector2f& newCenter) { center = newCenter; }
    };
    
    // =========================================================================
    // ИГРОВЫЕ ОБЪЕКТЫ С КОЛЛИЗИЯМИ
    // =========================================================================
    
    class GameObject {
    protected:
        sf::Shape* shape;
        AABBCollider collider;
        sf::Vector2f velocity;
        bool isStatic;
        
    public:
        GameObject(sf::Shape* shape, bool staticObj = false) 
            : shape(shape), isStatic(staticObj) {
            updateCollider();
        }
        
        virtual void update(float deltaTime) {
            if (!isStatic) {
                shape->move(velocity * deltaTime);
                updateCollider();
            }
        }
        
        void updateCollider() {
            collider.setBounds(shape->getGlobalBounds());
        }
        
        void draw(sf::RenderWindow& window) {
            window.draw(*shape);
        }
        
        AABBCollider& getCollider() { return collider; }
        sf::Vector2f getPosition() const { return shape->getPosition(); }
        void setPosition(const sf::Vector2f& position) { 
            shape->setPosition(position); 
            updateCollider();
        }
        void setVelocity(const sf::Vector2f& newVelocity) { velocity = newVelocity; }
        bool getIsStatic() const { return isStatic; }
        
        virtual void onCollision(GameObject* other) {
            // Можно переопределить в дочерних классах
            shape->setFillColor(sf::Color::Red);
        }
    };
    
    // =========================================================================
    // МЕНЕДЖЕР КОЛЛИЗИЙ
    // =========================================================================
    
    class CollisionManager {
    private:
        std::vector<GameObject*> objects;
        
    public:
        void addObject(GameObject* obj) {
            objects.push_back(obj);
        }
        
        void checkCollisions() {
            for (size_t i = 0; i < objects.size(); ++i) {
                for (size_t j = i + 1; j < objects.size(); ++j) {
                    if (objects[i]->getCollider().intersects(objects[j]->getCollider())) {
                        objects[i]->onCollision(objects[j]);
                        objects[j]->onCollision(objects[i]);
                    }
                }
            }
        }
        
        void update(float deltaTime) {
            for (auto obj : objects) {
                obj->update(deltaTime);
            }
            checkCollisions();
        }
        
        void draw(sf::RenderWindow& window) {
            for (auto obj : objects) {
                obj->draw(window);
            }
        }
        
        GameObject* getObjectAt(const sf::Vector2f& point) {
            for (auto obj : objects) {
                if (obj->getCollider().contains(point)) {
                    return obj;
                }
            }
            return nullptr;
        }
    };
    
    // =========================================================================
    // СОЗДАНИЕ ТЕСТОВОЙ СЦЕНЫ
    // =========================================================================
    
    CollisionManager collisionManager;
    
    // Статические объекты (стены)
    sf::RectangleShape wall1(sf::Vector2f(800, 20));
    wall1.setPosition(0, 0);
    wall1.setFillColor(sf::Color::Blue);
    GameObject staticWall1(&wall1, true);
    collisionManager.addObject(&staticWall1);
    
    sf::RectangleShape wall2(sf::Vector2f(800, 20));
    wall2.setPosition(0, 580);
    wall2.setFillColor(sf::Color::Blue);
    GameObject staticWall2(&wall2, true);
    collisionManager.addObject(&staticWall2);
    
    sf::RectangleShape wall3(sf::Vector2f(20, 600));
    wall3.setPosition(0, 0);
    wall3.setFillColor(sf::Color::Blue);
    GameObject staticWall3(&wall3, true);
    collisionManager.addObject(&staticWall3);
    
    sf::RectangleShape wall4(sf::Vector2f(20, 600));
    wall4.setPosition(780, 0);
    wall4.setFillColor(sf::Color::Blue);
    GameObject staticWall4(&wall4, true);
    collisionManager.addObject(&staticWall4);
    
    // Динамические объекты
    std::vector<sf::CircleShape> dynamicShapes;
    std::vector<GameObject*> dynamicObjects;
    
    for (int i = 0; i < 5; ++i) {
        auto circle = new sf::CircleShape(20.0f);
        circle->setPosition(100 + i * 120, 200);
        circle->setFillColor(sf::Color::Green);
        dynamicShapes.push_back(*circle);
        
        auto obj = new GameObject(circle);
        obj->setVelocity(sf::Vector2f((rand() % 100 - 50), (rand() % 100 - 50)));
        collisionManager.addObject(obj);
        dynamicObjects.push_back(obj);
    }
    
    // Игрок
    sf::CircleShape playerShape(25.0f);
    playerShape.setFillColor(sf::Color::Yellow);
    playerShape.setPosition(400, 500);
    GameObject player(&playerShape);
    collisionManager.addObject(&player);
    
    // =========================================================================
    // ОБРАБОТКА ВВОДА ДЛЯ ИГРОКА
    // =========================================================================
    
    float playerSpeed = 200.0f;
    sf::Vector2f playerInput(0, 0);
    
    // =========================================================================
    // ОТЛАДОЧНАЯ ВИЗУАЛИЗАЦИЯ КОЛЛИЗИЙ
    // =========================================================================
    
    sf::RectangleShape debugRect;
    debugRect.setFillColor(sf::Color::Transparent);
    debugRect.setOutlineColor(sf::Color::Red);
    debugRect.setOutlineThickness(1.0f);
    
    sf::Clock gameClock;
    
    while (window.isOpen()) {
        float deltaTime = gameClock.restart().asSeconds();
        
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }
        
        // ======================
        // ОБРАБОТКА ВВОДА
        // ======================
        
        playerInput = sf::Vector2f(0, 0);
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) playerInput.y = -1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) playerInput.y = 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) playerInput.x = -1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) playerInput.x = 1;
        
        // Нормализация вектора движения (чтобы диагональ не была быстрее)
        float length = std::sqrt(playerInput.x * playerInput.x + playerInput.y * playerInput.y);
        if (length > 0) playerInput /= length;
        
        player.setVelocity(playerInput * playerSpeed);
        
        // ======================
        // ОБНОВЛЕНИЕ ИГРЫ
        // ======================
        
        collisionManager.update(deltaTime);
        
        // ======================
        // ОТРИСОВКА
        // ======================
        
        window.clear(sf::Color(30, 30, 50));
        
        // Отрисовка игровых объектов
        collisionManager.draw(window);
        
        // Отладочная отрисовка коллайдеров
        debugRect.setSize(sf::Vector2f(
            player.getCollider().getBounds().width,
            player.getCollider().getBounds().height
        ));
        debugRect.setPosition(
            player.getCollider().getBounds().left,
            player.getCollider().getBounds().top
        );
        window.draw(debugRect);
        
        // Информация об управлении
        sf::Font font;
        // font.loadFromFile(...);
        sf::Text infoText;
        infoText.setFont(font);
        infoText.setCharacterSize(16);
        infoText.setFillColor(sf::Color::White);
        infoText.setPosition(10, 10);
        infoText.setString("WASD - Move player\nCollisions shown in red");
        window.draw(infoText);
        
        window.display();
    }
    
    // Очистка динамической памяти
    for (auto obj : dynamicObjects) {
        delete obj;
    }
}

// =============================================================================
// РАСШИРЕННЫЙ КЛАСС КОЛЛИЗИЙ С РАЗРЕШЕНИЕМ
// =============================================================================

class PhysicsCollisionManager {
private:
    std::vector<GameObject*> objects;
    
public:
    void resolveCollision(GameObject* a, GameObject* b) {
        if (a->getIsStatic() && b->getIsStatic()) return;
        
        sf::FloatRect boundsA = a->getCollider().getBounds();
        sf::FloatRect boundsB = b->getCollider().getBounds();
        
        // Вычисление пересечения
        float overlapLeft = boundsB.left + boundsB.width - boundsA.left;
        float overlapRight = boundsA.left + boundsA.width - boundsB.left;
        float overlapTop = boundsB.top + boundsB.height - boundsA.top;
        float overlapBottom = boundsA.top + boundsA.height - boundsB.top;
        
        // Находим наименьшее перекрытие
        bool fromLeft = overlapLeft < overlapRight;
        bool fromTop = overlapTop < overlapBottom;
        float minOverlapX = fromLeft ? overlapLeft : overlapRight;
        float minOverlapY = fromTop ? overlapTop : overlapBottom;
        
        // Разрешаем коллизию по оси с наименьшим перекрытием
        if (minOverlapX < minOverlapY) {
            if (fromLeft) {
                a->setPosition(sf::Vector2f(boundsB.left + boundsB.width, a->getPosition().y));
            } else {
                a->setPosition(sf::Vector2f(boundsB.left - boundsA.width, a->getPosition().y));
            }
        } else {
            if (fromTop) {
                a->setPosition(sf::Vector2f(a->getPosition().x, boundsB.top + boundsB.height));
            } else {
                a->setPosition(sf::Vector2f(a->getPosition().x, boundsB.top - boundsA.height));
            }
        }
    }
};
```
## Группировка и управление объектами

```cpp
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <algorithm>

void demonstrateObjectManagement() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Object Management");
    
    // =========================================================================
    // БАЗОВЫЙ КЛАСС ДЛЯ ВСЕХ ИГРОВЫХ ОБЪЕКТОВ
    // =========================================================================
    
    class GameObject {
    protected:
        sf::Transformable transform;
        bool isActive;
        bool isVisible;
        std::string tag;
        
    public:
        GameObject() : isActive(true), isVisible(true), tag("Untagged") {}
        virtual ~GameObject() = default;
        
        // Виртуальные методы, которые должны быть реализованы в дочерних классах
        virtual void update(float deltaTime) = 0;
        virtual void draw(sf::RenderWindow& window) = 0;
        virtual sf::FloatRect getGlobalBounds() const = 0;
        
        // Базовые методы трансформации
        void setPosition(const sf::Vector2f& position) { transform.setPosition(position); }
        void setPosition(float x, float y) { transform.setPosition(x, y); }
        sf::Vector2f getPosition() const { return transform.getPosition(); }
        
        void setRotation(float angle) { transform.setRotation(angle); }
        float getRotation() const { return transform.getRotation(); }
        
        void setScale(const sf::Vector2f& factors) { transform.setScale(factors); }
        void setScale(float factorX, float factorY) { transform.setScale(factorX, factorY); }
        sf::Vector2f getScale() const { return transform.getScale(); }
        
        void move(const sf::Vector2f& offset) { transform.move(offset); }
        void move(float offsetX, float offsetY) { transform.move(offsetX, offsetY); }
        
        void rotate(float angle) { transform.rotate(angle); }
        void scale(const sf::Vector2f& factor) { transform.scale(factor); }
        
        // Управление состоянием
        void setActive(bool active) { isActive = active; }
        bool getActive() const { return isActive; }
        
        void setVisible(bool visible) { isVisible = visible; }
        bool getVisible() const { return isVisible; }
        
        void setTag(const std::string& newTag) { tag = newTag; }
        const std::string& getTag() const { return tag; }
        
        // Виртуальные методы для расширения функциональности
        virtual void onCollision(GameObject* other) {}
        virtual void onMouseEnter() {}
        virtual void onMouseLeave() {}
        virtual void onClick() {}
    };
    
    // =========================================================================
    // КОНКРЕТНЫЕ РЕАЛИЗАЦИИ ИГРОВЫХ ОБЪЕКТОВ
    // =========================================================================
    
    class SpriteObject : public GameObject {
    private:
        sf::Sprite sprite;
        
    public:
        SpriteObject(const sf::Texture& texture) {
            sprite.setTexture(texture);
            // Центрирование origin для удобства трансформаций
            sf::FloatRect bounds = sprite.getLocalBounds();
            sprite.setOrigin(bounds.width / 2, bounds.height / 2);
        }
        
        void update(float deltaTime) override {
            // Базовая логика обновления
            if (!isActive) return;
        }
        
        void draw(sf::RenderWindow& window) override {
            if (!isVisible) return;
            
            // Применяем трансформации к спрайту
            sprite.setPosition(transform.getPosition());
            sprite.setRotation(transform.getRotation());
            sprite.setScale(transform.getScale());
            
            window.draw(sprite);
        }
        
        sf::FloatRect getGlobalBounds() const override {
            return transform.transformRect(sprite.getGlobalBounds());
        }
        
        void setTexture(const sf::Texture& texture) {
            sprite.setTexture(texture);
        }
        
        void setColor(const sf::Color& color) {
            sprite.setColor(color);
        }
    };
    
    class ShapeObject : public GameObject {
    private:
        std::unique_ptr<sf::Shape> shape;
        
    public:
        ShapeObject(std::unique_ptr<sf::Shape> shapePtr) : shape(std::move(shapePtr)) {
            // Центрирование origin
            sf::FloatRect bounds = shape->getLocalBounds();
            shape->setOrigin(bounds.width / 2, bounds.height / 2);
        }
        
        void update(float deltaTime) override {
            if (!isActive) return;
        }
        
        void draw(sf::RenderWindow& window) override {
            if (!isVisible) return;
            
            shape->setPosition(transform.getPosition());
            shape->setRotation(transform.getRotation());
            shape->setScale(transform.getScale());
            
            window.draw(*shape);
        }
        
        sf::FloatRect getGlobalBounds() const override {
            return transform.transformRect(shape->getGlobalBounds());
        }
        
        void setFillColor(const sf::Color& color) {
            shape->setFillColor(color);
        }
        
        void setOutlineColor(const sf::Color& color) {
            shape->setOutlineColor(color);
        }
        
        void setOutlineThickness(float thickness) {
            shape->setOutlineThickness(thickness);
        }
    };
    
    class TextObject : public GameObject {
    private:
        sf::Text text;
        
    public:
        TextObject(const sf::Font& font, const std::string& content, unsigned int characterSize) {
            text.setFont(font);
            text.setString(content);
            text.setCharacterSize(characterSize);
            
            // Центрирование текста
            sf::FloatRect bounds = text.getLocalBounds();
            text.setOrigin(bounds.width / 2, bounds.height / 2);
        }
        
        void update(float deltaTime) override {
            if (!isActive) return;
        }
        
        void draw(sf::RenderWindow& window) override {
            if (!isVisible) return;
            
            text.setPosition(transform.getPosition());
            text.setRotation(transform.getRotation());
            text.setScale(transform.getScale());
            
            window.draw(text);
        }
        
        sf::FloatRect getGlobalBounds() const override {
            return transform.transformRect(text.getGlobalBounds());
        }
        
        void setString(const std::string& content) {
            text.setString(content);
        }
        
        void setColor(const sf::Color& color) {
            text.setFillColor(color);
        }
        
        void setStyle(sf::Text::Style style) {
            text.setStyle(style);
        }
    };
    
    // =========================================================================
    // МЕНЕДЖЕР ОБЪЕКТОВ
    // =========================================================================
    
    class ObjectManager {
    private:
        std::vector<std::unique_ptr<GameObject>> objects;
        std::vector<GameObject*> objectsToAdd;
        std::vector<size_t> objectsToRemove;
        
    public:
        void update(float deltaTime) {
            // Добавление новых объектов
            for (auto& obj : objectsToAdd) {
                objects.push_back(std::unique_ptr<GameObject>(obj));
            }
            objectsToAdd.clear();
            
            // Обновление существующих объектов
            for (size_t i = 0; i < objects.size(); ++i) {
                if (objects[i]->getActive()) {
                    objects[i]->update(deltaTime);
                }
            }
            
            // Удаление объектов
            std::sort(objectsToRemove.begin(), objectsToRemove.end(), std::greater<size_t>());
            for (auto index : objectsToRemove) {
                if (index < objects.size()) {
                    objects.erase(objects.begin() + index);
                }
            }
            objectsToRemove.clear();
        }
        
        void draw(sf::RenderWindow& window) {
            for (auto& obj : objects) {
                if (obj->getVisible()) {
                    obj->draw(window);
                }
            }
        }
        
        template<typename T, typename... Args>
        T* createObject(Args&&... args) {
            static_assert(std::is_base_of<GameObject, T>::value, 
                         "T must inherit from GameObject");
            
            T* newObj = new T(std::forward<Args>(args)...);
            objectsToAdd.push_back(newObj);
            return newObj;
        }
        
        void addObject(std::unique_ptr<GameObject> object) {
            objects.push_back(std::move(object));
        }
        
        void removeObject(GameObject* object) {
            for (size_t i = 0; i < objects.size(); ++i) {
                if (objects[i].get() == object) {
                    objectsToRemove.push_back(i);
                    return;
                }
            }
        }
        
        void removeObjectByIndex(size_t index) {
            if (index < objects.size()) {
                objectsToRemove.push_back(index);
            }
        }
        
        void clearAllObjects() {
            objects.clear();
            objectsToAdd.clear();
            objectsToRemove.clear();
        }
        
        GameObject* getObjectByIndex(size_t index) {
            if (index < objects.size()) {
                return objects[index].get();
            }
            return nullptr;
        }
        
        std::vector<GameObject*> getObjectsByTag(const std::string& tag) {
            std::vector<GameObject*> result;
            for (auto& obj : objects) {
                if (obj->getTag() == tag) {
                    result.push_back(obj.get());
                }
            }
            return result;
        }
        
        GameObject* findObjectByName(const std::string& name) {
            // Здесь можно добавить систему имен, если нужно
            return nullptr;
        }
        
        size_t getObjectCount() const {
            return objects.size();
        }
        
        // Поиск объектов в области
        std::vector<GameObject*> findObjectsInArea(const sf::FloatRect& area) {
            std::vector<GameObject*> result;
            for (auto& obj : objects) {
                if (area.intersects(obj->getGlobalBounds())) {
                    result.push_back(obj.get());
                }
            }
            return result;
        }
        
        // Поиск объектов по условию
        template<typename Predicate>
        std::vector<GameObject*> findObjects(Predicate pred) {
            std::vector<GameObject*> result;
            for (auto& obj : objects) {
                if (pred(obj.get())) {
                    result.push_back(obj.get());
                }
            }
            return result;
        }
    };
    
    // =========================================================================
    // КОМПОЗИЦИЯ ОБЪЕКТОВ (РОДИТЕЛЬСКАЯ СИСТЕМА)
    // =========================================================================
    
    class CompositeObject : public GameObject {
    private:
        std::vector<std::unique_ptr<GameObject>> children;
        sf::Vector2f localOffset;
        
    public:
        void addChild(std::unique_ptr<GameObject> child) {
            children.push_back(std::move(child));
        }
        
        template<typename T, typename... Args>
        T* createChild(Args&&... args) {
            static_assert(std::is_base_of<GameObject, T>::value, 
                         "T must inherit from GameObject");
            
            auto child = std::make_unique<T>(std::forward<Args>(args)...);
            T* ptr = child.get();
            children.push_back(std::move(child));
            return ptr;
        }
        
        void update(float deltaTime) override {
            if (!isActive) return;
            
            // Обновляем детей с учетом родительской трансформации
            for (auto& child : children) {
                if (child->getActive()) {
                    // Применяем относительные трансформации
                    sf::Vector2f worldPos = transform.getTransform().transformPoint(child->getPosition());
                    child->setPosition(worldPos);
                    
                    child->update(deltaTime);
                }
            }
        }
        
        void draw(sf::RenderWindow& window) override {
            if (!isVisible) return;
            
            for (auto& child : children) {
                if (child->getVisible()) {
                    child->draw(window);
                }
            }
        }
        
        sf::FloatRect getGlobalBounds() const override {
            if (children.empty()) {
                return sf::FloatRect(transform.getPosition().x, transform.getPosition().y, 0, 0);
            }
            
            // Вычисляем общие границы всех детей
            sf::FloatRect bounds = children[0]->getGlobalBounds();
            for (size_t i = 1; i < children.size(); ++i) {
                sf::FloatRect childBounds = children[i]->getGlobalBounds();
                bounds.left = std::min(bounds.left, childBounds.left);
                bounds.top = std::min(bounds.top, childBounds.top);
                bounds.width = std::max(bounds.left + bounds.width, childBounds.left + childBounds.width) - bounds.left;
                bounds.height = std::max(bounds.top + bounds.height, childBounds.top + childBounds.height) - bounds.top;
            }
            
            return bounds;
        }
        
        void removeChild(GameObject* child) {
            children.erase(
                std::remove_if(children.begin(), children.end(),
                    [child](const std::unique_ptr<GameObject>& obj) {
                        return obj.get() == child;
                    }),
                children.end()
            );
        }
        
        void clearChildren() {
            children.clear();
        }
        
        size_t getChildCount() const {
            return children.size();
        }
    };
    
    // =========================================================================
    // ПУЛ ОБЪЕКТОВ ДЛЯ ОПТИМИЗАЦИИ
    // =========================================================================
    
    template<typename T>
    class ObjectPool {
    private:
        std::vector<std::unique_ptr<T>> pool;
        size_t nextAvailable;
        
    public:
        ObjectPool(size_t initialSize = 100) : nextAvailable(0) {
            for (size_t i = 0; i < initialSize; ++i) {
                pool.push_back(std::make_unique<T>());
                pool.back()->setActive(false);
            }
        }
        
        T* getObject() {
            // Ищем неактивный объект
            for (size_t i = 0; i < pool.size(); ++i) {
                if (!pool[i]->getActive()) {
                    pool[i]->setActive(true);
                    return pool[i].get();
                }
            }
            
            // Если не нашли, создаем новый
            auto newObj = std::make_unique<T>();
            T* ptr = newObj.get();
            pool.push_back(std::move(newObj));
            return ptr;
        }
        
        void returnObject(T* object) {
            object->setActive(false);
            // Сбрасываем состояние объекта к начальному, если нужно
        }
        
        size_t getActiveCount() const {
            size_t count = 0;
            for (const auto& obj : pool) {
                if (obj->getActive()) {
                    count++;
                }
            }
            return count;
        }
        
        size_t getTotalCount() const {
            return pool.size();
        }
    };
    
    // =========================================================================
    // ДЕМОНСТРАЦИЯ СИСТЕМЫ
    // =========================================================================
    
    ObjectManager objectManager;
    
    // Создание текстур для демонстрации
    sf::Texture playerTexture, enemyTexture, bulletTexture;
    // Загрузка текстур...
    
    // Создание различных объектов
    auto player = objectManager.createObject<SpriteObject>(playerTexture);
    player->setPosition(400, 300);
    player->setTag("Player");
    player->setScale(2.0f, 2.0f);
    
    // Создание группы врагов
    for (int i = 0; i < 5; ++i) {
        auto enemy = objectManager.createObject<ShapeObject>(
            std::make_unique<sf::CircleShape>(20.0f)
        );
        enemy->setPosition(100 + i * 120, 100);
        enemy->setTag("Enemy");
        enemy->setFillColor(sf::Color::Red);
    }
    
    // Создание составного объекта (сложный объект из нескольких частей)
    auto spaceship = std::make_unique<CompositeObject>();
    spaceship->setPosition(200, 400);
    
    // Корпус корабля
    auto hull = spaceship->createChild<ShapeObject>(std::make_unique<sf::CircleShape>(30.0f));
    hull->setPosition(0, 0);
    hull->setFillColor(sf::Color::Blue);
    
    // Кабина корабля
    auto cockpit = spaceship->createChild<ShapeObject>(std::make_unique<sf::CircleShape>(15.0f));
    cockpit->setPosition(0, -20);
    cockpit->setFillColor(sf::Color::Cyan);
    
    // Двигатели корабля
    auto engine1 = spaceship->createChild<ShapeObject>(std::make_unique<sf::RectangleShape>(sf::Vector2f(10, 20)));
    engine1->setPosition(-20, 25);
    engine1->setFillColor(sf::Color::Yellow);
    
    auto engine2 = spaceship->createChild<ShapeObject>(std::make_unique<sf::RectangleShape>(sf::Vector2f(10, 20)));
    engine2->setPosition(20, 25);
    engine2->setFillColor(sf::Color::Yellow);
    
    objectManager.addObject(std::move(spaceship));
    
    // Создание текстового объекта
    auto infoText = objectManager.createObject<TextObject>(sf::Font(), "Object Count: 0", 20);
    infoText->setPosition(100, 500);
    infoText->setColor(sf::Color::White);
    
    sf::Clock gameClock;
    
    while (window.isOpen()) {
        float deltaTime = gameClock.restart().asSeconds();
        
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            
            // Создание пуль по клику
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    auto bullet = objectManager.createObject<ShapeObject>(
                        std::make_unique<sf::CircleShape>(5.0f)
                    );
                    bullet->setPosition(player->getPosition());
                    bullet->setTag("Bullet");
                    bullet->setFillColor(sf::Color::Green);
                    
                    // Направление к курсору
                    sf::Vector2f direction(
                        event.mouseButton.x - player->getPosition().x,
                        event.mouseButton.y - player->getPosition().y
                    );
                    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
                    if (length > 0) direction /= length;
                    
                    // Здесь можно добавить velocity компонент для движения
                }
            }
        }
        
        // Обновление информации
        std::string info = "Object Count: " + std::to_string(objectManager.getObjectCount());
        if (auto textObj = dynamic_cast<TextObject*>(infoText)) {
            textObj->setString(info);
        }
        
        // Обновление всех объектов
        objectManager.update(deltaTime);
        
        // Отрисовка
        window.clear(sf::Color(30, 30, 50));
        objectManager.draw(window);
        window.display();
    }
}

// =============================================================================
// СИСТЕМА КОМПОНЕНТОВ (ECS-ПОДОБНАЯ)
// =============================================================================

class Component {
public:
    virtual ~Component() = default;
    virtual void update(float deltaTime) {}
    virtual void draw(sf::RenderWindow& window) {}
};

class TransformComponent : public Component {
public:
    sf::Vector2f position;
    float rotation;
    sf::Vector2f scale;
    
    TransformComponent() : position(0, 0), rotation(0), scale(1, 1) {}
};

class RenderComponent : public Component {
private:
    std::unique_ptr<sf::Shape> shape;
    
public:
    RenderComponent(std::unique_ptr<sf::Shape> shapePtr) : shape(std::move(shapePtr)) {}
    
    void draw(sf::RenderWindow& window) override {
        window.draw(*shape);
    }
    
    void setColor(const sf::Color& color) {
        shape->setFillColor(color);
    }
};

class Entity {
private:
    std::vector<std::unique_ptr<Component>> components;
    
public:
    template<typename T, typename... Args>
    T* addComponent(Args&&... args) {
        static_assert(std::is_base_of<Component, T>::value, 
                     "T must inherit from Component");
        
        auto component = std::make_unique<T>(std::forward<Args>(args)...);
        T* ptr = component.get();
        components.push_back(std::move(component));
        return ptr;
    }
    
    template<typename T>
    T* getComponent() {
        for (auto& component : components) {
            if (auto ptr = dynamic_cast<T*>(component.get())) {
                return ptr;
            }
        }
        return nullptr;
    }
    
    void update(float deltaTime) {
        for (auto& component : components) {
            component->update(deltaTime);
        }
    }
    
    void draw(sf::RenderWindow& window) {
        for (auto& component : components) {
            component->draw(window);
        }
    }
};
```

## Оптимизация отрисовки

```cpp
#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>

void demonstrateOptimization() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Optimization Techniques");
    
    // =========================================================================
    // BATCH RENDERING - ГРУППИРОВКА ОБЪЕКТОВ ПО ТЕКСТУРАМ
    // =========================================================================
    
    class BatchRenderer {
    private:
        struct RenderBatch {
            sf::Texture* texture;
            sf::VertexArray vertices;
            sf::RenderStates states;
        };
        
        std::vector<RenderBatch> batches;
        
    public:
        void begin() {
            batches.clear();
        }
        
        void addSprite(const sf::Sprite& sprite) {
            sf::Texture* texture = const_cast<sf::Texture*>(sprite.getTexture());
            
            // Ищем batch с такой же текстурой
            auto it = std::find_if(batches.begin(), batches.end(),
                [texture](const RenderBatch& batch) {
                    return batch.texture == texture;
                });
            
            if (it == batches.end()) {
                // Создаем новый batch
                RenderBatch newBatch;
                newBatch.texture = texture;
                newBatch.vertices.setPrimitiveType(sf::Quads);
                newBatch.states.texture = texture;
                batches.push_back(newBatch);
                it = batches.end() - 1;
            }
            
            // Добавляем вершины спрайта
            sf::FloatRect bounds = sprite.getGlobalBounds();
            sf::FloatRect texRect = sprite.getTextureRect();
            sf::Color color = sprite.getColor();
            const sf::Transform& transform = sprite.getTransform();
            
            sf::Vertex quad[4];
            
            // Левый верхний
            quad[0].position = transform.transformPoint(0, 0);
            quad[0].texCoords = sf::Vector2f(texRect.left, texRect.top);
            quad[0].color = color;
            
            // Правый верхний
            quad[1].position = transform.transformPoint(bounds.width, 0);
            quad[1].texCoords = sf::Vector2f(texRect.left + texRect.width, texRect.top);
            quad[1].color = color;
            
            // Правый нижний
            quad[2].position = transform.transformPoint(bounds.width, bounds.height);
            quad[2].texCoords = sf::Vector2f(texRect.left + texRect.width, texRect.top + texRect.height);
            quad[2].color = color;
            
            // Левый нижний
            quad[3].position = transform.transformPoint(0, bounds.height);
            quad[3].texCoords = sf::Vector2f(texRect.left, texRect.top + texRect.height);
            quad[3].color = color;
            
            // Добавляем вершины в batch
            for (int i = 0; i < 4; ++i) {
                it->vertices.append(quad[i]);
            }
        }
        
        void end(sf::RenderWindow& window) {
            for (auto& batch : batches) {
                window.draw(batch.vertices, batch.states);
            }
        }
    };
    
    // =========================================================================
    // SPATIAL PARTITIONING - ПРОСТРАНСТВЕННОЕ РАЗДЕЛЕНИЕ
    // =========================================================================
    
    class SpatialGrid {
    private:
        struct Cell {
            std::vector<sf::Sprite*> objects;
        };
        
        std::vector<std::vector<Cell>> grid;
        sf::Vector2u gridSize;
        sf::Vector2f cellSize;
        sf::FloatRect worldBounds;
        
    public:
        SpatialGrid(const sf::FloatRect& bounds, unsigned int cellsX, unsigned int cellsY) 
            : worldBounds(bounds) {
            gridSize = sf::Vector2u(cellsX, cellsY);
            cellSize = sf::Vector2f(
                bounds.width / cellsX,
                bounds.height / cellsY
            );
            
            grid.resize(cellsY, std::vector<Cell>(cellsX));
        }
        
        void clear() {
            for (auto& row : grid) {
                for (auto& cell : row) {
                    cell.objects.clear();
                }
            }
        }
        
        void insert(sf::Sprite* object) {
            sf::FloatRect bounds = object->getGlobalBounds();
            
            // Определяем ячейки, которые пересекает объект
            unsigned int startX = std::max(0u, static_cast<unsigned int>((bounds.left - worldBounds.left) / cellSize.x));
            unsigned int endX = std::min(gridSize.x - 1, static_cast<unsigned int>((bounds.left + bounds.width - worldBounds.left) / cellSize.x));
            
            unsigned int startY = std::max(0u, static_cast<unsigned int>((bounds.top - worldBounds.top) / cellSize.y));
            unsigned int endY = std::min(gridSize.y - 1, static_cast<unsigned int>((bounds.top + bounds.height - worldBounds.top) / cellSize.y));
            
            for (unsigned int y = startY; y <= endY; ++y) {
                for (unsigned int x = startX; x <= endX; ++x) {
                    grid[y][x].objects.push_back(object);
                }
            }
        }
        
        std::vector<sf::Sprite*> getObjectsInArea(const sf::FloatRect& area) {
            std::vector<sf::Sprite*> result;
            
            unsigned int startX = std::max(0u, static_cast<unsigned int>((area.left - worldBounds.left) / cellSize.x));
            unsigned int endX = std::min(gridSize.x - 1, static_cast<unsigned int>((area.left + area.width - worldBounds.left) / cellSize.x));
            
            unsigned int startY = std::max(0u, static_cast<unsigned int>((area.top - worldBounds.top) / cellSize.y));
            unsigned int endY = std::min(gridSize.y - 1, static_cast<unsigned int>((area.top + area.height - worldBounds.top) / cellSize.y));
            
            for (unsigned int y = startY; y <= endY; ++y) {
                for (unsigned int x = startX; x <= endX; ++x) {
                    for (auto obj : grid[y][x].objects) {
                        // Проверяем, что объект еще не добавлен (может быть в нескольких ячейках)
                        if (std::find(result.begin(), result.end(), obj) == result.end()) {
                            result.push_back(obj);
                        }
                    }
                }
            }
            
            return result;
        }
    };
    
    // =========================================================================
    // FRUSTUM CULLING - ОТСЕЧЕНИЕ НЕВИДИМЫХ ОБЪЕКТОВ
    // =========================================================================
    
    class FrustumCuller {
    private:
        sf::View* currentView;
        
    public:
        void setView(sf::View* view) {
            currentView = view;
        }
        
        bool isVisible(const sf::FloatRect& bounds) {
            if (!currentView) return true;
            
            sf::FloatRect viewBounds(
                currentView->getCenter().x - currentView->getSize().x / 2,
                currentView->getCenter().y - currentView->getSize().y / 2,
                currentView->getSize().x,
                currentView->getSize().y
            );
            
            return viewBounds.intersects(bounds);
        }
        
        std::vector<sf::Sprite*> cullObjects(const std::vector<sf::Sprite*>& objects) {
            std::vector<sf::Sprite*> visibleObjects;
            
            for (auto obj : objects) {
                if (isVisible(obj->getGlobalBounds())) {
                    visibleObjects.push_back(obj);
                }
            }
            
            return visibleObjects;
        }
    };
    
    // =========================================================================
    // ДЕМОНСТРАЦИЯ ОПТИМИЗАЦИЙ
    // =========================================================================
    
    // Создание большого количества объектов для тестирования
    std::vector<sf::Sprite> testSprites;
    sf::Texture testTexture;
    testTexture.create(32, 32); // Тестовая текстура
    
    for (int i = 0; i < 1000; ++i) {
        sf::Sprite sprite(testTexture);
        sprite.setPosition(rand() % 1600, rand() % 1200);
        sprite.setColor(sf::Color(rand() % 255, rand() % 255, rand() % 255));
        testSprites.push_back(sprite);
    }
    
    // Инициализация систем оптимизации
    BatchRenderer batchRenderer;
    SpatialGrid spatialGrid(sf::FloatRect(0, 0, 1600, 1200), 16, 12);
    FrustumCuller culler;
    
    // Заполнение spatial grid
    for (auto& sprite : testSprites) {
        spatialGrid.insert(&sprite);
    }
    
    // Создание камеры
    sf::View gameView(sf::Vector2f(400, 300), sf::Vector2f(800, 600));
    culler.setView(&gameView);
    
    // Переменные для статистики
    int visibleObjects = 0;
    int totalObjects = testSprites.size();
    
    sf::Clock gameClock;
    
    while (window.isOpen()) {
        float deltaTime = gameClock.restart().asSeconds();
        
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }
        
        // Управление камерой
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) gameView.move(0, -300 * deltaTime);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) gameView.move(0, 300 * deltaTime);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) gameView.move(-300 * deltaTime, 0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) gameView.move(300 * deltaTime, 0);
        
        window.setView(gameView);
        
        // ======================
        // ОПТИМИЗИРОВАННАЯ ОТРИСОВКА
        // ======================
        
        // 1. Получаем объекты в области видимости камеры
        sf::FloatRect viewBounds(
            gameView.getCenter().x - gameView.getSize().x / 2,
            gameView.getCenter().y - gameView.getSize().y / 2,
            gameView.getSize().x,
            gameView.getSize().y
        );
        
        auto potentialObjects = spatialGrid.getObjectsInArea(viewBounds);
        
        // 2. Применяем frustum culling
        auto visibleSprites = culler.cullObjects(potentialObjects);
        visibleObjects = visibleSprites.size();
        
        // 3. Batch rendering
        batchRenderer.begin();
        for (auto sprite : visibleSprites) {
            batchRenderer.addSprite(*sprite);
        }
        
        window.clear(sf::Color(20, 20, 40));
        
        batchRenderer.end(window);
        
        // ======================
        // ОТЛАДОЧНАЯ ИНФОРМАЦИЯ
        // ======================
        
        window.setView(window.getDefaultView());
        
        sf::Font font;
        // font.loadFromFile(...);
        sf::Text statsText;
        statsText.setFont(font);
        statsText.setCharacterSize(16);
        statsText.setFillColor(sf::Color::White);
        statsText.setPosition(10, 10);
        
        std::string stats = "Total Objects: " + std::to_string(totalObjects) +
                           "\nVisible Objects: " + std::to_string(visibleObjects) +
                           "\nRendering Efficiency: " + 
                           std::to_string((float)visibleObjects / totalObjects * 100) + "%" +
                           "\nWASD - Move camera";
        statsText.setString(stats);
        window.draw(statsText);
        
        window.display();
    }
}

// =============================================================================
// ДОПОЛНИТЕЛЬНЫЕ ТЕХНИКИ ОПТИМИЗАЦИИ
// =============================================================================

// 1. OBJECT POOLING - переиспользование объектов
class BulletPool {
private:
    std::vector<sf::Sprite> bullets;
    std::vector<bool> active;
    
public:
    BulletPool(size_t size) {
        bullets.resize(size);
        active.resize(size, false);
        // Инициализация пуль...
    }
    
    sf::Sprite* getBullet() {
        for (size_t i = 0; i < active.size(); ++i) {
            if (!active[i]) {
                active[i] = true;
                return &bullets[i];
            }
        }
        return nullptr;
    }
    
    void returnBullet(sf::Sprite* bullet) {
        for (size_t i = 0; i < bullets.size(); ++i) {
            if (&bullets[i] == bullet) {
                active[i] = false;
                return;
            }
        }
    }
};

// 2. TEXTURE ATLAS - объединение текстур
class TextureAtlas {
private:
    sf::Texture texture;
    std::map<std::string, sf::IntRect> regions;
    
public:
    bool loadFromFile(const std::string& filename) {
        return texture.loadFromFile(filename);
    }
    
    void addRegion(const std::string& name, const sf::IntRect& region) {
        regions[name] = region;
    }
    
    sf::Sprite createSprite(const std::string& regionName) {
        sf::Sprite sprite(texture);
        auto it = regions.find(regionName);
        if (it != regions.end()) {
            sprite.setTextureRect(it->second);
        }
        return sprite;
    }
};

// 3. LEVEL OF DETAIL (LOD) - разные уровни детализации
class LODSystem {
private:
    struct LODLevel {
        float distance;
        sf::Texture* texture;
    };
    
    std::vector<LODLevel> levels;
    
public:
    void addLevel(float distance, sf::Texture* texture) {
        levels.push_back({distance, texture});
        std::sort(levels.begin(), levels.end(), 
            [](const LODLevel& a, const LODLevel& b) {
                return a.distance < b.distance;
            });
    }
    
    sf::Texture* getAppropriateTexture(float distance) {
        for (const auto& level : levels) {
            if (distance <= level.distance) {
                return level.texture;
            }
        }
        return levels.empty() ? nullptr : levels.back().texture;
    }
};
```

Это полное руководство по работе с SFML для 2D графики! Основные моменты:

1. **Окна и базовый цикл** - создание, настройка, управление
2. **Трансформации** - позиционирование, вращение, масштабирование  
3. **Текстуры и спрайты** - загрузка, настройка, анимация
4. **Геометрические фигуры** - прямоугольники, круги, многоугольники
5. **Цвета и прозрачность** - работа с alpha-каналом, blend modes
6. **Текст и шрифты** - рендеринг текста, стили, выравнивание
7. **Камеры и виды** - управление видом, split-screen, minimap
8. **Анимации** - линейные, криволинейные, tweening, физика
9. **Коллизии** - AABB, circle коллизии, разрешение столкновений
10. **Управление объектами** - иерархия, композиция, пулы объектов
11. **Оптимизация** - batch rendering, spatial partitioning, culling

Каждый раздел содержит практические примеры и готовые к использованию классы для быстрого старта в разработке 2D игр!