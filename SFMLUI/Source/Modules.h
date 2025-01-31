#pragma once
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

#include "Vectors.h"

namespace sf
{
	class Event;
}

namespace UI
{
	class UIElement;

	class Module
		: public sf::Drawable
		, public sf::Transformable
	{
	public:
		Module() = default;
		virtual ~Module() = default;
		virtual void receiveEvent(const sf::Event& _event);
		virtual void update(float _deltaTime) = 0;

	protected:
		UIElement* m_owner = nullptr;
		friend class UIElement;
	};

	class TextModule
		: public Module
	{
	public:
		TextModule();
		TextModule(const std::string& _text);
		virtual ~TextModule();

		virtual void update(float _deltaTime) override;

		void setText(const std::string& text);
		void setFont(sf::Font* font);
		void setColor(const sf::Color& color);
		void setTextCentered();

		void setSize(const UI::Vec2& _size);

		sf::Text& getText();
		sf::Text& operator()();

	private:
		void draw(sf::RenderTarget& _target, sf::RenderStates _states) const override;

	private:
		sf::Text m_text;

		UI::Vec2 m_size;
	};
}