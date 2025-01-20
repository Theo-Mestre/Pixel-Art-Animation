#pragma once

#include "UIElement.h"

namespace sf
{
	class Texture;
	class Color;
}

namespace UI
{
	class Image
		: public UIElement
	{
	public:
		Image();
		~Image();

		void initialize() override;
		void receiveEvent(const sf::Event& _event) override;
		void update(float _deltaTime) override;

		void setImage(sf::Texture& _texture);
		void setTextureRect(const sf::IntRect& _rect);
		void setColor(const sf::Color& _color);

	private:
		virtual void draw(sf::RenderTarget& _target, sf::RenderStates _states) const override;

		virtual void updateTextureRect();
	private:
		sf::IntRect m_textureRect;
		sf::Texture* m_texture;
	};
}