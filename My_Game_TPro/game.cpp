#include "game.h"














//=====================================Œ“ƒ≈À‹Õ€≈ ‘”Õ ÷»»=======================

bool mouseNearPoint(sf::Vector2f point, float distance, sf::RenderWindow* window)
{
	return getDistance((sf::Vector2f)sf::Mouse::getPosition(*window), point) <= distance;
}

bool mouseClickOnButton(sf::RectangleShape button, sf::RenderWindow* window)
{
	return isPointIntoShape((sf::Vector2f)sf::Mouse::getPosition(*window), button);
}
