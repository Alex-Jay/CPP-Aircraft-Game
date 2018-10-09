#pragma once
//Forward declaration of SFML classes
namespace sf
{
	class Texture;
}

enum class TextureIDs{Eagle, Raptor, Desert};

//Forward declaration and typedef
template <typename Resource, typename Identifier>
class ResourceHolder;

typedef ResourceHolder<sf::Texture, TextureIDs> TextureHolder;
