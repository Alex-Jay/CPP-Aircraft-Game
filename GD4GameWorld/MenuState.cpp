#include "MenuState.hpp"
#include "Utility.hpp"
#include "ResourceHolder.hpp"

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/View.hpp"

MenuState::MenuState(StateStack & stack, Context context)
	:State(stack, context)
	, mOptions()
	, mOptionIndex(0)
{
	sf::Texture& texture = context.textures->get(TextureIDs::TitleScreen);
	sf::Font& font = context.fonts->get(FontIDs::Main);

	mBackgroundSprite.setTexture(texture);

	// A simple menu system
	// Play option
	sf::Text playOption;
	playOption.setFont(font);
	playOption.setString("Play");
	centerOrigin(playOption);
	playOption.setPosition(context.window->getView().getSize() / 2.f);
	mOptions.push_back(playOption);

	// Exit option
	sf::Text exitOption;
	exitOption.setFont(font);
	exitOption.setString("Exit");
	centerOrigin(exitOption);
	exitOption.setPosition(playOption.getPosition() + sf::Vector2f(0.f, 30.f)); // Exit is relative to playOption
	mOptions.push_back(exitOption);

	updateOptionText();
}

void MenuState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());
	window.draw(mBackgroundSprite);
	// Iterate options and draw them accordingly
	for (const sf::Text& text : mOptions)
	{
		window.draw(text);
	}
}

bool MenuState::update(sf::Time dt)
{
	return true;
}

bool MenuState::handleEvent(const sf::Event & event)
{
	// Menu logic
	if (event.type != sf::Event::KeyPressed)
	{
		return false;
	}
	if (event.key.code == sf::Keyboard::Return)
	{
		if (mOptionIndex == static_cast<int>(OptionIDs::Play))
		{
			requestStackPop(); // Exit menu state
			requestStackPush(StateIDs::Game); // Enter game state
		}
		else if (mOptionIndex == static_cast<int>(OptionIDs::Exit))
		{
			requestStackPop();
		}
	}
	else if (event.key.code == sf::Keyboard::Up)
	{
		// Decrement mOptionIndex & wrap-around option index
		if (mOptionIndex > 0)
		{
			mOptionIndex--;
		}
		else
		{
			mOptionIndex = mOptions.size() - 1; // Wrap-around
		}
		// Update styling of selected option
		updateOptionText();
	}
	else if (event.key.code == sf::Keyboard::Down)
	{
		// Increment mOptionIndex & wrap-around option index
		if (mOptionIndex < mOptions.size() - 1)
		{
			mOptionIndex++;
		}
		else
		{
			mOptionIndex = 0; // Wrap-around
		}
		// Update styling of selected option
		updateOptionText();
	}
	return true;
}

void MenuState::updateOptionText()
{
	if (mOptions.empty())
	{
		return;
	}

	// White all the texts
	for (sf::Text& text : mOptions)
	{
		text.setOutlineColor(sf::Color::Black);
		text.setOutlineThickness(5.f);
		text.setFillColor(sf::Color::White);
	}

	// Red the selected text
	mOptions[mOptionIndex].setFillColor(sf::Color::Red);
}