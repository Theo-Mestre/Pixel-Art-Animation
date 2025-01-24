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
		sf::Vector2f normalizeCoordinates(sf::Vector2i _coord);

		void saveAnimationFile();

		void updateImageData();

		// UI Initialization
		void initializeUI();
		void initializeFunctionPanel();
		void initializeFunctionButtons();

		void initializeAnimationPanel();
		void initializeAnimationImages();

		void initializePreviewPanel();
	private:
		AnimatedSpriteModule* m_spriteModule;

		EditorData m_data;
		UI::Vec2 m_windowSize;

		sf::Image m_animationImage;

		SelectionCursor m_animationCursor;
		SelectionCursor m_textureCursor;

		float m_scaleFromTexToPanel;
		float m_scaleFromPanelToTex;

		sf::Vector2u m_textureSize;

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