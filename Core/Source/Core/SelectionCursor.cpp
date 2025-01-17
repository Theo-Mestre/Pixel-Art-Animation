#include "corepch.h"
#include "SelectionCursor.h"

SelectionCursor::SelectionCursor()
	: m_isSelected(false)
	, m_ShowCursor(true)
	, m_position(0.0f, 0.0f)
	, m_selectionZone(0, 0, 0, 0)
	, m_textureSize(0.0f)
	, m_window(nullptr)
{
}

SelectionCursor::~SelectionCursor()
{
	m_window = nullptr;
}

void SelectionCursor::OnInitialize(const std::string& _texturePath, const sf::IntRect& _selectionRect)
{
	m_texture.loadFromFile(_texturePath);
	m_sprite.setTexture(m_texture);
	
	m_selectionZone = _selectionRect;

	m_position = (sf::Vector2u)_selectionRect.getPosition();

	float scale = m_selectionZone.width / m_texture.getSize().x / m_textureSize;
	m_sprite.setScale(scale, scale);
}

void SelectionCursor::OnEvent(const sf::Event& _event)
{
	if (_event.type == sf::Event::MouseButtonPressed &&
		IsMouseInsideSelectionZone())
	{
		m_isSelected = !m_isSelected;

		if (m_isSelected && m_onClick)
		{
			m_onClick();
		}
	}
}

void SelectionCursor::OnUpdate(float _deltaTime)
{
	if (m_isSelected) return;

	if (!IsMouseInsideSelectionZone()) return;
	
	sf::Vector2f mousePosition = (sf::Vector2f)sf::Mouse::getPosition(*m_window);	

	mousePosition -= (sf::Vector2f)m_position;

	float scaleFromPanelToTex = (float)m_textureSize / m_selectionZone.width;

	mousePosition.x *= scaleFromPanelToTex;
	mousePosition.y *= scaleFromPanelToTex;

	m_sprite.setPosition((sf::Vector2f)m_position + sf::Vector2f(
		(int)mousePosition.x * m_selectionZone.width / m_textureSize,
		(int)mousePosition.y * m_selectionZone.height / m_textureSize));
}

void SelectionCursor::OnRender(sf::RenderWindow* _window)
{
	if (!m_ShowCursor) return;

	_window->draw(m_sprite);
}

sf::Vector2i SelectionCursor::GetSelectedPixel()
{
	if (m_isSelected == false) return { -1, -1 };

	sf::Vector2f spritePosition = m_sprite.getPosition();
	spritePosition -= (sf::Vector2f)m_position;

	float scaleFromPanelToTex = (float)m_textureSize / m_selectionZone.width;

	spritePosition.x *= scaleFromPanelToTex;
	spritePosition.y *= scaleFromPanelToTex;
	return (sf::Vector2i)spritePosition;
}

sf::Vector2i SelectionCursor::GetHoveredPixel()
{
	if (!IsMouseInsideSelectionZone()) return { -1, -1 };

	sf::Vector2f mousePosition = (sf::Vector2f)sf::Mouse::getPosition(*m_window);
	mousePosition -= (sf::Vector2f)m_position;

	float scaleFromPanelToTex = (float)m_textureSize / m_selectionZone.width;

	mousePosition.x *= scaleFromPanelToTex;
	mousePosition.y *= scaleFromPanelToTex;
	return (sf::Vector2i)mousePosition;
}

bool SelectionCursor::IsMouseInsideSelectionZone()
{
	sf::Vector2f mousePosition = (sf::Vector2f)sf::Mouse::getPosition(*m_window);
	
	if (mousePosition.x < m_position.x ||
		mousePosition.x > m_position.x + m_selectionZone.width ||
		mousePosition.y < m_position.y ||
		mousePosition.y > m_position.y + m_selectionZone.height)
	{
		m_ShowCursor = false;
		return false;
	}
	m_ShowCursor = true;
	return true;
}
