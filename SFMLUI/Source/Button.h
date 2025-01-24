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
		
	protected:
		virtual void onSizeChanged() override;

	private:
		bool contains(const Vec2& _point) const;

		virtual void draw(sf::RenderTarget& _target, sf::RenderStates _states) const override;
	private:
		sf::Texture* m_texture;

		std::function<void()> m_callback;
	};
}