#pragma once

#include <array>

#include "SFML/Graphics.hpp"

#include "SelectionCursor.h"

static constexpr unsigned int MAX_PANELS = 2;

class Editor
{
public:
	Editor(const std::string& _texturePath,
		const std::string& _animationPath,
		sf::Vector2u _animationSize,
		sf::RenderWindow* _window);

	~Editor();

	void OnInitialize();
	void OnEvent(const sf::Event& _event);
	void OnUpdate(float _deltaTime);
	void OnRender();

	inline void SetWindow(sf::RenderWindow* _window) { m_window = _window; }

private:
	sf::Vector2f NormalizeCoordinates(sf::Vector2i _coord);

private:
	sf::RenderWindow* m_window;

	sf::Image m_animationImage;

	SelectionCursor m_animationCursor;
	SelectionCursor m_textureCursor;

	float m_scaleFromTexToPanel;
	float m_scaleFromPanelToTex;

	enum PanelType
	{
		Animation,
		Texture,
		Preview
	};

	struct Panel
	{
		sf::Vector2f Position;
		sf::Color ClearColor;
		sf::Texture Texture;
		sf::RenderTexture RenderTexture;
	};

	sf::Sprite m_panelSprite;
	sf::Vector2u m_panelSize;
	sf::Vector2u m_textureSize;
	sf::Vector2u m_animationCount;

	std::array<Panel, MAX_PANELS> m_panels;

	float m_padding;
};