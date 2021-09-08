#pragma once
#include "SFML/Graphics.hpp"
#pragma warning( disable : 4244 ) 

struct CWindow
{
	unsigned int width;
	unsigned int height;
	sf::Font font;
    double curtime;
	float deltaTime;
};
extern CWindow g_WindowData;