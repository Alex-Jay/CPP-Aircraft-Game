#pragma once
#include <SFML/Window/Keyboard.hpp>
#include <sstream>
namespace sf 
{
	class Sprite;
	class Text;
	class Vector2f;
}
void centreOrigin(sf::Sprite& sprite);
void centreOrigin(sf::Text& text);
float toRadians(float degrees);
float toDegrees(float radians);
float randomInt(int exclusiveMax);
sf::Vector2f unitVector(sf::Vector2f vector);

// Convert enumerators to strings
std::string toString(sf::Keyboard::Key key);
