#pragma once

#include <functional>

#include <SFML/Graphics/Rect.hpp>

#include "Modules.h"
#include "Vectors.h"

namespace UI
{
	class MousePickerModule
		: public Module
	{
	public:
		MousePickerModule();
		virtual ~MousePickerModule() = default;

		virtual void receiveEvent(const sf::Event& _event) override;
		virtual void update(float _deltaTime) override;

		void setPickingZone(const PickingZone& _zone);
		const PickingZone& getPickingZone() const;

		void setSelected(bool _selected);
		bool isSelected() const;

		const Vec2& getSelectedPosition() const;
		const Vec2& getSelectedScreenPosition() const;

		void SetSelectionCallback(std::function<void()> _callback);
	private:
		void draw(sf::RenderTarget& _target, sf::RenderStates _states) const override;

		bool isMouseInsidePickingZone(const Vec2& _mousePos) const;

	private:
		PickingZone m_pickingZone;
		Vec2 m_selectedPosition;

		bool m_isSelected = false;

		std::function<void()> m_selectionCallback;
	};

}