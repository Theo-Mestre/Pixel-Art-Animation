#pragma once

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Color.hpp>

#include "Panel.h"

namespace UI
{
	class PanelRenderer
		: public Panel
	{
	public:
		PanelRenderer();
		virtual ~PanelRenderer();

		virtual void initialize() override;
		virtual void receiveEvent(const sf::Event& _event) override;
		virtual void update(float _deltaTime) override;

		void setClearColor(const sf::Color& _color);

		sf::RenderTarget& operator()();
	private:
		virtual void draw(sf::RenderTarget& _target, sf::RenderStates _states) const override;
		void renderChildrenOnTexture(sf::RenderStates _states);

		virtual void onSizeChanged() override;
	private:
		sf::RenderTexture m_renderTexture;
		sf::Color m_clearColor;
	};
}