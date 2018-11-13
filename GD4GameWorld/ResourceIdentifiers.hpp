#pragma once
//Forward declaration of SFML classes
namespace sf
{
	class Texture;
	class Font;
}

enum class TextureIDs { Eagle, Raptor, Avenger, Desert, TitleScreen, ButtonNormal, ButtonSelected, ButtonPressed, Bullet, Missile, HealthRefill, MissileRefill, FireSpread, FireRate };
enum class FontIDs { Main };

//Forward declaration and typedef
template <typename Resource, typename Identifier>
class ResourceHolder;

typedef ResourceHolder<sf::Texture, TextureIDs> TextureHolder;
typedef ResourceHolder<sf::Font, FontIDs> FontHolder;