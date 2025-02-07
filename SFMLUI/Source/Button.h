#pragma once

#include <functional>

#include "SFML/Graphics/VertexArray.hpp"

#include "UIElement.h"

namespace UI
{
	class Button
		: public UIElement
	{
	public:
		Button();
		virtual ~Button();

		virtual void initialize() override;
		virtual void update(float _deltaTime) override;
		virtual void receiveEvent(const sf::Event& _event) override;

		void setCallback(std::function<void()> _callback);
		void setTexture(sf::Texture* _texture);
		
		void setColor(const sf::Color& _color);
		void setHoveredColorDelta(const sf::Color& _color);
		void setClickedColorDelta(const sf::Color& _color);

		sf::Color getColor() const;
		sf::Color getHoveredColorDelta() const;
		sf::Color getClickedColorDelta() const;

	protected:
		virtual void onSizeChanged() override;

		void UpdateVertexColors();

	private:
		bool contains(const Vec2& _point) const;

		virtual void draw(sf::RenderTarget& _target, sf::RenderStates _states) const override;
	private:
		sf::Texture* m_texture;

		std::function<void()> m_callback;
		sf::Color m_color;
		sf::Color m_hoverdColorDelta;
		sf::Color m_clickedColorDelta;
	};
}