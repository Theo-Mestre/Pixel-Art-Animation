#pragma once

#include "Panel.h"

namespace sf
{
	class Font;
}

namespace UI
{
	class TextModule;
}

namespace Animation
{
	class FrameSelector
		: public UI::Panel
	{
	public:
		FrameSelector(const UI::Vec2& _windowSize);
		virtual ~FrameSelector() override;

		virtual void initialize() override;
		virtual void receiveEvent(const sf::Event& _event) override;
		virtual void update(float _deltaTime) override;

		void setSelectedFrameData(UI::Vec2i* _selectedFrame, const UI::Vec2u& _frameCount);
		void setFrameCount(const UI::Vec2u& _frameCount);
		void updateFrameBounds();

		void setFont(sf::Font* _font);

		void setOnSelectedFrameChangedCallback(const std::function<void()>& _callback);
	private:
		void initializeButtons(const UI::Vec2& _size);
		void initializeTextModule(const UI::Vec2& _size);

		void onButtonClick(int* _frame, int _value);
		void CheckFrameBounds();
	private:
		sf::Texture m_texture;

		UI::Vec2 m_windowSize;

		UI::Vec2i* m_selectedFrame;
		UI::Vec2u m_frameCount;

		UI::TextModule* m_textModule;
		sf::Font* m_font;

		std::function<void()> m_onSelectedFrameChangedcallback;
	};
}

