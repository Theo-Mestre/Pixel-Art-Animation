#pragma once

#include <functional>

#include <SFML/Graphics/VertexArray.hpp>
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

		void setSelectedPosition(const Vec2& _position);
		const Vec2& getSelectedPosition() const;
		void setSelectedScreenPosition(const Vec2& _position);
		const Vec2& getSelectedScreenPosition() const;

		void setSelectionCallback(std::function<void()> _callback);
		void setSelectionPixelSize(const Vec2u& _scale);
		void setSelectorSize(const Vec2& _size);

		Vec2i getSelectorPositionInPixelSpace();
	private:
		void draw(sf::RenderTarget& _target, sf::RenderStates _states) const override;

		bool isMouseInsidePickingZone(const Vec2& _mousePos) const;
		void UpdateVertexPositions();
		void snapToPixelPosition();
	private:
		PickingZone m_pickingZone;
		Vec2 m_selectedPosition;
		Vec2 m_selectorSize;
		Vec2u m_selectionPixelSize;

		bool m_isSelected = false;
		bool m_isHovered = false;

		std::function<void()> m_selectionCallback;

		sf::VertexArray m_vertices;
	};

}