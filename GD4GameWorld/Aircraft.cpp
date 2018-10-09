#include "Aircraft.hpp"
#include "ResourceHolder.hpp"

#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/RenderStates.hpp"

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

Aircraft::Aircraft(Type type, const TextureHolder & textures)
	:mType(type), mSprite(textures.get(toTextureID(type)))
{
	sf::FloatRect bounds = mSprite.getLocalBounds();
	mSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

void Aircraft::drawCurrent(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(mSprite, states);
}
