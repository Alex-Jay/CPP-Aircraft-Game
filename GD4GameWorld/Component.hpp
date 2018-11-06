#pragma once
#include "SFML/System/NonCopyable.hpp"
#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/Transformable.hpp"

#include <memory>

// Forward declaration
namespace sf
{
	class Event;
}

namespace GUI
{
	class Component : public sf::Drawable, public sf::Transformable, private sf::NonCopyable
	{
	public:
		typedef std::shared_ptr<Component> Ptr;
		
	public:
		Component();
		virtual ~Component(); // Destroy upto this component (For inheritance, prevents memory leaks).

		virtual bool isSelectable() const = 0; // Pure virtual - abstract (Similar to interface).
		bool isSelected() const;
		virtual void select();
		virtual void deselect();

		virtual bool isActive() const;
		virtual void activate();
		virtual void deactivate();

		virtual void handleEvent(const sf::Event& event) = 0;

	private:
		bool mIsSelected;
		bool mIsActive;
	};
}