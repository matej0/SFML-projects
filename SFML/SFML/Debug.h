#pragma once
#include <SFML/Graphics.hpp>
#include "WindowData.h"

namespace DEBUG
{
	void DrawString(sf::Vector2f vecPosition, int iCharSize, sf::Uint32 iStyle, sf::Font TextFont, sf::Color clrTextColor, sf::String strText, ...);
}