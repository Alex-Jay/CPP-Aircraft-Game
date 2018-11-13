#pragma once
#include "SceneNode.hpp"

class Entity : public SceneNode
{
public:
	Entity(int hitpoints);
	void setVelocity(sf::Vector2f velocity);
	void setVelocity(float vx, float vy);
	sf::Vector2f getVelocity() const;
	void accelerate(sf::Vector2f velocity);
	void accelerate(float vx, float vy);

	int getHitpoints() const;
	void repair(int points);
	void damage(int points);
	void destroy();
	virtual bool isDestroyed() const;


private:
	virtual void updateCurrent(sf::Time dt, CommandQueue& commands);

private:
	sf::Vector2f mVelocity;
	int mHitpoints;
};