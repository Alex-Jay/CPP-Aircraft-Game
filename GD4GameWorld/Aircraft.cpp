#include "Aircraft.hpp"
#include "ResourceHolder.hpp"
#include "DataTables.hpp"
#include "Projectile.hpp"
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
		createProjectile(node, Projectile::ProjectileIDs::Missle, 0.f, 0.5f, textures);
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
	if (mSpreadLevel < MAX_SPREAD_LEVEL)
	{
		++mSpreadLevel;
	}
}

void Aircraft::collectMissiles(unsigned int count)
{
	mMissileAmmo += count;
}

void Aircraft::fire()
{
	// Only aircraft with fire interval != 0 and able to fire
	if (Table[static_cast<int>(mType)].fireInterval != sf::Time::Zero)
	{
		mIsFiring = true;
	}
}

void Aircraft::launchMissile()
{
	if (mMissileAmmo > 0)
	{
		mIsLaunchingMissile = true;
		--mMissileAmmo;
	}
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
	// Enemy aircraft movement pattern
	const std::vector<Direction>& directions = Table[static_cast<int>(mType)].directions;
	if (!directions.empty())
	{
		if (mTravelledDistance > directions[mDirectionIndex].distance)
		{
			// Repeat the movement
			mDirectionIndex = (mDirectionIndex + 1) % directions.size();
			mTravelledDistance = 0;
		}

		// Compute the velocity from the direction
		float radians = toRadians(directions[mDirectionIndex].angle + 90.f);
		float vx = getMaxSpeed() * std::cos(radians);
		float vy = getMaxSpeed() * std::sin(radians);

		setVelocity(vx, vy);
		mTravelledDistance += getMaxSpeed() * dt.asSeconds();
	}
}

void Aircraft::checkPickupDrop(CommandQueue & commands)
{
	if (!isAllied() && randomInt(3) == 0)
	{
		commands.push(mDropPickupCommand);
	}
}

void Aircraft::checkProjectileLaunch(sf::Time dt, CommandQueue & commands)
{
	if (!isAllied())
	{
		fire();
	}
	if (mIsFiring && mFireCountdown <= sf::Time::Zero)
	{
		// Cooldown interval expired, can fire again
		commands.push(mFireCommand);
		mFireCountdown += Table[static_cast<int>(mType)].fireInterval / (mFireRateLevel + 1.f);
		mIsFiring = false;
	}

	// Check for missile launch
	if (mIsLaunchingMissile)
	{
		commands.push(mMissileCommand);
		mIsLaunchingMissile = false;
	}
}

void Aircraft::createBullets(SceneNode & node, const TextureHolder & textures) const
{
	Projectile::Projectile::ProjectileIDs type = isAllied() ? Projectile::Projectile::ProjectileIDs::AlliedBullet : Projectile::Projectile::ProjectileIDs::EnemyBullet;
	switch (mSpreadLevel)
	{
	case 1:
		createProjectile(node, type, 0.0f, 0.5f, textures);
		break;
	case 2:
		createProjectile(node, type, -0.33f, 0.33f, textures);
		createProjectile(node, type, 0.33f, 0.33f, textures);
		break;
	case 3:
		createProjectile(node, type, 0.0f, 0.5f, textures);
		createProjectile(node, type, -0.33f, 0.33f, textures);
		createProjectile(node, type, 0.33f, 0.33f, textures);
		break;
	}
}

void Aircraft::createProjectile(SceneNode & node, Projectile::Projectile::ProjectileIDs type, float xOffset, float yOffset, const TextureHolder & textures) const
{
	std::unique_ptr<Projectile> projectile(new Projectile(type, textures));
	sf::Vector2f offset(xOffset * mSprite.getGlobalBounds().width, yOffset * mSprite.getGlobalBounds().height);
	sf::Vector2f velocity(0, projectile->getMaxSpeed());

	/* Direction of the bullet 
		-1.f = Upwards - Player / Allied
		1.f = Downwards - Enemy
	*/
	float sign = isAllied() ? -1.f : 1.f;

	projectile->setPosition(getWorldPosition() + offset * sign);
	projectile->setVelocity(velocity * sign);
	node.attachChild(std::move(projectile));
}

void Aircraft::createPickup(SceneNode & node, const TextureHolder & textures) const
{
	// Select a random int in the range of PickupIDs, static_cast that type
	auto type = static_cast<int>(PickupIDs::Type)(randomInt(PickupIDs::TypeCount));
	std::unique_ptr<Pickup> pickup(new Pickup(type, textures));
	pickup.resetPosition(getWorldPosition());
	pickup.velocity(0.f, 0.f);
	node.attachChild(std::move(pickup));
}

void Aircraft::updateTexts()
{
	mHealthDisplay->setString(getHitpoints() + " HP");
	mHealthDisplay->setPosition(0.f, 50.f);
	mHealthDisplay->setRotation(-getRotation());

	if (mMissileDisplay)
	{
		if (mMissileAmmo == 0)
		{
			mMissileDisplay->setString("");
		}
		else
		{
			mMissileDisplay->setString("M: " + mMissileAmmo);
		}
	}
}
