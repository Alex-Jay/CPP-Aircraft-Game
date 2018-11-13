#include "World.hpp"

#include "SFML/Graphics/RenderWindow.hpp"

World::World(sf::RenderWindow & window)
	: mWindow(window)
	, mWorldView(window.getDefaultView())
	, mTextures()
	, mSceneGraph()
	, mSceneLayers()
	, mCommandQueue()
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
	mPlayerAircraft->setVelocity(0.f, 0.f);

	//Forward commands to the scene graph
	//Adapt velocity to remove diagonal advantage
	while (!mCommandQueue.isEmpty())
	{
		mSceneGraph.onCommand(mCommandQueue.pop(), dt);
	}
	adaptPlayerVelocity();

	//apply the movement
	mSceneGraph.update(dt);
	adaptPlayerPosition();
}

void World::draw()
{
	mWindow.setView(mWorldView);
	mWindow.draw(mSceneGraph);
}

CommandQueue & World::getCommandQueue()
{
	return mCommandQueue;
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

void World::adaptPlayerPosition()
{
	//Keep player on the screen
	sf::FloatRect viewBounds(mWorldView.getCenter() - mWorldView.getSize() / 2.f, mWorldView.getSize());
	const float borderDistance = 40.f;

	sf::Vector2f position = mPlayerAircraft->getPosition();
	position.x = std::max(position.x, viewBounds.left + borderDistance);
	position.x = std::min(position.x, viewBounds.left + viewBounds.width - borderDistance);
	position.y = std::max(position.y, viewBounds.top + borderDistance);
	position.y = std::min(position.y, viewBounds.top + viewBounds.height - borderDistance);
	mPlayerAircraft->setPosition(position);

}

void World::adaptPlayerVelocity()
{
	sf::Vector2f velocity = mPlayerAircraft->getVelocity();
	//If moving diagonally, reduce velocity by sqrt 2
	if (velocity.x != 0.f && velocity.y != 0.f)
	{
		mPlayerAircraft->setVelocity(velocity / std::sqrt(2.f));
	}
	mPlayerAircraft->accelerate(0.f, mScrollSpeed);
}
