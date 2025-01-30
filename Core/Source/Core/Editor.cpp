#include "corepch.h"

#include "FileUtility.h"

#include "Button.h"
#include "Image.h"
#include "PanelRenderer.h"
#include "Modules.h"

#include "AnimatedSpriteModule.h"
#include "MousePickerModule.h"
#include "Editor.h"
#include "EditorLayoutSettings.h"

static constexpr uint8_t MaxColor = 255;

namespace Animation
{
	using namespace LayoutSettings;

	Editor::Editor(const EditorData& _editorData)
		: m_data(_editorData)
	{
	}

	Editor::~Editor()
	{
		m_data.Window = nullptr;

		m_functionPanel = nullptr;
		m_animationPanel = nullptr;

		m_textureImageUI = nullptr;
		m_animationImageUI = nullptr;

		m_spriteModule = nullptr;

		for (auto texture : m_texture)
		{
			delete texture;
			texture = nullptr;
		}
	}

#pragma region EditorLoop
	void Editor::initialize()
	{
		m_windowSize = (UI::Vec2)m_data.Window->getSize();
		m_selectedFrame = { 0, 0 };

		initializeUI();

		updateImageData();
	}

	void Editor::receiveEvent(const sf::Event& _event)
	{
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
#pragma endregion

	void Editor::setData(const EditorData& _editorData)
	{
		m_data = _editorData;
	}

	void Editor::saveAnimationFile()
	{
		updateImageData();

		bool status = m_animationImage.saveToFile(m_data.AnimationPath);

		std::cout << (status ? "Animation saved successfully" : "Failed to save animation") << std::endl;
	}

#pragma region ImageProcessing
	void Editor::processSelectedPosition(SelectedImage::SelectedImage _selectedImage, UI::MousePickerModule* _picker)
	{
		// Update the texture if the selected image is the texture and the animation is already selected
		if (_selectedImage != SelectedImage::Texture) return;

		if (m_imagePickers[SelectedImage::Animation] == nullptr ||
			m_imagePickers[SelectedImage::Texture] == nullptr ||
			m_imagePickers[SelectedImage::Animation]->isSelected() == false) return;

		UI::Vec2i selectedTexCoord = m_imagePickers[SelectedImage::Texture]->getSelectorPositionInPixelSpace();
		UI::Vec2i selectedAnimCoord = m_imagePickers[SelectedImage::Animation]->getSelectorPositionInPixelSpace();

		selectedAnimCoord.x += m_selectedFrame.x * (int)m_animFrameSize.x;
		selectedAnimCoord.y += m_selectedFrame.y * (int)m_animFrameSize.y;

		updateAnimationImage(selectedAnimCoord, selectedTexCoord);

		m_imagePickers[SelectedImage::Animation]->setSelected(false);
		m_imagePickers[SelectedImage::Texture]->setSelected(false);
	}

	void Editor::updateAnimationImage(const UI::Vec2i& _animCoord, const UI::Vec2i& _texCoord)
	{
		if (_animCoord.x < 0 || _animCoord.x >= m_animTotalSize.x ||
			_animCoord.y < 0 || _animCoord.y >= m_animTotalSize.y)
			return;

		m_animationImage.setPixel(_animCoord.x, _animCoord.y, getCoordColor(_texCoord));

		updateImageData();
	}

	void Editor::updateImageData()
	{
		auto animationTexture = m_animationImageUI->getTexture();
		if (animationTexture == nullptr) return;

		// Create the image if it doesn't exist or the size is different from the texture
		if (animationTexture->getSize() != m_animationImage.getSize())
		{
			m_animTotalSize = (UI::Vec2)animationTexture->getSize();
			m_animFrameSize = { m_animTotalSize.x / m_data.AnimationCount.x,  m_animTotalSize.y / m_data.AnimationCount.y };

			m_animationImage.create((uint32_t)m_animTotalSize.x, (uint32_t)m_animTotalSize.y, sf::Color::Transparent);
			m_animationImage.copy(animationTexture->copyToImage(), 0, 0);
		}

		// Update the texture with the new image
		animationTexture->update(m_animationImage);
	}

	sf::Color Editor::getCoordColor(const UI::Vec2i& _position)
	{
		auto texture = m_textureImageUI->getTexture();
		if (texture == nullptr) return sf::Color::Transparent;

		if (_position.x < 0 || _position.x >= texture->getSize().x ||
			_position.y < 0 || _position.y >= texture->getSize().y)
			return sf::Color::Transparent;

		UI::Vec2i mappedCoord;
		mappedCoord.x = (int)((float)_position.x / (float)texture->getSize().x * MaxColor);
		mappedCoord.y = (int)((float)_position.y / (float)texture->getSize().y * MaxColor);

		return sf::Color(mappedCoord.x, mappedCoord.y, MaxColor, MaxColor);
	}
#pragma endregion

	void Editor::updateAnimationRect()
	{
		m_animationImageUI->setTextureRect({ (int)(m_selectedFrame.x * m_animFrameSize.x), (int)(m_selectedFrame.y * m_animFrameSize.y), (int)m_animFrameSize.x, (int)m_animFrameSize.y });
	}

	void Editor::OpenFile()
	{
		std::string path = Utility::OpenFileDialog("Textures", "png");
		if (path.empty()) return;
		
		if (!m_texture[0]->loadFromFile(path))
		{
			std::cout << "Failed to load texture: " << path << std::endl;
		}

		updateImageData();
	}

#pragma region UIInitialization
	void Editor::initializeUI()
	{
		m_font.loadFromFile("Fonts/Font.otf");

		initializeTextures();

		m_editorPanel.initialize();

		m_editorPanel.setSize(m_windowSize);
		m_editorPanel.setColor(BackgroundClearColor);

		// Initialize the function panel
		initializeFunctionPanel();
		initializeFunctionButtons();
		initializeAnimEditionButtons();

		// Initialize the animation panel
		initializeAnimationPanel();

		// Initialize the preview panel
		initializePreviewPanel();
	}

	void Editor::initializeTextures()
	{
		m_texture[SelectedImage::Animation] = new sf::Texture();
		m_texture[SelectedImage::Texture] = new sf::Texture();

		if (!m_texture[SelectedImage::Animation]->loadFromFile(m_data.AnimationPath))
		{
			std::cout << "Failed to load texture: " << m_data.AnimationPath << std::endl;
		}

		if (!m_texture[SelectedImage::Texture]->loadFromFile(m_data.TexturePath))
		{
			std::cout << "Failed to load texture: " << m_data.TexturePath << std::endl;
		}

		m_animTotalSize = (UI::Vec2)m_texture[SelectedImage::Animation]->getSize();
		m_animFrameSize = { m_animTotalSize.x / m_data.AnimationCount.x,  m_animTotalSize.y / m_data.AnimationCount.y };

		m_textureSize = m_texture[SelectedImage::Texture]->getSize();
	}

	void Editor::initializeFunctionPanel()
	{
		float functionPanelHeight = m_windowSize.y * FunctionPanelHeightInPercent;
		float padding = WindowPadding * 2.0f;
		UI::Vec2 panelSize = UI::Vec2(m_windowSize.x - padding, functionPanelHeight - padding);

		m_functionPanel = new UI::Panel();
		m_functionPanel->initialize();
		m_functionPanel->setSize(panelSize);
		m_functionPanel->setPosition(UI::Vec2(WindowPadding, WindowPadding));
		m_functionPanel->setColor(PanelClearColor);

		m_editorPanel.addChild(m_functionPanel);
	}

	void Editor::initializeFunctionButtons()
	{
		const uint32_t buttonNumber = 3;
		const float buttonPadding = 15.0f;
		const float buttonHeight = 40.0f;
		const char* buttonNames[buttonNumber] = { "Open", "Save", "Preview" };
		const auto buttonSize = UI::Vec2((m_functionPanel->getSize().x - ((buttonNumber + 1) * buttonPadding)) / buttonNumber, buttonHeight);
		auto buttonPosition = UI::Vec2(buttonPadding, buttonPadding);

		std::function<void()> buttonCallback[buttonNumber] =
		{
			[this]() { OpenFile(); },
			[this]() { saveAnimationFile(); },
			[this]()
			{
				m_animationPanel->setVisible(!m_animationPanel->isVisible());
				m_previewPanel->setVisible(!m_previewPanel->isVisible());

				// update the animation of the animated sprite
				m_spriteModule->asSprite().setAnimation(*m_animationImageUI->getTexture());
			}
		};

		for (size_t i = 0; i < buttonNumber; i++)
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

	void Editor::initializeAnimEditionButtons()
	{
		// Add / remove frame buttons
		const float buttonPadding = 15.0f;
		const float yPosition = buttonPadding * 2.0f + 40.0f;
		const UI::Vec2 buttonSize = { 100.0f, 50.0f };

		// Next Frame Button
		UI::Button* nextFrameButton = new UI::Button();

		nextFrameButton->setSize(buttonSize);
		nextFrameButton->setPosition(UI::Vec2(m_functionPanel->getSize().x - buttonSize.x - buttonPadding, yPosition));

		UI::TextModule* nextFrameText = new UI::TextModule("Next Frame");
		nextFrameText->setFont(&m_font);
		nextFrameButton->addModule(nextFrameText);
		nextFrameText->setTextCentered();

		nextFrameButton->setCallback([this, nextFrameText]()
			{
				m_selectedFrame.x = ++m_selectedFrame.x % m_data.AnimationCount.x;

				nextFrameText->setText("Next Frame: " + std::to_string(m_selectedFrame.x));
				updateAnimationRect();
			});



		m_functionPanel->addChild(nextFrameButton);
	}

	void Editor::initializeAnimationPanel()
	{
		const float padding = 2.0f * WindowPadding;
		const UI::Vec2 panelSize(m_windowSize.x - padding, AnimPanelHeightInPercent * m_windowSize.y - WindowPadding);

		m_animationPanel = new UI::Panel();
		m_animationPanel->initialize();
		m_animationPanel->setSize(panelSize);
		m_animationPanel->setPosition(UI::Vec2(WindowPadding, m_functionPanel->getSize().y + padding));
		m_animationPanel->setColor(PanelClearColor);

		m_editorPanel.addChild(m_animationPanel);

		initializeAnimationImages();
	}

	void Editor::initializeAnimationImages()
	{
		UI::Vec2 imageSize(m_animationPanel->getSize().y - PanelPadding * 2.0f, m_animationPanel->getSize().y - PanelPadding * 2.0f);
		UI::Vec2 animationPosition = UI::Vec2(PanelPadding, PanelPadding);
		UI::Vec2 texturePosition(m_animationPanel->getSize().x - PanelPadding - imageSize.x, PanelPadding);

		// Create the animation image
		m_animationImageUI = createAnimationImage(SelectedImage::Animation, animationPosition, imageSize);
		m_animationImageUI->setTextureRect({ 0, 0, (int32_t)m_animFrameSize.x, (int32_t)m_animFrameSize.y });
		m_imagePickers[SelectedImage::Animation]->setSelectionPixelSize((UI::Vec2u)m_animFrameSize);

		// Create the texture image
		m_textureImageUI = createAnimationImage(SelectedImage::Texture, texturePosition, imageSize);
	}

	UI::Image* Editor::createAnimationImage(SelectedImage::SelectedImage _imageID, const UI::Vec2& _position, const UI::Vec2& _size)
	{
		UI::PickingZone pickingZone(_position + m_animationPanel->getPosition(), _size);

		// Creating the image
		UI::Image* image = new UI::Image();
		image->initialize();
		image->setTexture(m_texture[_imageID]);
		image->setPosition(_position);
		image->setSize(_size);
		image->setClearColor(BackgroundClearColor);

		// Creating the ImagePicker Module
		m_imagePickers[_imageID] = new UI::MousePickerModule();
		m_imagePickers[_imageID]->setSelectionCallback(std::bind(&Editor::processSelectedPosition, this, _imageID, m_imagePickers[_imageID]));
		m_imagePickers[_imageID]->setSelectionPixelSize(m_texture[_imageID]->getSize());
		m_imagePickers[_imageID]->setPickingZone(pickingZone);
		m_imagePickers[_imageID]->setSelectorSize({
			_size.x / m_animFrameSize.x,
			_size.y / m_animFrameSize.y
			});

		image->addModule(m_imagePickers[_imageID]);
		m_animationPanel->addChild(image);

		return image;
	}

	void Editor::initializePreviewPanel()
	{
		m_previewPanel = new UI::PanelRenderer();

		m_previewPanel->initialize();
		m_previewPanel->setSize(m_animationPanel->getSize());
		m_previewPanel->setPosition(m_animationPanel->getPosition());
		m_previewPanel->setClearColor(PanelClearColor);
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

		UI::Vec2 spritePosition = m_previewPanel->getSize() / 2.0f - UI::Vec2(WindowPadding, WindowPadding);
		UI::Vec2 spriteSize = (UI::Vec2)sprite.getFrameSize();
		float scale = m_previewPanel->getSize().y / (float)spriteSize.y;
		UI::Vec2 origin = spriteSize * 0.5f;

		sprite.setPosition(spritePosition);
		sprite.setOrigin(origin);
		sprite.setScale(scale, scale);

		m_previewPanel->addModule(m_spriteModule);

		m_editorPanel.addChild(m_previewPanel);
	}
#pragma endregion 
}