#pragma once
#include "Entity.hpp"
#include "ResourceIdentifiers.hpp"
#include "SFML/Graphics/Sprite.hpp"

class Aircraft : public Entity
{
public:
	enum class Type{Eagle, Raptor};

public:
	Aircraft(Type type, const TextureHolder& texture);
	virtual unsigned int getCategory() const;

private:
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	Type mType;
	sf::Sprite mSprite;
};