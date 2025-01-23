#include "corepch.h"

#include "Button.h"
#include "Image.h"
#include "PanelRenderer.h"
#include "Modules.h"

#include "AnimatedSpriteModule.h"
#include "Editor.h"

namespace Animation
{
	Editor::Editor(const EditorData& _editorData)
		: m_data(_editorData)
	{
	}

	Editor::~Editor()
	{
		m_data.Window = nullptr;

		m_functionPanel = nullptr;
		m_animationPanel = nullptr;
	}

	void Editor::initialize()
	{
		m_windowSize = (UI::Vec2)m_data.Window->getSize();

		initializeUI();
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
		m_data.Window->draw(m_editorPanel);
	}

	void Editor::setData(const EditorData& _editorData)
	{
		m_data = _editorData;
	}

	sf::Vector2f Editor::normalizeCoordinates(sf::Vector2i _coord)
	{
		return  UI::Vec2((float)_coord.x / (float)m_textureSize.x, (float)_coord.y / (float)m_textureSize.y);
	}

	void Editor::initializeUI()
	{
		m_font.loadFromFile("Fonts/Font.otf");

		m_editorPanel.initialize();

		m_editorPanel.setSize(m_windowSize);
		m_editorPanel.setColor(m_backgroundClearColor);

		initializeFunctionPanel();
		initializeFunctionButtons();

		initializeAnimationPanel();
		initializeAnimationImages();

		initializePreviewPanel();
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
		m_functionPanel->setColor(m_panelClearColor);

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
			[this]()
			{
				m_animationPanel->setVisible(!m_animationPanel->isVisible());
				m_previewPanel->setVisible(!m_previewPanel->isVisible());
			}
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
		const UI::Vec2 panelSize(m_windowSize.x - padding, m_windowSize.y - functionTotalSize.y - m_padding);

		m_animationPanel = new UI::Panel();
		m_animationPanel->initialize();
		m_animationPanel->setSize(panelSize);
		m_animationPanel->setPosition(UI::Vec2(m_padding, functionPanelSize.y + padding));
		m_animationPanel->setColor(m_panelClearColor);

		m_editorPanel.addChild(m_animationPanel);
	}

	void Editor::initializeAnimationImages()
	{
		UI::Vec2 padding = UI::Vec2(m_imagePadding, m_imagePadding);
		float imageSize = (m_animationPanel->getSize().x / 2.0f) - padding.x * 1.5f;

		auto animationTexture = new sf::Texture();
		auto texTexture = new sf::Texture();

		if (!animationTexture->loadFromFile(m_data.AnimationPath))
		{
			std::cout << "Failed to load texture: " << m_data.AnimationPath << std::endl;
		}

		if (!texTexture->loadFromFile(m_data.TexturePath))
		{
			std::cout << "Failed to load texture: " << m_data.TexturePath << std::endl;
		}

		m_animationImageUI = new UI::Image();
		m_animationImageUI->initialize();
		m_animationImageUI->setTexture(animationTexture);
		m_animationImageUI->setSize(UI::Vec2(imageSize, imageSize));
		m_animationImageUI->setPosition(padding);
		m_animationImageUI->setClearColor(m_backgroundClearColor);

		m_animationPanel->addChild(m_animationImageUI);

		m_textureImageUI = new UI::Image();
		m_textureImageUI->initialize();
		m_textureImageUI->setTexture(texTexture);
		m_textureImageUI->setSize(UI::Vec2(imageSize, imageSize));
		m_textureImageUI->setOrigin(UI::Vec2(imageSize, 0.0f));
		m_textureImageUI->setPosition(UI::Vec2(m_animationPanel->getSize().x - padding.x, padding.y));
		m_textureImageUI->setClearColor(m_backgroundClearColor);

		m_animationPanel->addChild(m_textureImageUI);
	}

	void Editor::initializePreviewPanel()
	{
		m_previewPanel = new UI::PanelRenderer();

		m_previewPanel->initialize();
		m_previewPanel->setSize(m_animationPanel->getSize());
		m_previewPanel->setPosition(m_animationPanel->getPosition());
		m_previewPanel->setClearColor(m_panelClearColor);
		m_previewPanel->setVisible(false);

		AnimationData data
		{
			m_data.AnimationPath.c_str(),
			m_data.TexturePath.c_str(),
			m_data.AnimationCount,
			0.1f
		};

		m_spriteModule = new AnimatedSpriteModule(data);
		AnimatedSprite& sprite = m_spriteModule->asSprite();
		
		UI::Vec2 spritePosition = m_previewPanel->getSize() / 2.0f - UI::Vec2(m_padding, m_padding);
		UI::Vec2 spriteSize = (UI::Vec2)sprite.GetFrameSize();
		float scale = m_previewPanel->getSize().y / (float)spriteSize.y;
		UI::Vec2 origin = spriteSize * 0.5f;

		sprite.setPosition(spritePosition);
		sprite.setOrigin(origin);
		sprite.setScale(scale, scale);

		m_previewPanel->addModule(m_spriteModule);

		m_editorPanel.addChild(m_previewPanel);
	}
}