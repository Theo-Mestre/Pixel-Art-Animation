#include "corepch.h"

#include "Core/Editor.h"

#include "Application.h"

constexpr const char* TEXTURE_PATH = "Texture2.png";
constexpr const char* ANIMATION_PATH = "Animation.png";
constexpr uint32_t ANIM_COUNT_X = 1;
constexpr uint32_t ANIM_COUNT_Y = 1;

Application::Application()
	: m_window(nullptr)
	, m_editor(nullptr)
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
	return m_window != nullptr && m_window->isOpen();
}

void Application::OnInitialize()
{
	m_window = new sf::RenderWindow(sf::VideoMode(1080, 720), "App");

	Animation::EditorData editorData;
	editorData.Window = m_window;
	editorData.AnimationCount = { ANIM_COUNT_X, ANIM_COUNT_Y };
	editorData.AnimationPath = ANIMATION_PATH;
	editorData.TexturePath = TEXTURE_PATH;

	m_editor = new Animation::Editor(editorData);
	m_editor->initialize();
}

void Application::OnEvent(const sf::Event& _event)
{
	if (_event.type == sf::Event::Closed) m_window->close();

	m_editor->receiveEvent(_event);
}

void Application::OnUpdate(float _deltaTime)
{
	m_editor->update(_deltaTime);
}

void Application::OnRender()
{
	m_editor->draw();
}

void Application::OnShutdown()
{
	delete m_window;
	m_window = nullptr;
}
