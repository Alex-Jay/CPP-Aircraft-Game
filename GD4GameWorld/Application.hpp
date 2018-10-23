#pragma once

#include "Player.hpp"
#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"
#include "StateStack.hpp"

#include "SFML/System/Time.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/Graphics/RenderWindow.hpp"

class Application : private sf::NonCopyable
{
public:
	Application();
	void run();

private:
	void processInput();
	void update(sf::Time dt);
	void render();

	void updateStatistics(sf::Time elapsedTime);
	void registerStates();

private:
	static const sf::Time TimePerFrame;
	TextureHolder mTextures;
	FontHolder mFonts;
	sf::RenderWindow mWindow;
	Player mPlayer;
	StateStack mStateStack;
	sf::Text mStatisticsText;
	sf::Time mStatisticsUpdateTime;
	std::size_t mStatisticsNumFrames;
};