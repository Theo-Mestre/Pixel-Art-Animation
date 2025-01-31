#pragma once

#include <SFML/Graphics/Color.hpp>

#include "Vectors.h"

namespace Animation
{
	namespace LayoutSettings
	{
		constexpr float WindowPadding = 20.0f;
		constexpr float PanelPadding = 10.0f;

		// Percent size are calculated based on a size of each element with a 1920*1080 resolution (see figma board)
		constexpr float FunctionPanelHeightInPercent = 285 * 100 / 1080 * 0.01;
		constexpr float AnimPanelHeightInPercent = 720 * 100 / 1080 * 0.01;
		constexpr float ImagesHeightInPercent = 700 * 100 / 1080 * 0.01;

		const sf::Color BackgroundClearColor = sf::Color(20, 20, 20, 255);
		const sf::Color PanelClearColor = sf::Color(65, 65, 65, 255);

		// Frame Selector
		constexpr float FrameSelectorHeightInPercent = 265 * 100 / 1080 * 0.01;
		constexpr const char* FrameSelectorTexturePath = "Arrows.png";
	}
}