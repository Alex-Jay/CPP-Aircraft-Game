#include "Aircraft.hpp"
#include "ResourceHolder.hpp"
#include "DataTables.hpp"
#include "Pickup.hpp"
#include "TextNode.hpp"
#include "CommandQueue.hpp"
#include "Constants.hpp"
#include "Utility.hpp"

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

	centreOrigin(mSprite);

	// Create bullets/projectiles on SceneAirLayer
	mFireCommand.category = static_cast<int>(Category::SceneAirLayer);
	mFireCommand.action = [this, &textures](SceneNode& node, sf::Time)
	{
		createBullets(node, textures);
	};

	mMissileCommand.category = static_cast<int>(Category::SceneAirLayer);
	mMissileCommand.action = [this, &textures](SceneNode& node, sf::Time)
	{
		// Offset top position projectile in-front of the plane
		createProjectile(node, ProjectileIDs::Missle, 0.f, 0.5f, textures);
	};

	mDropPickupCommand.category = static_cast<int>(Category::SceneAirLayer);
	mDropPickupCommand.action = [this, &textures](SceneNode& node, sf::Time)
	{
		createPickup(node, textures);
	};

	std::unique_ptr<TextNode> healthDisplay(new TextNode(fonts, ""));
	mHealthDisplay = healthDisplay.get();
	attachChild(std::move(healthDisplay));

	if (getCategory() == static_cast<int>(Category::PlayerAircraft))
	{
		std::unique_ptr<TextNode> missileDisplay(new TextNode(fonts, ""));
		missileDisplay->setPosition(0.f, 70.f);
		mMissileDisplay = missileDisplay.get();
		attachChild(std::move(missileDisplay));
	}

	updateTexts();
}

unsigned int Aircraft::getCategory() const
{
	if (isAllied())
		return static_cast<int>(Category::PlayerAircraft);
	else
		return static_cast<int>(Category::EnemyAircraft);
}

sf::FloatRect Aircraft::getBoundingRectangle() const
{
	return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

bool Aircraft::isMarkedForRemoval() const
{
	return mIsMarkedForRemoval;
}

bool Aircraft::isAllied() const
{
	return mType == Aircraft::Type::Eagle;
}

float Aircraft::getMaxSpeed() const
{
	return Table[static_cast<int>(mType)].speed;
}

void Aircraft::increaseFireRate()
{
	if (mFireRateLevel < MAX_FIRE_RATE_LEVEL)
	{
		++mFireRateLevel;
	}
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
	// Entity has been destroyed, drop pickup, mark fro removal
	if (isDestroyed())
	{
		checkPickupDrop(commands);
		mIsMarkedForRemoval = true;
		return;
	}
	// Check if bullets or missiles are fired
	checkProjectileLaunch(dt, commands);

	// Update enemy movement pattern
	updateMovementPattern(dt);
	Entity::updateCurrent(dt, commands);

	updateTexts();
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
