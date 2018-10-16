#pragma once
//Entity/Scene node category, used to dispatch messages
enum class Category
{
	None = 0,
	Scene = 1 << 0,
	PlayerAircraft = 1 << 1,
	AlliedAircraft = 1 << 2,
	EnemyAircraft = 1 << 3
};