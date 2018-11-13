#include "Aircraft.hpp"
#include "ResourceHolder.hpp"
#include "DataTables.hpp"
#include "Pickup.hpp"
#include "CommandQueue.hpp"
#include "Constants.hpp"

#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/RenderStates.hpp"

// Local (Global) namespace, e.g. Table is only visible inside Aircraft.cpp
namespace
{
	const std::vector<AircraftData> Table = initializedAircraftData();
}

TextureIDs toTextureID(Aircraft::Type type)
{
	switch (type)
	{
	case Aircraft::Type::Eagle:
		return TextureIDs::Eagle;
	case Aircraft::Type::Raptor:
		return TextureIDs::Raptor;
	default:
		return TextureIDs::Eagle;
	}
}

Aircraft::Aircraft(Type type, const TextureHolder & textures, const FontHolder& fonts)
	: Entity(Table[static_cast<int>(type)].hitPoints)
	, mType(type)
	, mSprite(textures.get(Table[static_cast<int>(type)].texture))
	, mFireCommand()
	, mMissileCommand()
	, mFireCountdown(sf::Time::Zero)
	, mIsFiring(false)
	, mIsLaunchingMissile(false)
	, mFireRateLevel(FIRE_RATE_LEVEL_ONE)
	, mSpreadLevel(SREAD_LEVEL)
	, mMissileAmmo(MISSLE_AMMO)
	, mDropPickupCommand()
	, mTravelledDistance(0)
	, mDirectionIndex(0)
	, mHealthDisplay(nullptr)
	, mMissileDisplay(nullptr)
{
	sf::FloatRect bounds = mSprite.getLocalBounds();
	mSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

unsigned int Aircraft::getCategory() const
{
	switch (mType)
	{
	case Type::Eagle:
		return static_cast<unsigned int>(Category::PlayerAircraft);
	default:
		return static_cast<unsigned int>(Category::EnemyAircraft);
	}
}

sf::FloatRect Aircraft::getBoundingRectangle()
{
	return sf::FloatRect();
}

bool Aircraft::isMarkedForRemoval() const
{
	return false;
}

bool Aircraft::isAllied() const
{
	return false;
}

float Aircraft::getMaxSpeed() const
{
	return 0.0f;
}

void Aircraft::increaseFireRate()
{
}

void Aircraft::increaseSpread()
{
}

void Aircraft::collectMissiles(unsigned int count)
{
}

void Aircraft::fire()
{
}

void Aircraft::launchMissile()
{
}

void Aircraft::drawCurrent(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(mSprite, states);
}

void Aircraft::updateCurrent(sf::Time dt, CommandQueue & commands)
{
}

void Aircraft::updateMovementPattern(sf::Time dt)
{
}

void Aircraft::checkPickupDrop(CommandQueue & commands)
{
}

void Aircraft::checkProjectileLaunch(sf::Time dt, CommandQueue & commands)
{
}

void Aircraft::createBullets(SceneNode & node, const TextureHolder & textures) const
{
}

void Aircraft::createProjectile(SceneNode & node, ProjectileIDs::Type type, float xOffset, float yOffset, const TextureHolder & textures) const
{
}

void Aircraft::createPickup(SceneNode & node, const TextureHolder & textures) const
{
}

void Aircraft::updateTexts()
{
}
