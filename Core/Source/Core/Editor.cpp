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
		m_panels[Animation].Texture.loadFromFile(_animationPath);
		m_panels[Texture].Texture.loadFromFile(_texturePath);
		m_textureSize = m_panels[Texture].Texture.getSize();
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

		m_panelSize.x = (m_windowSize.x - (MAX_PANELS + 1) * m_padding) / MAX_PANELS;
		m_panelSize.y = m_panelSize.x;

		m_scaleFromTexToPanel = (float)m_panelSize.x / (float)m_textureSize.x;

		for (int i = 0; i < MAX_PANELS; i++)
		{
			m_panels[i].Position = sf::Vector2f(m_padding + (m_panelSize.x + m_padding) * i, m_windowSize.y - m_padding - m_panelSize.y);
			m_panels[i].ClearColor = sf::Color(0, 0, 0, 255);
			m_panels[i].RenderTexture.create(m_textureSize.x, m_textureSize.y);
		}

		m_animationImage.create(m_textureSize.x, m_textureSize.y);
		m_animationImage.copy(m_panels[Animation].Texture.copyToImage(), 0, 0);

		m_animationCursor.SetTextureSize(m_textureSize.x);
		m_animationCursor.SetWindow(m_editorData.Window);
		m_animationCursor.OnInitialize("Selection.png", sf::IntRect
		(
			m_panels[Animation].Position.x,
			m_panels[Animation].Position.y,
			m_panelSize.x,
			m_panelSize.y
		));

		m_textureCursor.SetTextureSize(m_textureSize.x);
		m_textureCursor.SetWindow(m_editorData.Window);
		m_textureCursor.OnInitialize("Selection.png", sf::IntRect
		(
			m_panels[Texture].Position.x,
			m_panels[Texture].Position.y,
			m_panelSize.x,
			m_panelSize.y
		));
		m_textureCursor.SetClickCallback([&]()
			{
				m_textureCursor.SetSelected(false);

				if (!m_animationCursor.IsSelected()) return;

				sf::Vector2i pixel = m_animationCursor.GetSelectedPixel();
				sf::Vector2f coord = normalizeCoordinates(m_textureCursor.GetHoveredPixel());
				sf::Color color(coord.x * 255.0f, coord.y * 255.0f, 0.0f, 255.0f);

				m_animationImage.setPixel(pixel.x, pixel.y, color);

				m_panels[Animation].Texture.loadFromImage(m_animationImage);

				m_animationCursor.SetSelected(false);
			});

		initializePanel();
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

		m_animationCursor.OnEvent(_event);
		m_textureCursor.OnEvent(_event);
	}

	void Editor::update(float _deltaTime)
	{
		m_editorPanel.update(_deltaTime);

		m_animationCursor.OnUpdate(_deltaTime);
		m_textureCursor.OnUpdate(_deltaTime);
	}

	void Editor::draw()
	{
		static sf::Sprite sprite;

		for (int i = 0; i < MAX_PANELS; i++)
		{
			auto& panel = m_panels[i];

			if (panel.Texture.getSize() == sf::Vector2u(0, 0)) continue;

			sprite.setTexture(panel.Texture);

			panel.RenderTexture.clear(panel.ClearColor);
			panel.RenderTexture.draw(sprite);
			panel.RenderTexture.display();

			m_panelSprite.setTexture(panel.RenderTexture.getTexture());
			m_panelSprite.setPosition(panel.Position);
			m_panelSprite.setScale(m_scaleFromTexToPanel, m_scaleFromTexToPanel);
			m_editorData.Window->draw(m_panelSprite);
		}

		m_animationCursor.OnRender(m_editorData.Window);
		m_textureCursor.OnRender(m_editorData.Window);

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

	void Editor::initializePanel()
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

		sf::Vector2f panelSize = sf::Vector2f(m_windowSize.x - 2.0f * m_padding, 200.0f - m_padding);
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
		const UI::Vec2 functionSize(m_windowSize.y - 2.0f * m_padding, 200.0f);

		m_animationPanel = new UI::Panel();
		m_animationPanel->initialize();

		UI::Vec2 functionPanelSize = m_functionPanel->getSize();
		UI::Vec2 panelSize(
			m_windowSize.x - 2.0f * m_padding,
			m_windowSize.y - functionSize.y - 2 * m_padding
		);

		m_animationPanel->setSize(panelSize);
		m_animationPanel->setPosition(UI::Vec2(m_padding, functionSize.y + m_padding));
		m_animationPanel->SetColor(sf::Color(50, 50, 50, 255));

		m_editorPanel.addChild(m_animationPanel);
	}
}