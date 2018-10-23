#pragma once
//Forward declaration of SFML classes
namespace sf
{
	class Texture;
	class Font;
}

enum class TextureIDs { Eagle, Raptor, Desert, TitleScreen };
enum class FontIDs { Main };

//Forward declaration and typedef
template <typename Resource, typename Identifier>
class ResourceHolder;

typedef ResourceHolder<sf::Texture, TextureIDs> TextureHolder;
typedef ResourceHolder<sf::Font, FontIDs> FontHolder;