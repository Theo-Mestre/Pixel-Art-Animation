#include "SFML/Graphics.hpp"

#include "Modules.h"
#include "UIElement.h"

namespace UI
{
	void UIModule::receiveEvent(const sf::Event& _event)
	{
	}

	TextModule::TextModule()
		: UIModule()
	{
		setColor(sf::Color::Black);
	}

	TextModule::TextModule(const std::string& _text)
		: TextModule()
	{
		setText(_text);
	}

	void TextModule::setText(const std::string& text)
	{
		m_text.setString(text);
	}

	void TextModule::setFont(sf::Font* font)
	{
		m_text.setFont(*font);
	}

	void TextModule::setColor(const sf::Color& color)
	{
		m_text.setFillColor(color);
	}

	void TextModule::setTextCentered()
	{
		m_text.setOrigin(m_text.getLocalBounds().width / 2.0f, m_text.getLocalBounds().height / 1.25f);
		m_text.setPosition(m_owner->getSize() / 2.0f);
	}

	void TextModule::update(float _deltaTime)
	{
	}

	sf::Text& TextModule::operator()()
	{
		return m_text;
	}

	void TextModule::draw(sf::RenderTarget& _target, sf::RenderStates _states) const
	{
		_states.transform *= getTransform();
		_target.draw(m_text, _states);
	}
}