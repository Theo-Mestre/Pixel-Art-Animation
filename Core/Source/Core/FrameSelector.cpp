#include "corepch.h"

#include "Button.h"
#include "Modules.h"

#include "FrameSelector.h"
#include "EditorLayoutSettings.h"

namespace Animation
{
	using namespace LayoutSettings;

	static constexpr size_t ButtonNumber = 4;

	FrameSelector::FrameSelector(const UI::Vec2& _windowSize)
		: Panel()
		, m_texture()
		, m_windowSize(_windowSize)
		, m_selectedFrame(nullptr)
		, m_frameCount()
		, m_textModule(nullptr)
		, m_font(nullptr)
		, m_onSelectedFrameChangedcallback(nullptr)
	{
	}

	FrameSelector::~FrameSelector()
	{
		m_selectedFrame = nullptr;
	}

	void FrameSelector::initialize()
	{
		Panel::initialize();
		setColor(sf::Color::Transparent);

		UI::Vec2 panelSize =
		{
			m_windowSize.y * FunctionPanelHeightInPercent - 2.0f * PanelPadding,
			m_windowSize.y * FunctionPanelHeightInPercent - 2.0f * PanelPadding
		};
		setSize(panelSize);

		panelSize = panelSize / (float)ButtonNumber;

		UI::Panel* middlePanel = new UI::Panel();
		middlePanel->initialize();
		middlePanel->setSize((panelSize - UI::Vec2(PanelPadding, PanelPadding)) * 2.0f);
		middlePanel->setPosition(panelSize + UI::Vec2(PanelPadding, PanelPadding));
		addChild(middlePanel);

		initializeButtons(panelSize);
		initializeTextModule(panelSize);
	}

	void FrameSelector::receiveEvent(const sf::Event& _event)
	{
		Panel::receiveEvent(_event);
	}

	void FrameSelector::update(float _deltaTime)
	{
		Panel::update(_deltaTime);
	}

	void FrameSelector::setSelectedFrameData(UI::Vec2i* _selectedFrame, const UI::Vec2u& _frameCount)
	{
		m_selectedFrame = _selectedFrame;
		m_frameCount = _frameCount;
	}

	void FrameSelector::setFrameCount(const UI::Vec2u& _frameCount)
	{
		m_frameCount = _frameCount;
		CheckFrameBounds();
	}

	void FrameSelector::updateFrameBounds()
	{
		CheckFrameBounds();
	}

	void FrameSelector::setFont(sf::Font* _font)
	{
		if (_font == nullptr) return;

		m_font = _font;
		if (m_textModule)
		{
			m_textModule->setFont(m_font);
		}
	}

	void FrameSelector::setOnSelectedFrameChangedCallback(const std::function<void()>& _callback)
	{
		m_onSelectedFrameChangedcallback = _callback;
	}

	void FrameSelector::initializeButtons(const UI::Vec2& _size)
	{
		m_texture.loadFromFile(FrameSelectorTexturePath);
		UI::Vec2 textureSize = (UI::Vec2)m_texture.getSize();
		textureSize.y /= ButtonNumber;

		sf::FloatRect textureRect[ButtonNumber] =
		{
			{0, 0, textureSize.x, textureSize.y},
			{0, textureSize.y, textureSize.x, textureSize.y},
			{0, textureSize.y * 2.0f, textureSize.x, textureSize.y},
			{0, textureSize.y * 3.0f, textureSize.x, textureSize.y}
		};

		UI::Vec2 position[ButtonNumber] =
		{
			{0.0f, 1.5f * _size.x},
			{m_size.x - _size.x, 1.5f * _size.y},
			{1.5f * _size.x, 0.0f},
			{1.5f * _size.x, m_size.y - _size.y}
		};

		UI::Button* button[ButtonNumber];
		for (int i = 0; i < ButtonNumber; i++)
		{
			button[i] = new UI::Button();
			button[i]->initialize();
			button[i]->setSize(_size);
			button[i]->setPosition(position[i]);
			button[i]->setTexture(&m_texture);
			button[i]->setTextureCoords(textureRect[i]);
			addChild(button[i]);
		}

		button[0]->setCallback([&]() { onButtonClick(&m_selectedFrame->x, -1); });
		button[1]->setCallback([&]() { onButtonClick(&m_selectedFrame->x, 1); });
		button[2]->setCallback([&]() { onButtonClick(&m_selectedFrame->y, -1); });
		button[3]->setCallback([&]() { onButtonClick(&m_selectedFrame->y, 1); });
	}

	void FrameSelector::initializeTextModule(const UI::Vec2& _size)
	{
		m_textModule = new UI::TextModule("0 : 0");
		addModule(m_textModule);
		if (m_font == nullptr) return;

		m_textModule->setFont(m_font);
		m_textModule->setSize(_size);
		m_textModule->getText().setCharacterSize(20);
		m_textModule->setTextCentered();
	}

	void FrameSelector::onButtonClick(int* _frame, int _value)
	{
		*_frame += _value;
		CheckFrameBounds();

		if (m_onSelectedFrameChangedcallback)
		{
			m_onSelectedFrameChangedcallback();
		}

		m_textModule->setText(std::to_string(m_selectedFrame->x) + " : " + std::to_string(m_selectedFrame->y));
	}

	void FrameSelector::CheckFrameBounds()
	{
		if (m_selectedFrame == nullptr) return;

		if (m_selectedFrame->x < 0)	m_selectedFrame->x = m_frameCount.x - 1;
		else if (m_selectedFrame->x >= m_frameCount.x) m_selectedFrame->x = 0;

		if (m_selectedFrame->y < 0) m_selectedFrame->y = m_frameCount.y - 1;
		else if (m_selectedFrame->y >= m_frameCount.y) m_selectedFrame->y = 0;
	}
}