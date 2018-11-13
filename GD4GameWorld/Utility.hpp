#pragma once
#include <SFML/Window/Keyboard.hpp>
#include <sstream>
namespace sf 
{
	class Sprite;
	class Text;
}
void centreOrigin(sf::Sprite& sprite);
void centreOrigin(sf::Text& text);

// Convert enumerators to strings
std::string toString(sf::Keyboard::Key key);
