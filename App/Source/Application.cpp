#include "corepch.h"

#include "UIElement.h"

#include "Application.h"

constexpr const char* TEXTURE_PATH = "Texture.png";
constexpr const char* ANIMATION_PATH = "Animation.png";
constexpr uint32_t ANIM_COUNT_X = 1;
constexpr uint32_t ANIM_COUNT_Y = 1;

Application::Application()
	: m_window(nullptr)
	, m_editor()
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

	while (isRunning())
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

bool Application::isRunning() const
{
	return m_window && m_window->isOpen();
}

void Application::OnInitialize()
{
	m_window = new sf::RenderWindow(sf::VideoMode(1080, 720), "App");

	Animation::EditorData editorData;
	editorData.Window = m_window;
	editorData.AnimationCount = { ANIM_COUNT_X, ANIM_COUNT_Y };
	editorData.AnimationPath = ANIMATION_PATH;
	editorData.TexturePath = TEXTURE_PATH;

	m_editor.setData(editorData);
	m_editor.initialize();

	//m_sprite = new Animation::AnimatedSprite(
	//	{
	//		ANIM,
	//		TEX,
	//		sf::Vector2u(48, 48),
	//		sf::Vector2u(ANIM_NUMBER),
	//		0.1f
	//	}
	//);
	//
	//m_sprite->setPosition(540.0f, 360.0f);
	//m_sprite->setOrigin(24.0f, 24.0f);
	//m_sprite->setScale(6.0f, 6.0f);

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
		m_editor.receiveEvent(_event);
	}
}

void Application::OnUpdate(float _deltaTime)
{
	if (m_switchDisplay)
	{
		m_editor.update(_deltaTime);
	}
	else
	{
		//m_sprite->Update(_deltaTime);
	}
}

void Application::OnRender()
{
	if (m_switchDisplay)
	{
		m_editor.draw();
	}
	else
	{
		//m_window->draw(*m_sprite);
	}
}

void Application::OnShutdown()
{
	delete m_window;
	m_window = nullptr;
}
