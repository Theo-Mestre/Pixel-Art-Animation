#include <iostream>

#include "SFML/Graphics.hpp"

#include "Button.h"

namespace UI
{
	Button::Button()
		: UIElement()
		, m_texture(nullptr)
		, m_callback(nullptr)
	{

	}

	Button::~Button()
	{
		m_callback = nullptr;
		m_texture = nullptr;
	}

	void Button::initialize()
	{
	}

	void Button::receiveEvent(const sf::Event& _event)
	{
		// Check hover state
		if (_event.type == sf::Event::MouseMoved)
		{
			Vec2 mousePos = Vec2(_event.mouseMove.x, _event.mouseMove.y);
			m_hovered = contains(mousePos);
		}

		// return if the event isn't a mouse button press
		if (_event.type != sf::Event::MouseButtonPressed) return;

		// Check if the button was pressed
		if (_event.mouseButton.button == sf::Mouse::Left)
		{
			if (m_hovered && m_callback != nullptr)
			{
				m_callback();
			}
		}

		handleEventModules(_event);
	}

	void Button::update(float _deltaTime)
	{
		updateModules(_deltaTime);
	}

	void Button::setCallback(std::function<void()> _callback)
	{
		m_callback = _callback;
	}

	void Button::setTexture(sf::Texture* _texture)
	{
		if (_texture == nullptr) return;

		m_texture = _texture;

		updateTextureCoords((Vec2)m_texture->getSize());
	}

	void Button::onSizeChanged()
	{
		m_quad[1].position = Vec2(m_size.x, 0.0f);
		m_quad[2].position = Vec2(m_size.x, m_size.y);
		m_quad[3].position = Vec2(0.0f, m_size.y);
	}

	bool Button::contains(const Vec2& _point) const
	{
		sf::FloatRect rect = getTransform().transformRect(m_quad.getBounds());
		return rect.contains(_point);
	}

	void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (isVisible() == false) return;

		if (m_texture != nullptr)
		{
			states.texture = m_texture;
		}

		states.transform *= getTransform();
		target.draw(m_quad, states);

		drawModules(target, states);
	}
}