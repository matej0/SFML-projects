#include "Debug.h"

void DEBUG::DrawString(sf::Vector2f vecPosition, int iCharSize, sf::Uint32 iStyle, sf::Font TextFont, sf::Color clrTextColor, sf::String strText, ...)
{
	static sf::RenderWindow* pRenderWindow = reinterpret_cast<sf::RenderWindow*>(g_WindowData.pRenderWindowPointer);
	sf::Text text(strText, TextFont, iCharSize);
	text.setPosition(vecPosition);
	text.setFillColor(clrTextColor);
	pRenderWindow->draw(text);
}