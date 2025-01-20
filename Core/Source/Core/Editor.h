#pragma once

#include <array>

#include "SFML/Graphics.hpp"

#include "Panel.h"

#include "SelectionCursor.h"

namespace Animation
{
	struct EditorData
	{
		sf::RenderWindow* Window;
	};

	class Editor
	{
	public:
		Editor(const EditorData& _editorData);

		Editor(const std::string& _texturePath,
			const std::string& _animationPath,
			sf::Vector2u _animationSize,
			sf::RenderWindow* _window);

		~Editor();

		void initialize();
		void receiveEvent(const sf::Event& _event);
		void update(float _deltaTime);
		void draw();

		void setData(const EditorData& _editorData);

	private:
		sf::Vector2f normalizeCoordinates(sf::Vector2i _coord);

		void initializePanels();
		void initializeFunctionPanel();
		void initializeFunctionButtons();

		void initializeAnimationPanel();

	private:
		EditorData m_editorData;
		UI::Vec2 m_windowSize;

		sf::Image m_animationImage;

		UI::Panel m_editorPanel;
		UI::Panel* m_animationPanel;
		UI::Panel* m_functionPanel;

		// PanelSettings
		float m_functionPanelHeightInPercent = 20.0f;

		SelectionCursor m_animationCursor;
		SelectionCursor m_textureCursor;

		float m_scaleFromTexToPanel;
		float m_scaleFromPanelToTex;

		sf::Sprite m_panelSprite;
		sf::Vector2u m_panelSize;
		sf::Vector2u m_textureSize;
		sf::Vector2u m_animationCount;

		float m_padding;

		sf::Font m_font;
	};
}