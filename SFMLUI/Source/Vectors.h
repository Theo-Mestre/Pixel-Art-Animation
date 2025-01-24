#pragma once 
#include <ostream>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>


namespace UI
{
	using Vec2 = sf::Vector2f;
	using Vec2i = sf::Vector2i;
	using Vec2u = sf::Vector2u;

	using PickingZone = sf::FloatRect;
}

inline std::ostream& operator<<(std::ostream& os, const UI::Vec2& vec)
{
	os << "(" << vec.x << "; " << vec.y << ")";
	return os;
}