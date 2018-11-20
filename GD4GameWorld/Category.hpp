#pragma once
//Entity/Scene node category, used to dispatch messages
// Enum Class defaults to int (4-bytes), use (enum class X : long int) to default to long int (8-bytes)
enum class Category {
	None = 0,
	SceneAirLayer = 1 << 0,
	PlayerAircraft = 1 << 1,
	AlliedAircraft = 1 << 2,
	EnemyAircraft = 1 << 3,
	Pickup = 1 << 4,
	AlliedProjectile = 1 << 5,
	EnemyProjectile = 1 << 6,

	Aircraft = PlayerAircraft | AlliedAircraft | EnemyAircraft,
	Projectile = AlliedProjectile | EnemyProjectile
};