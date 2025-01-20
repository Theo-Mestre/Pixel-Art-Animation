#include "corepch.h"

#include "Button.h"
#include "Modules.h"

#include "Editor.h"

namespace Animation
{
	Editor::Editor(const EditorData& _editorData)
		: m_editorData(_editorData)
	{
	}

	Editor::Editor(const std::string& _texturePath,
		const std::string& _animationPath,
		sf::Vector2u _animationSize,
		sf::RenderWindow* _window)
		: m_editorData({ _window })
		, m_animationCount(_animationSize)
	{
	}

	Editor::~Editor()
	{
		m_editorData.Window = nullptr;

		m_animationPanel = nullptr;
		m_functionPanel = nullptr;
	}

	void Editor::initialize()
	{
		m_windowSize = (UI::Vec2)m_editorData.Window->getSize();
		m_padding = 10.0f;

		initializePanels();
	}

	void Editor::receiveEvent(const sf::Event& _event)
	{
		if (_event.type == sf::Event::KeyPressed)
		{
			if (_event.key.code == sf::Keyboard::Space)
			{
				m_animationImage.saveToFile("AnimationTest.png");
			}
		}

		m_editorPanel.receiveEvent(_event);
	}

	void Editor::update(float _deltaTime)
	{
		m_editorPanel.update(_deltaTime);
	}

	void Editor::draw()
	{
		m_editorData.Window->draw(m_editorPanel);
	}

	void Editor::setData(const EditorData& _editorData)
	{
		m_editorData = _editorData;
	}

	sf::Vector2f Editor::normalizeCoordinates(sf::Vector2i _coord)
	{
		return  UI::Vec2((float)_coord.x / (float)m_textureSize.x, (float)_coord.y / (float)m_textureSize.y);
	}

	void Editor::initializePanels()
	{
		m_font.loadFromFile("Fonts/Font.otf");

		m_editorPanel.initialize();

		m_editorPanel.setSize(m_windowSize);
		m_editorPanel.SetColor(sf::Color(20, 20, 20, 255));

		initializeFunctionPanel();
		initializeFunctionButtons();

		initializeAnimationPanel();
	}

	void Editor::initializeFunctionPanel()
	{
		m_functionPanel = new UI::Panel();
		m_functionPanel->initialize();

		float functionPanelHeight = m_windowSize.y * m_functionPanelHeightInPercent * 0.01f;
		float padding = m_padding * 2.0f;

		sf::Vector2f panelSize = sf::Vector2f(m_windowSize.x - padding, functionPanelHeight - padding);
		m_functionPanel->setSize(panelSize);
		m_functionPanel->setPosition(sf::Vector2f(m_padding, m_padding));
		m_functionPanel->SetColor(sf::Color(50, 50, 50, 255));

		m_editorPanel.addChild(m_functionPanel);
	}

	void Editor::initializeFunctionButtons()
	{
		const uint32_t buttonNumber = 3;
		const float buttonPadding = 25.0f;
		const float buttonHeight = 50.0f;
		const char* buttonNames[buttonNumber] = { "Open", "Save", "Preview" };
		const auto buttonSize = UI::Vec2((m_functionPanel->getSize().x - ((buttonNumber + 1) * buttonPadding)) / buttonNumber, buttonHeight);
		auto buttonPosition = UI::Vec2(buttonPadding, buttonPadding);

		std::function<void()> buttonCallback[buttonNumber] =
		{
			[this]() { m_animationCursor.SetSelected(true); },
			[this]() { m_animationImage.saveToFile("AnimationTest.png"); },
			[this]() { std::cout << "Preview" << std::endl; }
		};

		for (int i = 0; i < buttonNumber; i++)
		{
			buttonPosition.x = buttonPadding + (buttonSize.x + buttonPadding) * i;

			UI::Button* button = new UI::Button();
			button->setSize(buttonSize);
			button->setPosition(buttonPosition);
			button->setCallback(buttonCallback[i]);

			UI::TextModule* text = new UI::TextModule(buttonNames[i]);
			text->setFont(&m_font);
			text->setColor(sf::Color::Black);
			button->addModule(text);
			text->setTextCentered();

			m_functionPanel->addChild(button);
		}
	}

	void Editor::initializeAnimationPanel()
	{
		const float padding = 2.0f * m_padding;
		const UI::Vec2 functionPanelSize = m_functionPanel->getSize();
		const UI::Vec2 functionTotalSize = m_functionPanel->getSize() + UI::Vec2(padding, padding);
		const UI::Vec2 panelSize(m_windowSize.x - padding, m_windowSize.y - functionTotalSize.y - padding);

		m_animationPanel = new UI::Panel();
		m_animationPanel->initialize();
		m_animationPanel->setSize(panelSize);
		m_animationPanel->setPosition(UI::Vec2(m_padding, functionPanelSize.y + padding));
		m_animationPanel->SetColor(sf::Color(50, 50, 50, 255));

		m_editorPanel.addChild(m_animationPanel);
	}
}