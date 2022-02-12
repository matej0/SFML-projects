#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Network.hpp>
#include <Windows.h>
#include <deque>
#include <sstream>
#include <array>
#include <random>
#include "WindowData.h"
#include "Tank.h"

#include "BirdWorld.h"

using namespace sf;
CWindow g_WindowData;

int main()
{
	ContextSettings settings;
	settings.antialiasingLevel = 2;

	RenderWindow window(VideoMode(800, 600, 32), "XD!", Style::Default, settings);
	window.setFramerateLimit(144);

	g_WindowData.pRenderWindowPointer = &window;
	g_WindowData.width = window.getSize().x;
	g_WindowData.height = window.getSize().y;

	g_WindowData.center.x = window.getSize().x / 2.f; 
	g_WindowData.center.y = window.getSize().y / 2.f;

	if (!g_WindowData.font.loadFromFile("C:\\Windows\\Fonts\\Verdana.ttf"))
	{
		//idk
	}

	Clock CurTimeClock;
	g_WindowData.pCurTime = &CurTimeClock;
	Clock deltaClock;

	/*Texture* pTexture = new Texture();
	
	if (!pTexture->loadFromFile("tank.bmp", IntRect(25, 10, (256 - 45), (256 - 10))))
	{
		std::cout << "Couldnt load tank texture! (tank.bmp)\n";
	}
	*/
	
	CBirdWorld World;

	World.LoadTextures();
	World.Initialize();

	while (window.isOpen())
	{
		if (CurTimeClock.getElapsedTime().asSeconds() >= FLT_MAX)
			CurTimeClock.restart();

		g_WindowData.deltaTime = deltaClock.restart().asSeconds();


		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}

		window.clear();

		World.Play(&window);

		window.display();
	}
}


