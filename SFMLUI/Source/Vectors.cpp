#include <ostream>

#include "Vectors.h"

std::ostream& operator<<(std::ostream& os, const UI::Vec2& vec)
{
	os << "(" << vec.x << "; " << vec.y << ")";
	return os;
}

std::ostream& operator<<(std::ostream& os, const UI::Vec2i& vec)
{
	os << "(" << vec.x << "; " << vec.y << ")";
	return os;
}

std::ostream& operator<<(std::ostream& os, const UI::Vec2u& vec)
{
	os << "(" << vec.x << "; " << vec.y << ")";
	return os;
}

std::ostream& operator<<(std::ostream& os, const UI::PickingZone& vec)
{
	os << "(" << vec.left << "; " << vec.top << "; " << vec.width << "; " << vec.height << ")";
	return os;
}
