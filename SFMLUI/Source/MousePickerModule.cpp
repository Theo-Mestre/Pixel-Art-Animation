#include <SFML/Graphics.hpp>

#include <iostream>

#include "UIElement.h"
#include "MousePickerModule.h"

namespace UI
{
	MousePickerModule::MousePickerModule()
		: Module()
		, m_pickingZone()
		, m_selectedPosition()
		, m_isSelected(false)
		, m_selectionCallback(nullptr)
	{
	}

	void MousePickerModule::receiveEvent(const sf::Event& _event)
	{
		if (_event.type == sf::Event::MouseMoved)
		{
			m_owner->setHovered(isMouseInsidePickingZone(Vec2(_event.mouseMove.x, _event.mouseMove.y)));
		}

		if (!m_owner->isHovered()) return;

		if (_event.type != sf::Event::MouseButtonPressed) return;

		if (_event.mouseButton.button == sf::Mouse::Right)
		{
			m_isSelected = false;
			return;
		}

		// If left clic is pressed
		if (_event.mouseButton.button != sf::Mouse::Left) return;

		// enable selection and invoke callback
		m_isSelected = true;
		m_selectedPosition = Vec2(_event.mouseButton.x, _event.mouseButton.y);

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

	const Vec2& MousePickerModule::getSelectedPosition() const
	{
		return
		{
			m_selectedPosition.x - m_pickingZone.getPosition().x,
			m_selectedPosition.y - m_pickingZone.getPosition().y
		};
	}

	const Vec2& MousePickerModule::getSelectedScreenPosition() const
	{
		return m_selectedPosition;
	}

	void MousePickerModule::SetSelectionCallback(std::function<void()> _callback)
	{
		m_selectionCallback = _callback;
	}

	void MousePickerModule::draw(sf::RenderTarget& _target, sf::RenderStates _states) const
	{
	}

	bool MousePickerModule::isMouseInsidePickingZone(const Vec2& _mousePos) const
	{
		PickingZone m_pickingZone = getPickingZone();

		return m_pickingZone.contains(_mousePos);
	}


}