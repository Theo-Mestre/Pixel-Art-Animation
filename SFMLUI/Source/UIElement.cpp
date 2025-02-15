#include <SFML/Graphics.hpp>

#include "UIElement.h"
#include "Modules.h"

namespace UI
{
	UIElement::UIElement()
		: m_quad(sf::Quads, 4)
		, m_size()
		, m_visible(true)
		, m_receiveEvents(true)
		, m_hovered(false)
		, m_parent(nullptr)
	{
	}

	UIElement::~UIElement()
	{
		m_quad.clear();

		for (int i = m_modules.size() - 1; i >= 0; i--)
		{
			delete m_modules[i];
		}

		m_modules.clear();
	}

	void UIElement::setSize(const sf::Vector2f& _size)
	{
		m_size = _size;
		
		onSizeChanged();
	}

	const sf::Vector2f& UIElement::getSize() const
	{
		return m_size;
	}

	void UIElement::setVisible(bool _visible)
	{
		m_visible = _visible;
	}

	bool UIElement::isVisible() const
	{
		return m_visible;
	}

	void UIElement::setReceiveEvents(bool _event)
	{
		m_receiveEvents = _event;
	}

	bool UIElement::isReceivingEvents() const
	{
		return m_receiveEvents && isVisible();
	}

	void UIElement::setHovered(bool _hovered)
	{
		m_hovered = _hovered;
	}

	bool UIElement::isHovered() const
	{
		return m_hovered;
	}

	void UIElement::setColor(const sf::Color& _color)
	{
		for (int i = 0; i < 4; i++)
		{
			m_quad[i].color = _color;
		}
	}

	void UIElement::setTextureCoords(const sf::FloatRect& _rect)
	{
		m_quad[0].texCoords = Vec2(_rect.left, _rect.top);
		m_quad[1].texCoords = Vec2(_rect.left + _rect.width, _rect.top);
		m_quad[2].texCoords = Vec2(_rect.left + _rect.width, _rect.top + _rect.height);
		m_quad[3].texCoords = Vec2(_rect.left, _rect.top + _rect.height);
	}

	void UIElement::setParent(UIElement* _parent)
	{
		m_parent = _parent;
	}

	UIElement* UIElement::getParent() const
	{
		return m_parent;
	}

	const sf::Transform& UIElement::getParentTransform() const
	{
		if (m_parent == nullptr) return getTransform();

		// return combined transform
		return m_parent->getTransform() * getTransform();
	}

	void UIElement::addModule(Module* _module)
	{
		_module->m_owner = this;
		m_modules.push_back(std::move(_module));
	}

	void UIElement::updateModules(float deltaTime)
	{
		for (auto module : m_modules)
		{
			module->update(deltaTime);
		}
	}

	void UIElement::handleEventModules(const sf::Event& event)
	{
		for (auto module : m_modules)
		{
			module->receiveEvent(event);
		}
	}

	void UIElement::drawModules(sf::RenderTarget& _target, sf::RenderStates _states) const
	{
		for (auto module : m_modules)
		{
			_target.draw(*module, _states);
		}
	}

	void UIElement::removeModule(Module* _module)
	{
		auto it = std::find(m_modules.begin(), m_modules.end(), _module);

		if (it != m_modules.end())
		{
			delete* it;
			m_modules.erase(it);
		}
	}

	void UIElement::removeAllModules()
	{
		for (int i = m_modules.size() - 1; i >= 0; i--)
		{
			delete m_modules[i];
		}

		m_modules.clear();
	}

	void UIElement::onSizeChanged()
	{
	}

	void UIElement::updateTextureCoords(const sf::Vector2f _size)
	{
		m_quad[0].texCoords = Vec2(0.0f, 0.0f);
		m_quad[1].texCoords = Vec2(_size.x, 0.0f);
		m_quad[2].texCoords = _size;
		m_quad[3].texCoords = Vec2(0.0f, _size.y);
	}
}