#include "Projectile.hpp"
#include "DataTables.hpp"
#include "ResourceHolder.hpp"
#include "Utility.hpp"
#include "Constants.hpp"

#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/RenderStates.hpp"

#include <cassert>

namespace
{
	const std::vector<ProjectileData> Table = initializeProjectileData();
}

Projectile::Projectile(Projectile::ProjectileIDs type, const TextureHolder & textures)
	: Entity(1)
	, mType(type)
	, mSprite(textures.get(Table[static_cast<int>(type)].texture))
	, mTargetDirection()
{
	centreOrigin(mSprite);
}

void Projectile::guideTowards(sf::Vector2f position)
{
	assert(isGuided());

	// Create unit vector facing towards enemy
	mTargetDirection = unitVector(position - getWorldPosition());
}

bool Projectile::isGuided() const
{
	return mType == ProjectileIDs::Missile;
}

unsigned int Projectile::getCategory() const
{
	return 0;
}

sf::FloatRect Projectile::getBoundingRect() const
{
	return sf::FloatRect();
}

float Projectile::getMaxSpeed() const
{
	return 0.0f;
}

int Projectile::getDamage() const
{
	return 0;
}

void Projectile::updateCurrent(sf::Time dt, CommandQueue & commands)
{
	if (isGuided())
	{
		const float approachRate = APPROACH_RATE;
		sf::Vector2f newVelocity = unitVector(approachRate * dt.asSeconds() * mTargetDirection + getVelocity());
		newVelocity *= getMaxSpeed();
		// Get inverse tan (No angle, have X & Y | Opposite / Adjacent)
		float angle = std::atan2(newVelocity.y, newVelocity.x);
		setRotation(toDegrees(angle) + 90.f);
		setVelocity(newVelocity);
	}
}

void Projectile::drawCurrent(sf::RenderTarget target, sf::RenderStates states) const
{
}