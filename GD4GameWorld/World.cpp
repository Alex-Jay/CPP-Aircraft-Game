#include "World.hpp"

#include "SFML/Graphics/RenderWindow.hpp"

World::World(sf::RenderWindow & window)
	: mWindow(window)
	, mWorldView(window.getDefaultView())
	, mTextures()
	, mSceneGraph()
	, mSceneLayers()
	, mWorldBounds(0.f, 0.f, mWorldView.getSize().x, 2000.f)
	, mSpawnPosition(mWorldView.getSize().x/2.f, mWorldBounds.height - mWorldView.getSize().y /2.f)
	, mScrollSpeed(-50.f)
	, mPlayerAircraft(nullptr)
{
	loadTextures();
	buildScene();
	mWorldView.setCenter(mSpawnPosition);
}

void World::update(sf::Time dt)
{
	//Scroll the world
	mWorldView.move(0.f, mScrollSpeed * dt.asSeconds());

	//Move the player
	sf::Vector2f position = mPlayerAircraft->getPosition();
	sf::Vector2f velocity = mPlayerAircraft->getVelocity();

	//If the player touches the border, flip its x velocity
	if (position.x <= mWorldBounds.left + 150.f || position.x >= mWorldBounds.left + mWorldBounds.width - 150.f)
	{
		velocity.x = -velocity.x;
		mPlayerAircraft->setVelocity(velocity);
	}

	//apply the movement
	mSceneGraph.update(dt);
}

void World::draw()
{
	mWindow.setView(mWorldView);
	mWindow.draw(mSceneGraph);
}

void World::loadTextures()
{
	mTextures.load(TextureIDs::Eagle, "Media/Textures/Eagle.png");
	mTextures.load(TextureIDs::Raptor, "Media/Textures/Raptor.png");
	mTextures.load(TextureIDs::Desert, "Media/Textures/Desert.png");
}

void World::buildScene()
{
	//Initialize the different layers
	for (int i = 0; i < static_cast<int>(Layer::LayerCount); ++i)
	{
		SceneNode::Ptr layer(new SceneNode());
		mSceneLayers[i] = layer.get();
		mSceneGraph.attachChild(std::move(layer));
	}

	//Prepare the filed background
	sf::Texture& texture = mTextures.get(TextureIDs::Desert);
	sf::IntRect textureRect(mWorldBounds);
	texture.setRepeated(true);

	//Add the background sprite to the scene
	std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(texture, textureRect));
	backgroundSprite->setPosition(mWorldBounds.top, mWorldBounds.left);
	mSceneLayers[static_cast<int>(Layer::Background)]->attachChild(std::move(backgroundSprite));

	//Add player's aircraft
	std::unique_ptr<Aircraft> leader(new Aircraft(Aircraft::Type::Eagle, mTextures));
	mPlayerAircraft = leader.get();
	mPlayerAircraft->setPosition(mSpawnPosition);
	mPlayerAircraft->setVelocity(40.f, mScrollSpeed);
	mSceneLayers[static_cast<int>(Layer::Air)]->attachChild(std::move(leader));

	//Add escort 1
	std::unique_ptr<Aircraft> leftEscort(new Aircraft(Aircraft::Type::Raptor, mTextures));
	leftEscort->setPosition(-80.f, 50.f);
	mPlayerAircraft->attachChild(std::move(leftEscort));

	//Add escort 2
	std::unique_ptr<Aircraft> rightEscort(new Aircraft(Aircraft::Type::Raptor, mTextures));
	rightEscort->setPosition(80.f, 50.f);
	mPlayerAircraft->attachChild(std::move(rightEscort));
}
