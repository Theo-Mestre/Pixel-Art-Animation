#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Glsl.hpp>

#include "Image.h"

namespace UI
{
	static sf::Shader s_shader;
	static const char* s_shaderVertName = "Shaders/Image.vert";
	static const char* s_shaderFragName = "Shaders/Image.frag";

	Image::Image()
		: UIElement()
		, m_texture(nullptr)
		, m_textureRect()
		, m_clearColor(sf::Color::Transparent)
	{
	} 

	Image::~Image()
	{
		if (m_texture) return;

		delete m_texture;
		m_texture = nullptr;
	}

	void Image::initialize()
	{
		if (s_shader.getNativeHandle() != 0) return;

		if (!s_shader.loadFromFile(s_shaderVertName, s_shaderFragName))
		{
			std::cout << "Failed to load shader" << std::endl;
		}
	}

	void Image::receiveEvent(const sf::Event& _event)
	{
		if (!isReceivingEvents()) return;

		handleEventModules(_event);
	}

	void Image::update(float _deltaTime)
	{
		if (!isVisible()) return;

		updateModules(_deltaTime);
	}

	void Image::setTexture(sf::Texture* _texture)
	{
		if (!_texture) return;

		if (m_texture)
		{
			delete m_texture;
		}

		m_texture = _texture;

		setTextureRect(sf::IntRect(0, 0, m_texture->getSize().x, m_texture->getSize().y));
	}

	void Image::setTextureRect(const sf::IntRect& _rect)
	{
		m_textureRect = _rect;

		updateTextureRect();
	}

	void Image::setColor(const sf::Color& _color)
	{
		for (int i = 0; i < 4; i++)
		{
			m_quad[i].color = _color;
		}
	}

	void Image::setClearColor(const sf::Color& _color)
	{
		m_clearColor = _color;
	}

	const sf::Texture* Image::getTexture() const
	{
		return m_texture;
	}

	sf::Texture* Image::getTexture()
	{
		return m_texture;
	}

	void Image::draw(sf::RenderTarget& _target, sf::RenderStates _states) const
	{
		if (!isVisible()) return;

		// Check texture state
		if (m_texture)
		{
			_states.texture = m_texture;
		}

		// Check shader state
		if (s_shader.getNativeHandle() != 0)
		{
			s_shader.setUniform("u_texture", sf::Shader::CurrentTexture);
			s_shader.setUniform("u_clearColor", sf::Glsl::Vec4(m_clearColor));
			_states.shader = &s_shader;
		}

		_states.transform *= getTransform();
		_target.draw(m_quad, _states);

		drawModules(_target, _states);
	}

	void Image::onSizeChanged()
	{
		m_quad[0].position = Vec2(0.0f, 0.0f);
		m_quad[1].position = Vec2(m_size.x, 0.0f);
		m_quad[2].position = m_size;
		m_quad[3].position = Vec2(0.0f, m_size.y);
	}

	void Image::updateTextureRect()
	{
		m_quad[0].texCoords = Vec2(m_textureRect.left, m_textureRect.top);
		m_quad[1].texCoords = Vec2(m_textureRect.left + m_textureRect.width, m_textureRect.top);
		m_quad[2].texCoords = Vec2(m_textureRect.left + m_textureRect.width, m_textureRect.top + m_textureRect.height);
		m_quad[3].texCoords = Vec2(m_textureRect.left, m_textureRect.top + m_textureRect.height);
	}
}