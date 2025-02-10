#pragma once

#include <array>
#include <functional>

#include "SFML/Graphics.hpp"

#include "Panel.h"

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
	class FrameSelector;
	class FunctionButtons;
	class EditorDataPanel;

	// Contains the data needed to initialize and use the Editor
	struct EditorData
	{
		sf::RenderWindow* Window = nullptr;

		// Animation Data
		UI::Vec2u AnimationCount;
		std::string AnimationPath;
		std::string TexturePath;
		float FrameDuration = 0.1f;
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
		void setResetCallback(const std::function<void(EditorData*)>& _callback) { m_onReset = _callback; }

	private:
		void saveAnimationTexture();
		void saveAs();

		void processSelectedPosition(SelectedImage::SelectedImage _selectedImage, UI::MousePickerModule* _picker);
		void updateAnimationImage(const UI::Vec2i& _animCoord, const UI::Vec2i& _texCoord);
		void updateImageData();
		sf::Color getCoordColor(const UI::Vec2i& _position);
		void updateAnimationRect();

		void SaveAnimFile(const std::string& _path);
		void OpenAnimFile();
		void ReadAnimFile(const std::string& _path);
		void TogglePreviewPanel();
		void ToggleEditorDataPanel();

		// UI Initialization
		void initializeUI();
		void initializeTextures();
		void initializeFunctionPanel();
		void initializeFunctionButtons();

		void initializeAnimationPanel();
		void initializeAnimationImages();
		UI::Image* createAnimationImage(SelectedImage::SelectedImage _imageID, const UI::Vec2& _position, const UI::Vec2& _size);

		void initializePreviewPanel();

		void initializeEditorDataPanel();
	private:
		AnimatedSpriteModule* m_spriteModule;

		EditorData m_data;
		UI::Vec2 m_windowSize;

		sf::Image m_animationImage;

		sf::Vector2u m_textureSize;
		UI::Vec2 m_animTotalSize;
		UI::Vec2 m_animFrameSize;
		UI::Vec2i m_selectedFrame;

		// UI
		UI::Panel m_editorPanel;
		UI::Panel* m_functionPanel = nullptr;
		UI::Panel* m_animationPanel = nullptr;
		UI::PanelRenderer* m_previewPanel = nullptr;

		// Images 
		UI::Image* m_animationImageUI = nullptr;
		UI::Image* m_textureImageUI = nullptr;
		UI::MousePickerModule* m_imagePickers[SelectedImage::Count] = { nullptr };
		sf::Texture* m_textures[SelectedImage::Count] = { nullptr };

		// FunctionButtons
		FunctionButtons* m_functionButtons = nullptr;

		// FrameSelector
		FrameSelector* m_frameSelector = nullptr;

		// EditorDataPanel
		EditorDataPanel* m_editorDataPanel = nullptr;

		// StyelSettings
		sf::Font m_font;

		std::function<void(EditorData*)> m_onReset;
	};
}