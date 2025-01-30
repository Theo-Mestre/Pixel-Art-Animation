#pragma once

#include <SFML/Graphics/Color.hpp>

#include "Vectors.h"

namespace Animation
{
	namespace LayoutSettings
	{
		constexpr float WindowPadding = 25.0f;
		constexpr float PanelPadding = 10.0f;

		// Percent size are calculated based on a 1920*1080 resolution (see figma board)
		constexpr float FunctionPanelHeightInPercent = 285 * 100 / (1080 - 3 * WindowPadding) * 0.01;
		constexpr float AnimPanelHeightInPercent = 720 * 100 / (1080 - 3 * WindowPadding) * 0.01;
		constexpr float ImagesHeightInPercent = 700 * 100 / (1080 - 2 * PanelPadding) * 0.01;

		const sf::Color BackgroundClearColor = sf::Color(20, 20, 20, 255);
		const sf::Color PanelClearColor = sf::Color(65, 65, 65, 255);


	}
}