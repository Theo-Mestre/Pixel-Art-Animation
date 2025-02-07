#pragma once

#include <SFML/Graphics/Color.hpp>

#include "UIElement.h"

namespace sf
{
	class Texture;
}

namespace UI
{
	class Image
		: public UIElement
	{
	public:
		Image();
		virtual ~Image();

		void initialize() override;
		void receiveEvent(const sf::Event& _event) override;
		void update(float _deltaTime) override;

		void setTexture(sf::Texture* _texture);
		void setTextureRect(const sf::IntRect& _rect);
		void setColor(const sf::Color& _color);
		void setClearColor(const sf::Color& _color);

		const sf::Texture* getTexture() const;
		sf::Texture* getTexture();

	private:
		virtual void draw(sf::RenderTarget& _target, sf::RenderStates _states) const override;

		virtual void onSizeChanged() override;
		virtual void updateTextureRect();
	private:
		sf::IntRect m_textureRect;
		sf::Texture* m_texture;
		sf::Color m_clearColor;
	};
}