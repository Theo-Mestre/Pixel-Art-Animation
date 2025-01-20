#pragma once

#include "Core/AnimatedSprite.h"
#include "Core/Editor.h"

namespace sf
{
	class RenderWindow;
	class Event;
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

	sf::Texture m_whiteTexture;

	Animation::AnimatedSprite* m_sprite;
	Animation::Editor m_editor;

	bool m_switchDisplay;
};