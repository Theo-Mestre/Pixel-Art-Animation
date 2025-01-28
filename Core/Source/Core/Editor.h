#pragma once

#include <array>

#include "SFML/Graphics.hpp"

#include "Panel.h"

#include "SelectionCursor.h"

namespace UI
{
	class Button;
	class Image;
	class PanelRenderer;
	class MousePickerModule;
}

namespace Animation
{
	class AnimatedSpriteModule;

	// Contains the data needed to initialize and use the Editor
	struct EditorData
	{
		sf::RenderWindow* Window = nullptr;

		// Animation Data
		UI::Vec2u AnimationCount;
		std::string AnimationPath;
		std::string TexturePath;
	};

	namespace SelectedImage
	{
		enum SelectedImage
		{
			Animation,
			Texture,

			Count
		};
	}

	class Editor
	{
	public:
		Editor() = default;
		Editor(const EditorData& _editorData);

		~Editor();

		void initialize();
		void receiveEvent(const sf::Event& _event);
		void update(float _deltaTime);
		void draw();

		// Set the Editor Data 
		void setData(const EditorData& _editorData);

	private:
		void saveAnimationFile();

		UI::Vec2i fromScreenToTextureCoord(const UI::Vec2& _position, const UI::Vec2& _size, const UI::Vec2u& _textureSize);
		void processSelectedPosition(SelectedImage::SelectedImage _selectedImage, UI::MousePickerModule* _picker);
		void updateAnimationImage();
		void updateImageData();
		sf::Color getCoordColor(const UI::Vec2i& _position);
		void updateAnimationRect();

		// UI Initialization
		void initializeUI();
		void initializeTextures();
		void initializeFunctionPanel();
		void initializeFunctionButtons();
		void initializeAnimEditionButtons();

		void initializeAnimationPanel();
		void initializeAnimationImages();
		UI::Image* createAnimationImage(sf::Texture* _tex, const UI::Vec2& _position, const UI::Vec2& _size, const std::function<void(UI::MousePickerModule*)>& _callback);

		void initializePreviewPanel();
	private:
		AnimatedSpriteModule* m_spriteModule;

		EditorData m_data;
		UI::Vec2 m_windowSize;

		sf::Image m_animationImage;

		sf::Vector2u m_textureSize;
		UI::Vec2 m_animTotalSize;
		UI::Vec2 m_animFrameSize;
		UI::Vec2u m_selectedFrame;

		// UI
		UI::Panel m_editorPanel;
		UI::Panel* m_functionPanel = nullptr;
		UI::Panel* m_animationPanel = nullptr;
		UI::PanelRenderer* m_previewPanel = nullptr;

		// PanelSettings
		float m_functionPanelHeightInPercent = 20.0f;

		// Images 
		UI::Image* m_animationImageUI = nullptr;
		UI::Image* m_textureImageUI = nullptr;
		UI::MousePickerModule* m_imagePickers[SelectedImage::Count] = { nullptr };
		sf::Texture* m_texture[SelectedImage::Count] = { nullptr };

		// ImageSettings
		float m_imagePadding = 10.0f;

		// LayoutSettings
		float m_padding = 10.0f;

		// StyelSettings
		sf::Font m_font;
		const sf::Color m_backgroundClearColor = sf::Color(20, 20, 20, 255);
		const sf::Color m_panelClearColor = sf::Color(50, 50, 50, 255);
	};
}