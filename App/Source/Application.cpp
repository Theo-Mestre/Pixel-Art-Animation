#include "corepch.h"
#include "Application.h"

Application::Application()
	: m_window(nullptr)
	, m_editor("TextureFinal.png", "AnimationTest.png", sf::Vector2u(6, 1), m_window)
	, m_switchDisplay(true)
{
}

Application::~Application()
{
}

void Application::Run()
{
	sf::Event event;
	sf::Clock clock;
	float deltaTime = 0.0f;

	OnInitialize();

	while (IsRunning())
	{
		deltaTime = clock.restart().asSeconds();

		while (m_window->pollEvent(event)) OnEvent(event);

		m_window->clear(sf::Color(65, 65, 65, 255));

		OnUpdate(deltaTime);
		OnRender();

		m_window->display();
	}
	OnShutdown();
}

bool Application::IsRunning() const
{
	return m_window && m_window->isOpen();
}

void Application::OnInitialize()
{
	m_window = new sf::RenderWindow(sf::VideoMode(1080, 720), "App");

	sf::Texture animation;
	animation.loadFromFile("AnimationTest.png");
	m_sprite.SetAnimation(animation);

	sf::Texture texture;
	texture.loadFromFile("TextureFinal.png");
	m_sprite.SetTexture(texture);

	m_sprite.SetFrameSize(sf::Vector2u(48, 48));
	m_sprite.SetFrameNumber(sf::Vector2u(1, 1));
	m_sprite.SetCurrentFrame(sf::Vector2u(0, 0));
	m_sprite.SetFrameDuration(0.1f);

	m_sprite.setPosition(540.0f, 360.0f);
	m_sprite.setOrigin(24.0f, 24.0f);
	m_sprite.setScale(6.0f, 6.0f);

	m_editor.SetWindow(m_window);
	m_editor.OnInitialize();
}

void Application::OnEvent(const sf::Event& _event)
{
	if (_event.type == sf::Event::Closed) m_window->close();
	if (_event.type == sf::Event::KeyPressed)
	{
		if (_event.key.code == sf::Keyboard::Tab)
		{
			m_switchDisplay = !m_switchDisplay;
		}
	}
	if (m_switchDisplay)
	{
		m_editor.OnEvent(_event);
	}
}

void Application::OnUpdate(float _deltaTime)
{
	if (m_switchDisplay)
	{
		m_editor.OnUpdate(_deltaTime);
	}
	else 
	{
		m_sprite.Update(_deltaTime);
	}
}

void Application::OnRender()
{
	if (m_switchDisplay)
	{
		m_editor.OnRender();
	}
	else
	{
		m_window->draw(m_sprite);
	}
}

void Application::OnShutdown()
{
	delete m_window;
	m_window = nullptr;
}
