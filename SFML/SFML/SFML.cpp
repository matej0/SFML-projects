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
using namespace sf;
CWindow g_WindowData;

int main()
{
	ContextSettings settings;
	settings.antialiasingLevel = 2;

	RenderWindow window(VideoMode(800, 800, 32), "FUCK NIGGERS!", Style::Default, settings);
	window.setFramerateLimit(60);

	g_WindowData.pRenderWindowPointer = &window;
	g_WindowData.width = window.getSize().x;
	g_WindowData.height = window.getSize().y;

	g_WindowData.center[0] = window.getSize().x / 2.f; 
	g_WindowData.center[1] = window.getSize().y / 2.f;

	if (!g_WindowData.font.loadFromFile("C:\\Windows\\Fonts\\Verdana.ttf"))
	{
		//idk
	}

	Clock deltaClock;

	CTank Tank(30.f, Color::White);
	CTarget Target;

	while (window.isOpen())
	{
		g_WindowData.deltaTime = deltaClock.restart().asSeconds();

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}

		window.clear();

		DrawCrosshair(&window);
		Tank.MouseMove();
		{
			Tank.SpawnBullet();
			Tank.SimulateBullet();
			Tank.DrawBullet(&window);

			Target.Think(Tank);
			Target.Draw(&window);
		}
		Tank.Draw(&window);

		Tank.DrawDebugOverlay(&window);
		
		window.display();
	}
}


