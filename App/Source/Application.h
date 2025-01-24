#pragma once

namespace sf
{
	class RenderWindow;
	class Event;
}

namespace Animation
{
	class Editor;
}

class Application
{
public:
	Application();
	~Application();
	void Run();

	bool isRunning() const;
private:
	void OnInitialize();
	void OnEvent(const sf::Event& _event);
	void OnUpdate(float _deltaTime);
	void OnRender();
	void OnShutdown();

private:
	sf::RenderWindow* m_window;

	Animation::Editor* m_editor;
};