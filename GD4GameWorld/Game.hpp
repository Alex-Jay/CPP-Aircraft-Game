#pragma once

#include "World.hpp"

#include "SFML/System/Time.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/RenderWindow.hpp"

class Game : private sf::NonCopyable
{
public:
	Game();
	void run();

private:
	void processEvents();
	void update(sf::Time dt);
	void render();
	void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);
	void updateStatistics(sf::Time elapsedTime);

private:
	sf::RenderWindow mWindow;
	World mWorld;
	sf::Font mFont;
	sf::Text mStatisticsText;
	sf::Time mStatisticsUpdateTime;
	std::size_t mStatisticsNumFrames;
	static const sf::Time TimePerFrame;
};