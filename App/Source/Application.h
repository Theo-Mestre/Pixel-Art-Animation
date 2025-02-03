#pragma once

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

	// The editor is reset when a texture is changed so everything is updated
	// This will change in the future to only update the necessary data
	void RequireEditorReset(Animation::EditorData* _editorData);
	void CreateEditor();

private:
	sf::RenderWindow* m_window;

	Animation::Editor* m_editor;

	Animation::EditorData* m_newEditorData;
	bool m_requireEditorReset;
};