#include "corepch.h"
#include "Editor.h"

Editor::Editor(const std::string& _texturePath,
	const std::string& _animationPath,
	sf::Vector2u _animationSize,
	sf::RenderWindow* _window)
	: m_window(_window)
	, m_animationCount(_animationSize)
{
	m_panels[Animation].Texture.loadFromFile(_animationPath);
	m_panels[Texture].Texture.loadFromFile(_texturePath);
	m_textureSize = m_panels[Texture].Texture.getSize();
}

Editor::~Editor()
{
	m_window = nullptr;
}

void Editor::OnInitialize()
{
	sf::Vector2u windowSize = m_window->getSize();
	m_padding = 10.0f;

	m_panelSize.x = (windowSize.x - (MAX_PANELS + 1) * m_padding) / MAX_PANELS;
	m_panelSize.y = m_panelSize.x;

	m_scaleFromTexToPanel = (float)m_panelSize.x / (float)m_textureSize.x;

	for (int i = 0; i < MAX_PANELS; i++)
	{
		m_panels[i].Position = sf::Vector2f(m_padding + (m_panelSize.x + m_padding) * i, windowSize.y - m_padding - m_panelSize.y);
		m_panels[i].ClearColor = sf::Color(0, 0, 0, 255);
		m_panels[i].RenderTexture.create(m_textureSize.x, m_textureSize.y);
	}

	m_animationImage.create(m_textureSize.x, m_textureSize.y);
	m_animationImage.copy(m_panels[Animation].Texture.copyToImage(), 0, 0);

	m_animationCursor.SetTextureSize(m_textureSize.x);
	m_animationCursor.SetWindow(m_window);
	m_animationCursor.OnInitialize("Selection.png", sf::IntRect
	(
		m_panels[Animation].Position.x,
		m_panels[Animation].Position.y,
		m_panelSize.x,
		m_panelSize.y
	));

	m_textureCursor.SetTextureSize(m_textureSize.x);
	m_textureCursor.SetWindow(m_window);
	m_textureCursor.OnInitialize("Selection.png", sf::IntRect
	(
		m_panels[Texture].Position.x,
		m_panels[Texture].Position.y,
		m_panelSize.x,
		m_panelSize.y
	));
	m_textureCursor.SetClickCallback([&]()
		{
			m_textureCursor.SetSelected(false);

			if (!m_animationCursor.IsSelected()) return;

			sf::Vector2i pixel = m_animationCursor.GetSelectedPixel();
			auto hover = m_textureCursor.GetHoveredPixel();
			std::cout << "hover : { " << hover.x << " : " << hover.y << " }" << std::endl;
			sf::Vector2f coord = NormalizeCoordinates(m_textureCursor.GetHoveredPixel());
			std::cout << "coords : { " << coord.x << " : " << coord.y << " }" << std::endl;
			sf::Color color(coord.x * 255.0f, coord.y * 255.0f, 0.0f, 255.0f);
			std::cout << "coords : { " << coord.x * 255.0f << " : " << coord.y * 255.0f << " }" << std::endl;
			m_animationImage.setPixel(pixel.x, pixel.y, color);
			m_panels[Animation].Texture.loadFromImage(m_animationImage);
			std::cout << "coords : { " << color.r / 255.0f * 48.0f << " : " << color.g / 255.0f * 48.0f << " }" << std::endl;
			m_animationCursor.SetSelected(false);
		});
}
void Editor::OnEvent(const sf::Event& _event)
{
	if (_event.type == sf::Event::KeyPressed)
	{
		if (_event.key.code == sf::Keyboard::Space)
		{
			m_animationImage.saveToFile("AnimationTest.png");
		}
	}

	m_animationCursor.OnEvent(_event);
	m_textureCursor.OnEvent(_event);
}

void Editor::OnUpdate(float _deltaTime)
{
	m_animationCursor.OnUpdate(_deltaTime);
	m_textureCursor.OnUpdate(_deltaTime);
}

void Editor::OnRender()
{
	static sf::Sprite sprite;

	for (int i = 0; i < MAX_PANELS; i++)
	{
		auto& panel = m_panels[i];

		if (panel.Texture.getSize() == sf::Vector2u(0, 0)) continue;

		sprite.setTexture(panel.Texture);

		panel.RenderTexture.clear(panel.ClearColor);
		panel.RenderTexture.draw(sprite);
		panel.RenderTexture.display();

		m_panelSprite.setTexture(panel.RenderTexture.getTexture());
		m_panelSprite.setPosition(panel.Position);
		m_panelSprite.setScale(m_scaleFromTexToPanel, m_scaleFromTexToPanel);
		m_window->draw(m_panelSprite);
	}

	m_animationCursor.OnRender(m_window);
	m_textureCursor.OnRender(m_window);
}

sf::Vector2f Editor::NormalizeCoordinates(sf::Vector2i _coord)
{
	return  sf::Vector2f((float)_coord.x / (float)m_textureSize.x, (float)_coord.y / (float)m_textureSize.y);
}
