#pragma once
#include "StateIdentifiers.hpp"
#include "ResourceIdentifiers.hpp"

#include "SFML/System/Time.hpp"
#include "SFML/Window/Event.hpp"

// Used for smart pointers
#include <memory>

// Forward-declaration
namespace sf
{
	class RenderWindow;
}

class Player;
class StateStack;

class State
{
public:
	typedef std::unique_ptr<State> Ptr;

	struct Context
	{
		Context(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts, Player& player);
		sf::RenderWindow* window;
		TextureHolder* textures;
		FontHolder* fonts;
		Player* player;
	};

public:
	State(StateStack& stack, Context context);
	virtual ~State();

	virtual void draw() = 0;
	virtual bool update(sf::Time dt) = 0;
	virtual bool handleEvent(const sf::Event& event) = 0;

protected:
	void requestStackPush(StateIDs stateID);
	void requestStackPop();
	void requestStackClear();

	Context getContext() const;

private:
	StateStack* mStack;
	Context mContext;
};