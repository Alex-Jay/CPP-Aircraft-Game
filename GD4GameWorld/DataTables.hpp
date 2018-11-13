#pragma once
#include "ResourceIdentifiers.hpp"

#include "SFML/System/Time.hpp"
#include "SFML/Graphics/Color.hpp"

#include <vector>
#include <functional>

class Aircraft;

struct Direction
{
	Direction(float angle, float distance) 
		: angle(angle), distance(distance)
	{
	}

	float angle;
	float distance;
};

struct AircraftData
{
	int hitPoints;
	float speed;
	TextureIDs texture;
	sf::Time fireInterval;
	std::vector<Direction> directions;
};

struct ProjectileData
{
	int damage;
	float speed;
	TextureIDs texture;
};

struct PickupData
{
	std::function<void(Aircraft&)> action;
	TextureIDs texture;
};

std::vector<AircraftData> initializedAircraftData();
std::vector<ProjectileData> initializedProjectileData();
std::vector<PickupData> initializedPickupData();