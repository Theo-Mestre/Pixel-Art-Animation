#include <SFML/Graphics.hpp>

#include "PanelRenderer.h"

namespace UI
{
	static sf::Sprite s_sprite;

	PanelRenderer::PanelRenderer()
		: Panel()
		, m_renderTexture()
		, m_clearColor(sf::Color::Transparent)
	{
	}

	PanelRenderer::~PanelRenderer()
	{
	}

	void PanelRenderer::initialize()
	{
		Panel::initialize();
		m_renderTexture.create((uint32_t)m_size.x, (uint32_t)m_size.y);
	}

	void PanelRenderer::receiveEvent(const sf::Event& _event)
	{
		Panel::receiveEvent(_event);
	}

	void PanelRenderer::update(float _deltaTime)
	{
		Panel::update(_deltaTime);
	}

	void PanelRenderer::setClearColor(const sf::Color& _color)
	{
		m_clearColor = _color;
	}

	sf::RenderTarget& PanelRenderer::operator()()
	{
		return m_renderTexture;
	}

	void PanelRenderer::draw(sf::RenderTarget& _target, sf::RenderStates _states) const
	{
		if (!isVisible()) return;

		//TODO Change Draw to avoid Const Cast

		const_cast<sf::RenderTexture&>(m_renderTexture).clear(m_clearColor);

		for (auto& [child, ownership] : m_children)
		{
			if (child->isVisible())
			{
				//m_renderTexture.draw(const_cast<UIElement&>(*child), _states);
			}
		}

		const_cast<sf::RenderTexture&>(m_renderTexture).display();

		const sf::Texture& texture = m_renderTexture.getTexture();
		
		_states.transform *= getTransform();
		_states.texture = &texture;
		_target.draw(m_quad, _states);
	}

	void PanelRenderer::onSizeChanged()
	{
		// m_quad[0] = 0.0f, 0.0f. Never changes so no need to update
		m_quad[1].position = sf::Vector2f(m_size.x, 0.0f);
		m_quad[2].position = sf::Vector2f(m_size.x, m_size.y);
		m_quad[3].position = sf::Vector2f(0.0f, m_size.y);

		m_renderTexture.create((uint32_t)m_size.x, (uint32_t)m_size.y);
	}
}