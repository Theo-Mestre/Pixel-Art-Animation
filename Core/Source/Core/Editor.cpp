#include "corepch.h"

#include "FileUtility.h"

#include "Image.h"
#include "Label.h"
#include "Button.h"
#include "Modules.h"
#include "PanelRenderer.h"

#include "AnimatedSpriteModule.h"
#include "EditorLayoutSettings.h"
#include "MousePickerModule.h"
#include "FunctionButtons.h"
#include "EditorDataPanel.h"
#include "FrameSelector.h"
#include "Editor.h"

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

		m_editorDataPanel = nullptr;

		for (auto texture : m_textures)
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

	void Editor::saveAnimationTexture()
	{
		updateImageData();
		bool status = m_animationImage.saveToFile(m_data.AnimationPath);
		std::cout << (status ? "Animation saved successfully." : "Failed to save animation.") << std::endl;
	}

	void Editor::saveAs()
	{
		std::string path = Utility::SaveFileDialog({ ".anim", ".png" });
		if (path.empty()) return;

		std::string extension = path.substr(path.find_last_of('.'));

		if (extension == ".anim")
		{
			SaveAnimFile(path);
			return;
		}

		if (extension == ".png")
		{
			m_data.AnimationPath = path;
			saveAnimationTexture();	
		}
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

	void Editor::SaveAnimFile(const std::string& _path)
	{
		std::ofstream file(_path, std::ios::out);
		if (!file.is_open())
		{
			std::cout << "Failed to open file: " << m_data.AnimationPath << std::endl;
			return;
		}

		file << m_data.AnimationPath << std::endl;
		file << m_data.TexturePath << std::endl;
		file << m_data.AnimationCount.x << " " << m_data.AnimationCount.y << std::endl;
		file << m_data.FrameDuration << std::endl;

		file.close();

		std::cout << "Animation file saved successfully." << std::endl;
	}

	void Editor::OpenAnimFile()
	{
		std::string path = Utility::OpenFileDialog("Animation File", "anim");
		if (path.empty()) return;

		ReadAnimFile(path);

		if (m_onReset != nullptr)
		{
			std::cout << "Resetting the editor." << std::endl;
			m_onReset(&m_data);
		}
	}

	void Editor::ReadAnimFile(const std::string& _path)
	{
		std::ifstream file(_path, std::ios::in);
		if (!file.is_open())
		{
			std::cout << "Failed to open file: " << _path << std::endl;
			return;
		}

		std::string line;
		std::getline(file, line);

		m_data.AnimationPath = line;

		std::getline(file, line);
		m_data.TexturePath = line;

		std::getline(file, line);

		{ // Read the animation count
			std::stringstream ss(line);

			ss >> m_data.AnimationCount.x;
			ss >> m_data.AnimationCount.y;
		}

		{ // Read the animation speed
			std::getline(file, line);
			std::stringstream ss(line);

			ss >> m_data.FrameDuration;
		}

		file.close();
	}

	void Editor::TogglePreviewPanel()
	{
		if (m_editorDataPanel->isVisible()) return;

		bool isVisible = m_previewPanel->isVisible();

		m_previewPanel->setVisible(!isVisible);

		m_animationPanel->setVisible(isVisible);

		if (!isVisible == true)
		{
			// update the animation of the animated sprite
			m_spriteModule->asSprite().setAnimation(*m_animationImageUI->getTexture());
		}
	}

	void Editor::ToggleEditorDataPanel()
	{
		bool isVisible = m_editorDataPanel->isVisible();

		if (isVisible && m_onReset != nullptr)
		{
			std::cout << "Resetting the editor." << std::endl;
			m_onReset(&m_data);
			return;
		}

		m_editorDataPanel->setVisible(!isVisible);
		m_editorDataPanel->UpdateData();

		m_animationPanel->setVisible(isVisible);
		m_previewPanel->setVisible(false);
	}

#pragma region UIInitialization
	void Editor::initializeUI()
	{
		m_font.loadFromFile(FontPath);

		initializeTextures();

		m_editorPanel.initialize();

		m_editorPanel.setSize(m_windowSize);
		m_editorPanel.setColor(BackgroundClearColor);

		// Initialize the animation panel
		initializeAnimationPanel();

		// Initialize the function panel
		initializeFunctionPanel();
		initializeFunctionButtons();

		// Initialize the preview panel
		initializePreviewPanel();

		// Initialize the editor data panel
		initializeEditorDataPanel();
	}

	void Editor::initializeTextures()
	{
		m_textures[SelectedImage::Animation] = new sf::Texture();
		m_textures[SelectedImage::Texture] = new sf::Texture();

		if (!m_textures[SelectedImage::Animation]->loadFromFile(m_data.AnimationPath))
		{
			std::cout << "Failed to load texture: " << m_data.AnimationPath << std::endl;
		}

		if (!m_textures[SelectedImage::Texture]->loadFromFile(m_data.TexturePath))
		{
			std::cout << "Failed to load texture: " << m_data.TexturePath << std::endl;
		}

		m_animTotalSize = (UI::Vec2)m_textures[SelectedImage::Animation]->getSize();
		m_animFrameSize = { m_animTotalSize.x / m_data.AnimationCount.x,  m_animTotalSize.y / m_data.AnimationCount.y };

		m_textureSize = m_textures[SelectedImage::Texture]->getSize();
	}

	void Editor::initializeFunctionPanel()
	{
		const UI::Vec2 panelSize = UI::Vec2
		(
			m_windowSize.x - 2.0f * WindowPadding,
			m_windowSize.y * FunctionPanelHeightInPercent
		);

		m_functionPanel = new UI::Panel();
		m_functionPanel->initialize();
		m_functionPanel->setSize(panelSize);
		m_functionPanel->setPosition(UI::Vec2(WindowPadding, WindowPadding));
		m_functionPanel->setColor(PanelClearColor);

		m_editorPanel.addChild(m_functionPanel);
	}

	void Editor::initializeFunctionButtons()
	{
		// Initialize the frame selector
		m_functionButtons = new FunctionButtons(m_windowSize);
		m_functionPanel->addChild(m_functionButtons);

		m_functionButtons->initialize();
		m_functionButtons->setPosition(UI::Vec2(PanelPadding, PanelPadding));
		m_functionButtons->setButtonCallback(FunctionButtons::ButtonType::SaveFile, [this]() { saveAnimationTexture(); });
		m_functionButtons->setButtonCallback(FunctionButtons::ButtonType::SaveAsFile, [this]() { saveAs(); });
		m_functionButtons->setButtonCallback(FunctionButtons::ButtonType::OpenFile, [this]() { OpenAnimFile(); });
		m_functionButtons->setButtonCallback(FunctionButtons::ButtonType::EditData, [this]() { ToggleEditorDataPanel(); });

		// Initialize the frame selector
		m_frameSelector = new FrameSelector(m_windowSize);
		m_functionPanel->addChild(m_frameSelector);

		m_frameSelector->setFont(&m_font);
		m_frameSelector->initialize();
		m_frameSelector->setPosition(UI::Vec2(m_frameSelector->getPosition().x + m_frameSelector->getSize().x + 2.0f * PanelPadding, PanelPadding));
		m_frameSelector->setSelectedFrameData(&m_selectedFrame, m_data.AnimationCount);
		m_frameSelector->setOnSelectedFrameChangedCallback([this]() { updateAnimationRect(); });

		// Preview Button
		UI::Vec2 size = { PreviewButtonWidthInPercent, PreviewButtonHeightInPercent };
		size.x *= m_windowSize.x;
		size.y *= m_windowSize.y;

		UI::Button* previewButton = new UI::Button();
		previewButton->initialize();
		previewButton->setSize(size);
		previewButton->setColor(ButtonColor);
		previewButton->setPosition({ m_functionPanel->getSize().x - size.x - PanelPadding, PanelPadding });
		previewButton->setCallback([this]() { TogglePreviewPanel(); });

		UI::Vec2 labelPosition = previewButton->getPosition();
		labelPosition.x += PanelPadding;
		labelPosition.y += size.y * 0.2f;

		UI::Label* previewLabel = new UI::Label();
		previewLabel->initialize();
		previewLabel->setString(PreviewButtonText);
		previewLabel->setFont(m_font);
		previewLabel->setTextColor(LabelColor);
		previewLabel->setOrigin({ 0.0f, previewLabel->getSize().y / 2.0f });
		previewLabel->setPosition(labelPosition);

		m_functionPanel->addChild(previewButton);
		m_functionPanel->addChild(previewLabel);
	}

#pragma region AnimationPanel
	void Editor::initializeAnimationPanel()
	{
		const UI::Vec2 panelSize = UI::Vec2(
			m_windowSize.x - 2.0f * WindowPadding,
			EditorDataPanelHeightInPercent * m_windowSize.y
		);

		m_animationPanel = new UI::Panel();
		m_editorPanel.addChild(m_animationPanel);

		m_animationPanel->initialize();
		m_animationPanel->setSize(panelSize);
		m_animationPanel->setPosition(UI::Vec2(WindowPadding, m_windowSize.y - (panelSize.y + WindowPadding)));
		m_animationPanel->setColor(PanelClearColor);

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
		image->setTexture(m_textures[_imageID]);
		image->setPosition(_position);
		image->setSize(_size);
		image->setClearColor(BackgroundClearColor);

		// Creating the ImagePicker Module
		m_imagePickers[_imageID] = new UI::MousePickerModule();
		m_imagePickers[_imageID]->setSelectionCallback(std::bind(&Editor::processSelectedPosition, this, _imageID, m_imagePickers[_imageID]));
		m_imagePickers[_imageID]->setSelectionPixelSize(m_textures[_imageID]->getSize());
		m_imagePickers[_imageID]->setPickingZone(pickingZone);
		m_imagePickers[_imageID]->setSelectorSize({
			_size.x / m_animFrameSize.x,
			_size.y / m_animFrameSize.y
			});

		image->addModule(m_imagePickers[_imageID]);
		m_animationPanel->addChild(image);

		return image;
	}
#pragma endregion

#pragma region PreviewPanel
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
		sprite.setFrameDuration(m_data.FrameDuration);

		m_previewPanel->addModule(m_spriteModule);

		m_editorPanel.addChild(m_previewPanel);
	}

	void Editor::initializeEditorDataPanel()
	{
		const UI::Vec2 panelSize = UI::Vec2(
			m_windowSize.x - 2.0f * WindowPadding,
			AnimPanelHeightInPercent * m_windowSize.y
		);

		m_editorDataPanel = new EditorDataPanel();

		m_editorDataPanel->setColor(PanelClearColor);
		m_editorDataPanel->setSize(panelSize);
		m_editorDataPanel->setPosition(UI::Vec2(WindowPadding, m_windowSize.y - (panelSize.y + WindowPadding)));
		m_editorDataPanel->setData(&m_data);
		m_editorDataPanel->initialize();
		m_editorDataPanel->setTextFont(m_font);
		m_editorDataPanel->UpdateData();
		m_editorDataPanel->setVisible(false);

		m_editorPanel.addChild(m_editorDataPanel);
	}
#pragma endregion 
#pragma endregion 
}