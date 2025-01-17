#pragma once

#include <functional>

#include "SFML/Graphics.hpp"

class SelectionCursor
{
public:
	SelectionCursor();
	~SelectionCursor();

	void OnInitialize(const std::string& _texturePath, const sf::IntRect& _selectionRect);
	void OnEvent(const sf::Event& _event);
	void OnUpdate(float _deltaTime);
	void OnRender(sf::RenderWindow* _window);

	inline void SetSelectionRect(const sf::IntRect& _selectionRect) { m_selectionZone = _selectionRect; }
	inline void SetSelected(bool _isSelected) { m_isSelected = _isSelected; }
	inline void SetShowSelection(bool _showSelection) { m_ShowCursor = _showSelection; }
	inline void SetScale(float _scale) { m_sprite.setScale(_scale, _scale); }
	inline void SetTextureSize(float _textureSize) { m_textureSize = _textureSize; }
	inline bool IsSelected() const { return m_isSelected; }
	inline bool DoesShowSelection() const { return m_ShowCursor; }

	inline void SetWindow(sf::RenderWindow* _window) { m_window = _window; }

	inline void SetClickCallback(const std::function<void()> _function) { m_onClick = _function; }
	sf::Vector2i GetSelectedPixel();
	sf::Vector2i GetHoveredPixel();

private:
	bool IsMouseInsideSelectionZone();
private:
	sf::IntRect m_selectionZone;

	sf::Texture m_texture;
	sf::Sprite m_sprite;

	sf::Vector2u m_position;
	
	bool m_isSelected;
	bool m_ShowCursor;

	float m_textureSize;

	sf::RenderWindow* m_window;

	std::function<void()> m_onClick;
};

