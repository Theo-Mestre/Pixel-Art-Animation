#pragma once

#include <SFML/Graphics/Text.hpp>

#include "UIElement.h"

namespace sf
{
	class Font;
}

namespace UI
{
	class Label
		: public UIElement
	{
	public:
		Label();
		virtual ~Label();

		virtual void initialize() override;
		virtual void receiveEvent(const sf::Event& _event) override;
		virtual void update(float _deltaTime) override;

		void setSize(const Vec2& _size);
		void setString(const std::string& _text);
		void setFont(const sf::Font& _font);
		void setTextColor(const sf::Color& _color);
		void setCharacterSize(uint32_t _size);

		const Vec2& getSize() const;
		const std::string& getString() const;
		const sf::Color& getColor() const;
		const sf::Color& getBackgroundColor() const;

		sf::Text& asText();
	private:
		virtual void draw(sf::RenderTarget& _target, sf::RenderStates _states) const override;

		virtual void onSizeChanged() override;
	private:
		sf::Text m_text;
		UI::Vec2 m_size;
	};

}