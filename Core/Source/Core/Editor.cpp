#include "corepch.h"

#include "Button.h"
#include "Image.h"
#include "PanelRenderer.h"
#include "Modules.h"

#include "AnimatedSpriteModule.h"
#include "MousePickerModule.h"
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

#pragma region EditorLoop
	void Editor::initialize()
	{
		m_windowSize = (UI::Vec2)m_data.Window->getSize();

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

	UI::Vec2i Editor::fromScreenToTextureCoord(const UI::Vec2& _position, const UI::Vec2& _size)
	{
		return
		{
			int(_position.x / _size.x * m_textureSize.x),
			int(_position.y / _size.y * m_textureSize.y)
		};
	}

	void Editor::processSelectedPosition(SelectedImage::SelectedImage _selectedImage, UI::MousePickerModule* _picker)
	{
		UI::Vec2 selectedPosition = _picker->getSelectedPosition();
		UI::Vec2 size = _picker->getPickingZone().getSize();

		//// Change the position to be in the texture space
		selectedPosition = (UI::Vec2)fromScreenToTextureCoord(selectedPosition, size);

		// snap the position to the screen position of a pixel
		selectedPosition.x *= size.x / m_textureSize.x;
		selectedPosition.y *= size.y / m_textureSize.y;
		_picker->setSelectedPosition(selectedPosition);

		//// If the texture is selected, update the texture image
		if (_selectedImage != SelectedImage::Texture) return;

		if (m_imagePickers[SelectedImage::Animation] == nullptr ||
			m_imagePickers[SelectedImage::Texture] == nullptr ||
			m_imagePickers[SelectedImage::Animation]->isSelected() == false) return;

		updateAnimationImage();

		m_imagePickers[SelectedImage::Animation]->setSelected(false);
		m_imagePickers[SelectedImage::Texture]->setSelected(false);
	}

	void Editor::updateAnimationImage()
	{
		auto animationPicker = m_imagePickers[SelectedImage::Animation];
		auto texturePicker = m_imagePickers[SelectedImage::Texture];

		UI::Vec2i selectedAnimCoord = fromScreenToTextureCoord(animationPicker->getSelectedPosition(), animationPicker->getPickingZone().getSize());
		UI::Vec2i selectedTexCoord = fromScreenToTextureCoord(texturePicker->getSelectedPosition(), texturePicker->getPickingZone().getSize());

		m_animationImage.setPixel(selectedAnimCoord.x, selectedAnimCoord.y, getCoordColor(selectedTexCoord));

		updateImageData();
	}

	void Editor::updateImageData()
	{
		auto animationTexture = m_animationImageUI->getTexture();
		if (animationTexture == nullptr) return;

		// Create the image if it doesn't exist or the size is different from the texture
		if (animationTexture->getSize() != m_animationImage.getSize())
		{
			m_textureSize = animationTexture->getSize();

			m_animationImage.create((uint32_t)m_textureSize.x, (uint32_t)m_textureSize.y, sf::Color::Transparent);
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
		mappedCoord.x = (int)((float)_position.x / (float)texture->getSize().x * 255);
		mappedCoord.y = (int)((float)_position.y / (float)texture->getSize().y * 255);

		return sf::Color(mappedCoord.x, mappedCoord.y, 0, 255);
	}

#pragma region UIInitialization
	void Editor::initializeUI()
	{
		m_font.loadFromFile("Fonts/Font.otf");

		m_editorPanel.initialize();

		m_editorPanel.setSize(m_windowSize);
		m_editorPanel.setColor(m_backgroundClearColor);

		// Initialize the function panel
		initializeFunctionPanel();
		initializeFunctionButtons();

		// Initialize the animation panel
		initializeAnimationPanel();
		initializeAnimationImages();

		// Initialize the preview panel
		initializePreviewPanel();
	}

	void Editor::initializeFunctionPanel()
	{
		float functionPanelHeight = m_windowSize.y * m_functionPanelHeightInPercent * 0.01f;
		float padding = m_padding * 2.0f;
		UI::Vec2 panelSize = UI::Vec2(m_windowSize.x - padding, functionPanelHeight - padding);

		m_functionPanel = new UI::Panel();
		m_functionPanel->initialize();
		m_functionPanel->setSize(panelSize);
		m_functionPanel->setPosition(UI::Vec2(m_padding, m_padding));
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
			[this]() { std::cout << "Open not implemented yet!" << std::endl; },
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

		float imageWidth = m_animationPanel->getSize().x / 2.0f - 1.5f * padding.x;
		UI::Vec2 imageSize(imageWidth, imageWidth);

		// Create the animation image
		UI::Vec2 animationPosition = padding;
		m_animationImageUI = createAnimationImage(m_data.AnimationPath, animationPosition, imageSize, [this](UI::MousePickerModule* _picker)
			{
				processSelectedPosition(SelectedImage::Animation, _picker);
			});
		m_imagePickers[SelectedImage::Animation] = m_animationImageUI->getFirstModuleOfType<UI::MousePickerModule>();

		// Create the texture image
		UI::Vec2 texturePosition((m_animationPanel->getSize().x + padding.x) * 0.5f, padding.y);
		m_textureImageUI = createAnimationImage(m_data.TexturePath, texturePosition, imageSize, [this](UI::MousePickerModule* _picker)
			{
				processSelectedPosition(SelectedImage::Texture, _picker);
			});
		m_imagePickers[SelectedImage::Texture] = m_textureImageUI->getFirstModuleOfType<UI::MousePickerModule>();
	}

	UI::Image* Editor::createAnimationImage(const std::string& _texPath, const UI::Vec2& _position, const UI::Vec2& _size, const std::function<void(UI::MousePickerModule*)>& _callback)
	{
		auto texture = new sf::Texture();
		if (!texture->loadFromFile(_texPath))
		{
			std::cout << "Failed to load texture: " << _texPath << std::endl;
		}

		UI::Image* image = new UI::Image();
		image->initialize();
		image->setTexture(texture);
		image->setPosition(_position);
		image->setSize(_size);
		image->setClearColor(m_backgroundClearColor);

		UI::MousePickerModule* animationPicker = new UI::MousePickerModule();

		image->addModule(animationPicker);

		UI::PickingZone pickingZone(_position + m_animationPanel->getPosition(), _size);
		animationPicker->setPickingZone(pickingZone);
		animationPicker->setSelectionCallback(std::bind(_callback, animationPicker));
		float pixelSize = _size.x / (float)texture->getSize().x;
		animationPicker->setSelectorSize(UI::Vec2(pixelSize, pixelSize));

		m_animationPanel->addChild(image);

		return image;
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