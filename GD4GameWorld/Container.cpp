#include "Container.hpp"

#include "SFML/Window/Event.hpp"
#include "SFML/Graphics/RenderStates.hpp"
#include "SFML/Graphics/RenderTarget.hpp"

namespace GUI
{
	GUI::Container::Container()
		: mChildren(0), mSelectedChild(-1)
	{
	}

	void GUI::Container::pack(Component::Ptr component)
	{
		mChildren.push_back(component);
		if (!hasSelection() && component->isSelectable())
		{
			select(mChildren.size() - 1);
		}
	}

	bool GUI::Container::isSelectable() const
	{
		return false;
	}

	void GUI::Container::handleEvent(sf::Event & event)
	{
		// If we have selected a child, propogate (pass) the events on to them
		if (hasSelection() && mChildren[mSelectedChild]->isActive())
		{
			mChildren[mSelectedChild]->handleEvent(event);
		}
		else if (event.type == sf::Event::KeyReleased)
		{
			// Pressing up (Select previous)
			if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up)
			{
				selectPrevious();
			}
			// Pressing down (Select next)
			else if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down)
			{
				selectNext();
			}
			// If space or return pressed (Interact)
			else if (event.key.code == sf::Keyboard::Return || event.key.code == sf::Keyboard::Space)
			{
				if (hasSelection())
				{
					mChildren[mSelectedChild]->activate();
				}
			}
		}

	}

	void GUI::Container::draw(sf::RenderTarget & target, sf::RenderStates states) const
	{
		states.transform *= getTransform();

		for (const Component::Ptr& child : mChildren)
		{
			target.draw(*child, states);
		}
	}

	bool GUI::Container::hasSelection() const
	{
		return mSelectedChild >= 0;
	}

	void GUI::Container::select(std::size_t index)
	{
		if (mChildren[index]->isSelectable())
		{
			if (hasSelection())
			{
				mChildren[index]->deselect();
			}

			mChildren[index]->select();
			mSelectedChild = index;
		}
	}

	void GUI::Container::selectNext()
	{
		if (!hasSelection())
		{
			return;
		}
		// Search for the next selectable component, wrap-around is necessary
		int next = mSelectedChild;
		do
		{
			next = (next + 1) % mChildren.size(); // Modulus allows for wrap around, E.g. if 4 buttons, 4 % 4 = 0, so wrapped around
		} while (!mChildren[next]->isSelectable());

		// Select the component
		select(next);
	}

	void GUI::Container::selectPrevious()
	{
		if (!hasSelection())
		{
			return;
		}
		// Search for the previous selectable component, wrap-around is necessary
		int previous = mSelectedChild;
		do
		{
			previous = (previous + mChildren.size() - 1) % mChildren.size();
		} while (!mChildren[previous]->isSelectable());

		// Select the component
		select(previous);
	}
}