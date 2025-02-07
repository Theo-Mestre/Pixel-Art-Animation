#include <iostream>

#include "SFML/Graphics.hpp"

#include "Button.h"

namespace UI
{
	Button::Button()
		: UIElement()
		, m_texture(nullptr)
		, m_callback(nullptr)
		, m_color(sf::Color::White)
		, m_hoverdColorDelta(sf::Color(25, 25, 25, 0))
		, m_clickedColorDelta(sf::Color(50, 50, 50, 0))
	{

	}

	Button::~Button()
	{
		m_callback = nullptr;
		m_texture = nullptr;
	}

	void Button::initialize()
	{
		UpdateVertexColors();
	}

	void Button::receiveEvent(const sf::Event& _event)
	{
		// Check hover state
		if (_event.type == sf::Event::MouseMoved)
		{
			Vec2 mousePos = Vec2(_event.mouseMove.x, _event.mouseMove.y);
			m_hovered = contains(mousePos);
			UpdateVertexColors();
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
		UpdateVertexColors();

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

	void Button::setColor(const sf::Color& _color)
	{
		m_color = _color;
		UpdateVertexColors();
	}

	void Button::setHoveredColorDelta(const sf::Color& _color)
	{
		m_hoverdColorDelta = _color;
	}

	void Button::setClickedColorDelta(const sf::Color& _color)
	{
		m_clickedColorDelta = _color;
	}

	sf::Color Button::getColor() const
	{
		return m_color;
	}

	sf::Color Button::getHoveredColorDelta() const
	{
		return m_hoverdColorDelta;
	}

	sf::Color Button::getClickedColorDelta() const
	{
		return m_clickedColorDelta;
	}

	void Button::onSizeChanged()
	{
		m_quad[1].position = Vec2(m_size.x, 0.0f);
		m_quad[2].position = Vec2(m_size.x, m_size.y);
		m_quad[3].position = Vec2(0.0f, m_size.y);
	}

	void Button::UpdateVertexColors()
	{
		sf::Color color = m_color;

		if (m_hovered)
		{
			color -= m_hoverdColorDelta;
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && m_hovered)
		{
			color -= m_clickedColorDelta;
		}

		for (size_t i = 0; i < 4; i++)
		{
			m_quad[i].color = color;
		}
	}

	bool Button::contains(const Vec2& _point) const
	{
		if (m_parent == nullptr)
		{
			sf::FloatRect rect = getTransform().transformRect(m_quad.getBounds());
			return rect.contains(_point);
		}

        sf::FloatRect absoluteRect = getTransform().transformRect(m_quad.getBounds());
        UIElement* parent = m_parent;

        while (parent != nullptr && parent->getParent() != nullptr)
        {
			absoluteRect = parent->getTransform().transformRect(absoluteRect);
			parent = parent->getParent();
        }

        return absoluteRect.contains(_point);
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