#include <SFML/Graphics.hpp>

#include "Label.h"

namespace UI
{
	Label::Label()
		: UIElement()
		, m_text()
		, m_size()
	{
	}

	Label::~Label()
	{
	}

	void Label::initialize()
	{
	}

	void Label::receiveEvent(const sf::Event& _event)
	{
	}

	void Label::update(float _deltaTime)
	{
	}

	void Label::setSize(const Vec2& _size)
	{
		m_size = _size;

		onSizeChanged();
	}

	void Label::setString(const std::string& _text)
	{
		m_text.setString(_text);
	}

	void Label::setFont(const sf::Font& _font)
	{
		m_text.setFont(_font);
	}

	void Label::setTextColor(const sf::Color& _color)
	{
		m_text.setFillColor(_color);
	}

	void Label::setCharacterSize(uint32_t _size)
	{
		m_text.setCharacterSize(_size);
	}

	const Vec2& Label::getSize() const
	{
		return m_size;
	}

	const std::string& Label::getString() const
	{
		return m_text.getString();
	}

	const sf::Color& Label::getColor() const
	{
		return m_text.getFillColor();
	}

	const sf::Color& Label::getBackgroundColor() const
	{
		return m_quad[0].color;
	}

	sf::Text& Label::asText()
	{
		return m_text;
	}

	void Label::draw(sf::RenderTarget& _target, sf::RenderStates _states) const
	{
		_states.transform *= getTransform();
		_target.draw(m_quad, _states);
		_target.draw(m_text, _states);
	}

	void Label::onSizeChanged()
	{
		m_quad[0].position = { 0.0f, 0.0f };
		m_quad[1].position = { m_size.x, 0.0f };
		m_quad[2].position = { m_size.x, m_size.y };
		m_quad[3].position = { 0.0f, m_size.y };

		updateTextureCoords(m_size);
	}

}