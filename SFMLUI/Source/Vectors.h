#pragma once 

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>

namespace UI
{
	using Vec2 = sf::Vector2f;
	using Vec2i = sf::Vector2i;
	using Vec2u = sf::Vector2u;

	using PickingZone = sf::FloatRect;
}

std::ostream& operator<<(std::ostream& os, const UI::Vec2& vec);
std::ostream& operator<<(std::ostream& os, const UI::Vec2i& vec);
std::ostream& operator<<(std::ostream& os, const UI::Vec2u& vec);
std::ostream& operator<<(std::ostream& os, const UI::PickingZone& vec);