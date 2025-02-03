#pragma once

#include "Panel.h"

namespace UI
{
	class Button;
}

namespace Animation
{
	class FunctionButtons
		: public UI::Panel
	{
	public:
		enum ButtonType
		{
			SaveFile,
			OpenFile,
			SaveAsFile,
			EditData,

			Count
		};

	public:
		FunctionButtons(const UI::Vec2& _windowSize);
		virtual ~FunctionButtons() override;

		virtual void initialize() override;
		virtual void receiveEvent(const sf::Event& _event) override;
		virtual void update(float _deltaTime) override;

		void setButtonCallback(ButtonType _buttonType, const std::function<void()>& _callback);

	private:
		void initializeButtons(const UI::Vec2& _size);
	private:
		sf::Texture m_texture[ButtonType::Count];

		UI::Vec2 m_windowSize;

		std::array<UI::Button*, ButtonType::Count> m_buttons;

		std::function<void()> m_onSelectedFrameChangedcallback;
	};
}

