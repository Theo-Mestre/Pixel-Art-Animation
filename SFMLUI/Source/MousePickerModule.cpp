#include <SFML/Graphics.hpp>

#include <iostream>

#include "UIElement.h"
#include "MousePickerModule.h"

namespace UI
{
	static constexpr uint8_t VertexCount = 4;

	MousePickerModule::MousePickerModule()
		: Module()
		, m_pickingZone()
		, m_selectedPosition()
		, m_selectorSize()
		, m_selectionPixelSize()
		, m_isSelected(false)
		, m_selectionCallback(nullptr)
		, m_vertices(sf::Quads, VertexCount)
	{
		for (int i = 0; i < VertexCount; i++)
		{
			m_vertices[i].color = sf::Color::White;
		}
	}

	void MousePickerModule::receiveEvent(const sf::Event& _event)
	{
		if (_event.type == sf::Event::MouseMoved)
		{
			m_isHovered = isMouseInsidePickingZone(Vec2(_event.mouseMove.x, _event.mouseMove.y));
			m_owner->setHovered(m_isHovered);
		}

		if (!m_isHovered) return;

		if (_event.type != sf::Event::MouseButtonPressed) return;

		if (_event.mouseButton.button == sf::Mouse::Right)
		{
			m_isSelected = false;
			m_selectedPosition = Vec2(-1, -1);
			return;
		}

		// if left clic isn't pressed return
		if (_event.mouseButton.button != sf::Mouse::Left) return;

		// enable selection and invoke callback
		m_isSelected = true;
		setSelectedScreenPosition(Vec2(_event.mouseButton.x, _event.mouseButton.y));

		if (m_selectionCallback)
		{
			m_selectionCallback();
		}
	}

	void MousePickerModule::update(float _deltaTime)
	{

	}

	void MousePickerModule::setPickingZone(const PickingZone& _zone)
	{
		m_pickingZone = _zone;
	}

	const PickingZone& MousePickerModule::getPickingZone() const
	{
		return m_pickingZone;
	}

	void MousePickerModule::setSelected(bool _selected)
	{
		m_isSelected = _selected;
	}

	bool MousePickerModule::isSelected() const
	{
		return m_isSelected;
	}

	void MousePickerModule::setSelectedPosition(const Vec2& _position)
	{
		m_selectedPosition = _position;
		snapToPixelPosition();
		UpdateVertexPositions();
	}

	const Vec2& MousePickerModule::getSelectedPosition() const
	{
		return m_selectedPosition;
	}

	void MousePickerModule::setSelectedScreenPosition(const Vec2& _position)
	{
		m_selectedPosition = _position - m_pickingZone.getPosition();
		snapToPixelPosition();
		UpdateVertexPositions();
	}

	const Vec2& MousePickerModule::getSelectedScreenPosition() const
	{
		return m_selectedPosition + m_pickingZone.getPosition();
	}

	void MousePickerModule::setSelectionCallback(std::function<void()> _callback)
	{
		m_selectionCallback = _callback;
	}

	void MousePickerModule::setSelectionPixelSize(const Vec2u& _size)
	{
		m_selectionPixelSize = _size;
	}

	void MousePickerModule::setSelectorSize(const Vec2& _size)
	{
		m_selectorSize = _size;
	}

	Vec2i MousePickerModule::getSelectorPositionInPixelSpace()
	{
		return 
		{
			// Add 0.01f to avoid rounding errors
			(int32_t)(m_selectedPosition.x / m_pickingZone.getSize().x * m_selectionPixelSize.x + 0.01f),
			(int32_t)(m_selectedPosition.y / m_pickingZone.getSize().y * m_selectionPixelSize.y + 0.01f)
		};
	}

	void MousePickerModule::draw(sf::RenderTarget& _target, sf::RenderStates _states) const
	{
		if (!m_isSelected) return;

		_target.draw(m_vertices, _states.Default);
	}

	bool MousePickerModule::isMouseInsidePickingZone(const Vec2& _mousePos) const
	{
		PickingZone m_pickingZone = getPickingZone();

		return m_pickingZone.contains(_mousePos);
	}

	void MousePickerModule::UpdateVertexPositions()
	{
		auto position = getSelectedScreenPosition();

		m_vertices[0].position = sf::Vector2f(position.x, position.y);
		m_vertices[1].position = sf::Vector2f(position.x + m_selectorSize.x, position.y);
		m_vertices[2].position = sf::Vector2f(position.x + m_selectorSize.x, position.y + m_selectorSize.y);
		m_vertices[3].position = sf::Vector2f(position.x, position.y + m_selectorSize.y);
	}

	void MousePickerModule::snapToPixelPosition()
	{
		m_selectedPosition = (UI::Vec2)getSelectorPositionInPixelSpace();

		m_selectedPosition.x *= m_pickingZone.getSize().x / m_selectionPixelSize.x;
		m_selectedPosition.y *= m_pickingZone.getSize().y / m_selectionPixelSize.y;
	}
}