#include "SFML/Graphics.hpp"

#include "Panel.h"

namespace UI
{
	Panel::Panel()
		: UIElement()
		, m_children()
	{
	}

	Panel::~Panel()
	{
		for (int i = m_children.size() - 1; i >= 0; i--)
		{
			auto [child, hasOwnership] = m_children[i];

			if (hasOwnership)
			{
				delete child;
			}
		}

		m_children.clear();
	}

	void Panel::initialize()
	{
	}

	void Panel::receiveEvent(const sf::Event& _event)
	{
		for (auto& [child, ownership] : m_children)
		{
			if (child->isReceivingEvents())
			{
				child->receiveEvent(_event);
			}
		}

		handleEventModules(_event);
	}

	void Panel::update(float _deltaTime)
	{
		for (auto& [child, ownership] : m_children)
		{
			if (child->isVisible())
			{
				child->update(_deltaTime);
			}
		}

		updateModules(_deltaTime);
	}

	void Panel::addChild(UIElement* _child, bool _hasOwnership)
	{
		m_children.push_back(std::make_pair(_child, _hasOwnership));
	}

	void Panel::removeChild(UIElement* _child)
	{
		// Find the corresponding child
		auto it = std::find_if(m_children.begin(), m_children.end(),
			[_child](const std::pair<UIElement*, bool>& _pair)
			{
				return _pair.first == _child;
			});

		if (it == m_children.end()) return;

		// Delete the child if the panel has ownership
		if (it->second)
		{
			delete it->first;
		}
		m_children.erase(it);
	}

	void Panel::addChildren(const std::initializer_list<std::pair<UIElement*, bool>>& _children)
	{
		m_children.insert(m_children.end(), _children);
	}

	UIElementContainer& Panel::getChildren()
	{
		return m_children;
	}

	const UIElementContainer& Panel::getChildren() const
	{
		return m_children;
	}

	void Panel::draw(sf::RenderTarget& _target, sf::RenderStates _states) const
	{
		if (!isVisible()) return;
		
		_states.transform *= getTransform();
		_target.draw(m_quad, _states);

		for (auto& [child, ownership] : m_children)
		{
			if (child->isVisible())
			{
				_target.draw(*child, _states);
			}
		}

		drawModules(_target, _states);
	}
	
	void Panel::onSizeChanged()
	{
		m_quad[1].position = sf::Vector2f(m_size.x, 0.0f);
		m_quad[2].position = sf::Vector2f(m_size.x, m_size.y);
		m_quad[3].position = sf::Vector2f(0.0f, m_size.y);
	}
}