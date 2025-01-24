#pragma once

#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/Transformable.hpp"
#include "SFML/Graphics/VertexArray.hpp"

#include "Vectors.h"

namespace sf
{
	class Event;
}

namespace UI
{
	class Module;

	class UIElement
		: public sf::Drawable
		, public sf::Transformable
	{
	public:
		UIElement();
		virtual ~UIElement();

		virtual void initialize() = 0;
		virtual void receiveEvent(const sf::Event& _event) = 0;
		virtual void update(float _deltaTime) = 0;

		void setSize(const sf::Vector2f& _size);
		const sf::Vector2f& getSize() const;

		void setVisible(bool _visible);
		bool isVisible() const;

		void setReceiveEvents(bool _event);
		bool isReceivingEvents() const;

		void setHovered(bool _hovered);
		bool isHovered() const;

		void setColor(const sf::Color& _color);

		// Modules
		void addModule(Module* _module);
		void updateModules(float deltaTime);
		void handleEventModules(const sf::Event& event);
		void drawModules(sf::RenderTarget& _target, sf::RenderStates _states) const;

		void removeModule(Module* _module);
		void removeAllModules();

	protected:
		virtual void onSizeChanged();

		virtual void updateTextureCoords(const sf::Vector2f _size);

	protected:
		sf::VertexArray m_quad;

		std::vector<Module*> m_modules;

		Vec2 m_size;

		bool m_visible;
		bool m_receiveEvents;
		bool m_hovered;

	public:
		template <typename T>
		T* getFirstModuleOfType() const
		{
			static_assert(std::is_base_of<Module, T>::value, "T must derive from Module");

			for (auto module : m_modules)
			{
				if (dynamic_cast<T*>(module) != nullptr)
				{
					return static_cast<T*>(module);
				}
			}
		}
	};
}