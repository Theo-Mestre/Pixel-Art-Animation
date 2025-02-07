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
		constexpr const char* FrameSelectorTexturePath = "Textures/Arrows.png";

		// Preview Button
		constexpr float PreviewButtonWidthInPercent = 200 * 100 / 1920 * 0.01;
		constexpr float PreviewButtonHeightInPercent = 100 * 100 / 1080 * 0.01;
		constexpr const char* PreviewButtonText = "Preview";

		constexpr const char* FontPath = "Fonts/Font.ttf";

		// Function Buttons
		constexpr const char* IconsPath[4] =
		{
			"Textures/SaveIcon.png",
			"Textures/OpenIcon.png",
			"Textures/SaveAsIcon.png",
			"Textures/EditIcon.png"
		};

		// Editor Data Panel
		constexpr float EditorDataPanelHeightInPercent = 720 * 100 / 1080 * 0.01;
		constexpr float LabelWidthInPercent = 1600 * 100 / 1920 * 0.01;
		constexpr float LabelHeightInPercent = 30 * 0.01;

		const sf::Color LabelColor = sf::Color(255, 255, 255, 255);
		const sf::Color LabelBackgroundColor = sf::Color(50, 50, 50, 255);
		const sf::Color ButtonColor = sf::Color(100, 100, 100, 255);

		constexpr const char* OpenButtonTexturePath = "Textures/OpenButton.png";
		constexpr const char* OpenButtonText = "Open";
		constexpr const char* AnimCountText = "Animation Count:";
		constexpr const char* DimensionText[2] = { "X:", "Y:" };

		constexpr float AnimCountLabelWidthInPercent = 750 * 100 / 1920 * 0.01;
		constexpr float AnimCountLabelHeightInPercent = 15 * 0.01;
		constexpr uint32_t AnimCountButtonNumber = 4;

	}
}