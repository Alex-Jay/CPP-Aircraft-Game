#include "DataTables.hpp"
#include "Aircraft.hpp"
#include "Projectile.hpp"
#include "Pickup.hpp"
#include "Constants.hpp"

std::vector<AircraftData> initializedAircraftData()
{
	std::vector<AircraftData> data(static_cast<int>(Aircraft::Type::TypeCount));

	// Eagle Aircraft
	data[static_cast<int>(Aircraft::Type::Eagle)].hitPoints = 100;
	data[static_cast<int>(Aircraft::Type::Eagle)].speed = 200.f;
	data[static_cast<int>(Aircraft::Type::Eagle)].fireInterval = sf::seconds(1);
	data[static_cast<int>(Aircraft::Type::Eagle)].texture = TextureIDs::Eagle;

	// Raptor Aircraft
	data[static_cast<int>(Aircraft::Type::Raptor)].hitPoints = 20;
	data[static_cast<int>(Aircraft::Type::Raptor)].speed = 80.f;
	data[static_cast<int>(Aircraft::Type::Raptor)].fireInterval = sf::seconds(0);
	data[static_cast<int>(Aircraft::Type::Raptor)].texture = TextureIDs::Raptor;
	data[static_cast<int>(Aircraft::Type::Raptor)].directions.push_back(Direction(+45.f, 80.f));
	data[static_cast<int>(Aircraft::Type::Raptor)].directions.push_back(Direction(-45.f, 160.f));
	data[static_cast<int>(Aircraft::Type::Raptor)].directions.push_back(Direction(+45.f, 80.f));

	// Raptor Aircraft
	data[static_cast<int>(Aircraft::Type::Avenger)].hitPoints = 40;
	data[static_cast<int>(Aircraft::Type::Avenger)].speed = 50.f;
	data[static_cast<int>(Aircraft::Type::Avenger)].fireInterval = sf::seconds(2);
	data[static_cast<int>(Aircraft::Type::Avenger)].texture = TextureIDs::Avenger;
	data[static_cast<int>(Aircraft::Type::Avenger)].directions.push_back(Direction(+45.f, 50.f));
	data[static_cast<int>(Aircraft::Type::Avenger)].directions.push_back(Direction(0.f, 50.f));
	data[static_cast<int>(Aircraft::Type::Avenger)].directions.push_back(Direction(-45.f, 100.f));
	data[static_cast<int>(Aircraft::Type::Avenger)].directions.push_back(Direction(0.f, 50.f));
	data[static_cast<int>(Aircraft::Type::Avenger)].directions.push_back(Direction(+45.f, 50.f));

	return data;
}

std::vector<ProjectileData> initializedProjectileData()
{
	std::vector<ProjectileData> data(static_cast<int>(ProjectileIDs::TypeCount));
	
	data[static_cast<int>(ProjectileIDs::AlliedBullet)].damage = 10;
	data[static_cast<int>(ProjectileIDs::AlliedBullet)].speed = 300.f;
	data[static_cast<int>(ProjectileIDs::AlliedBullet)].texture = TextureIDs::Bullet;

	data[static_cast<int>(ProjectileIDs::EnemyBullet)].damage = 10;
	data[static_cast<int>(ProjectileIDs::EnemyBullet)].speed = 300.f;
	data[static_cast<int>(ProjectileIDs::EnemyBullet)].texture = TextureIDs::Bullet;

	data[static_cast<int>(ProjectileIDs::Missile)].damage = 200;
	data[static_cast<int>(ProjectileIDs::Missile)].speed = 150.f;
	data[static_cast<int>(ProjectileIDs::Missile)].texture = TextureIDs::Missile;

	return data;
}

std::vector<PickupData> initializedPickupData()
{
	std::vector<PickupData> data(static_cast<int>(PickupIDs::TypeCount));

	data[static_cast<int>(PickupIDs::HealthRefill)].texture = TextureIDs::HealthRefill;
	data[static_cast<int>(PickupIDs::HealthRefill)].action = [](Aircraft& a)
	{
		a.repair(HEALTH_PICKUP_REPAIR_VALUE);
	};

	data[static_cast<int>(PickupIDs::MissileRefill)].texture = TextureIDs::MissileRefill;
	data[static_cast<int>(PickupIDs::MissileRefill)].action = std::bind(&Aircraft::collectMissiles, _1, MISSILE_PICKUP_AMMO_VALUE);

	data[static_cast<int>(PickupIDs::FireSpread)].texture = TextureIDs::FireSpread;
	data[static_cast<int>(PickupIDs::FireSpread)].action = std::bind(&Aircraft::increaseSpread);

	data[static_cast<int>(PickupIDs::FireRate)].texture = TextureIDs::FireRate;
	data[static_cast<int>(PickupIDs::FireRate)].action = std::bind(&Aircraft::increaseFireRate);

	return data;
}
