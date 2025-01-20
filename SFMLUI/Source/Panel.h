#pragma once

#include <vector>

#include "SFML/Graphics/VertexArray.hpp"

#include "UIElement.h"

namespace UI
{
	using UIElementContainer = std::vector<std::pair<UIElement*, bool>>;

	class Panel
		: public UIElement
	{
	public:
		Panel();
		virtual ~Panel();

		virtual void initialize() override;
		virtual void receiveEvent(const sf::Event& _event) override;
		virtual void update(float _deltaTime) override;

		void addChild(UIElement* _child, bool _hasOwnership = true);
		void removeChild(UIElement* _child);

		void addChildren(const std::initializer_list<std::pair<UIElement*, bool>>& _children);

		UIElementContainer& getChildren();
		const UIElementContainer& getChildren() const;

	private:
		virtual void draw(sf::RenderTarget& _target, sf::RenderStates _states) const override;

		virtual void onSizeChanged() override;
	private:
		UIElementContainer m_children;

	public:
		// Panel shouldn't have a scale, use setSize instead
		void setScale(float _x, float _y) = delete;
		// Panel shouldn't have a scale, use setSize instead
		void setScale(const sf::Vector2f& _scale) = delete;
	};

}