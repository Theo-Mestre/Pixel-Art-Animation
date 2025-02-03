#include "corepch.h"

#include "EditorLayoutSettings.h"
#include "Button.h"

#include "FunctionButtons.h"

namespace Animation
{
	using namespace LayoutSettings;

	FunctionButtons::FunctionButtons(const UI::Vec2& _windowSize)
		: Panel()
		, m_texture()
		, m_windowSize(_windowSize)
		, m_buttons()
	{
	}

	FunctionButtons::~FunctionButtons()
	{
	}

	void FunctionButtons::initialize()
	{
		Panel::initialize();
		setColor(sf::Color::Transparent);

		UI::Vec2 panelSize =
		{
			m_windowSize.y * FunctionPanelHeightInPercent - 2.0f * PanelPadding,
			m_windowSize.y * FunctionPanelHeightInPercent - 2.0f * PanelPadding
		};
		setSize(panelSize);

		for (size_t i = 0; i < ButtonType::Count; i++)
		{
			m_texture[i].loadFromFile(IconsPath[i]);
		}

		panelSize = (panelSize - UI::Vec2(PanelPadding, PanelPadding)) * 0.5f;

		initializeButtons(panelSize);
	}

	void FunctionButtons::receiveEvent(const sf::Event& _event)
	{
		Panel::receiveEvent(_event);
	}

	void FunctionButtons::update(float _deltaTime)
	{
		Panel::update(_deltaTime);
	}

	void FunctionButtons::setButtonCallback(ButtonType _buttonType, const std::function<void()>& _callback)
	{
		if (_buttonType >= ButtonType::Count) return;
		if (m_buttons[_buttonType] == nullptr) return;

		m_buttons[_buttonType]->setCallback(_callback);
	}

	void FunctionButtons::initializeButtons(const UI::Vec2& _size)
	{
		const UI::Vec2 positions[ButtonType::Count] =
		{
			{ 0.0f, 0.0f },
			{ _size.x + PanelPadding, 0.0f },
			{ 0.0f, _size.y + PanelPadding },
			{ _size.x + PanelPadding,  _size.y + PanelPadding }
		};

		for (size_t i = 0; i < ButtonType::Count; i++)
		{
			m_buttons[i] = new UI::Button();
			m_buttons[i]->setSize(_size);
			m_buttons[i]->setPosition(positions[i]);
			m_buttons[i]->setTexture(&m_texture[i]);
			addChild(m_buttons[i]);
		}
	}
}