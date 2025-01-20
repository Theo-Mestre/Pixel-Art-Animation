#include <SFML/Graphics.hpp>

#include "Image.h"

namespace UI
{
	Image::Image()
		: UIElement()
		, m_texture(nullptr)
		, m_textureRect()
	{
	}

	Image::~Image()
	{
		m_texture = nullptr;
	}

	void Image::initialize()
	{
	}

	void Image::receiveEvent(const sf::Event& _event)
	{
		if (!isReceivingEvents()) return;
	}

	void Image::update(float _deltaTime)
	{
		if (!isVisible()) return;
	}

	void Image::setImage(sf::Texture& _texture)
	{
		m_texture = &_texture;
	}

	void Image::setTextureRect(const sf::IntRect& _rect)
	{
		m_textureRect = _rect;
	}

	void Image::setColor(const sf::Color& _color)
	{
		for (int i = 0; i < 4; i++)
		{
			m_quad[i].color = _color;
		}
	}

	void Image::draw(sf::RenderTarget& _target, sf::RenderStates _states) const
	{
		if (!isVisible()) return;

		if (m_texture)
		{
			_states.texture = m_texture;
		}

		_states.transform *= getTransform();
		_target.draw(m_quad, _states);
	}
	void Image::updateTextureRect()
	{
		m_quad[0].texCoords = sf::Vector2f(m_textureRect.left, m_textureRect.top);
		m_quad[1].texCoords = sf::Vector2f(m_textureRect.left + m_textureRect.width, m_textureRect.top);
		m_quad[2].texCoords = sf::Vector2f(m_textureRect.left + m_textureRect.width, m_textureRect.top + m_textureRect.height);
		m_quad[3].texCoords = sf::Vector2f(m_textureRect.left, m_textureRect.top + m_textureRect.height);
	}
}