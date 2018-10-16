#include "Player.hpp"
#include "CommanQueue.hpp"
#include "Aircraft.hpp"

struct AircraftMover
{
	AircraftMover(float vx, float vy) : velocity(vx, vy)
	{

	}
	void operator() (Aircraft& aircraft, sf::Time) const
	{
		aircraft.accelerate(velocity);
	}

	sf::Vector2f velocity;
};

Player::Player()
{
	//Set initial key bindings
	mKeyBindings[sf::Keyboard::Left] = Action::MoveLeft;
	mKeyBindings[sf::Keyboard::Right] = Action::MoveRight;
	mKeyBindings[sf::Keyboard::Up] = Action::MoveUp;
	mKeyBindings[sf::Keyboard::Down] = Action::MoveDown;

	//Set initial actions bindings
	initializeActions();

	//Add all categories to the player's aircraft
	for (auto& pair : mActionBinding)
	{
		pair.second.category = static_cast<unsigned int>(Category::PlayerAircraft);
	}
}

void Player::initializeActions()
{
	const float playerSpeed = 200.f;
	//Assign commands to actions
	mActionBinding[Action::MoveLeft].action = derivedAction<Aircraft>(AircraftMover(-playerSpeed, 0.f));
	mActionBinding[Action::MoveRight].action = derivedAction<Aircraft>(AircraftMover(playerSpeed, 0.f));
	mActionBinding[Action::MoveUp].action = derivedAction<Aircraft>(AircraftMover(0.f, -playerSpeed));
	mActionBinding[Action::MoveDown].action = derivedAction<Aircraft>(AircraftMover(0.f, playerSpeed));
}

void Player::handleEvent(const sf::Event & event, CommandQueue & commands)
{
	if (event.type == sf::Event::KeyPressed)
	{
		//Check if key pressed is in the key bindings, if so trigger command
		auto found = mKeyBindings.find(event.key.code);
		if (found != mKeyBindings.end() && !isRealtimeAction(found->second))
		{
			commands.push(mActionBinding[found->second]);
		}
	}
}

void Player::handleRealtimeInput(CommandQueue & commands)
{
	//Check if any key binding keys are pressed
	for (auto pair : mKeyBindings)
	{
		if (sf::Keyboard::isKeyPressed(pair.first) && isRealtimeAction(pair.second))
		{
			commands.push(mActionBinding[pair.second]);
		}
	}
}

void Player::assignKey(Action action, sf::Keyboard::Key key)
{
	//Remove all keys that are already mapped to an action
	for (auto itr = mKeyBindings.begin(); itr != mKeyBindings.end();) //Using iterator allows editing map elements
	{
		if (itr->second == action)
		{
			mKeyBindings.erase(itr++); //Erase then iterate
		}
		else
		{
			++itr; //Otherwise continue
		}
		//Insert new binding
		mKeyBindings[key] = action;
	}
}

sf::Keyboard::Key Player::getAssignedKey(Action action) const
{
	for (auto pair : mKeyBindings)
	{
		if (pair.second == action)
		{
			return pair.first;
		}
	}
	return sf::Keyboard::Unknown;
}

bool Player::isRealtimeAction(Action action)
{
	switch (action)
	{
		case Action::MoveLeft:
		case Action::MoveRight:
		case Action::MoveUp:
		case Action::MoveDown:
			return true;
		default:
			return false;
	}
}
