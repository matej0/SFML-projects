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
#include "HardGame.h"
//#include "Clock.h"
//#include "Snake.h"
#include "GUI.h"
using namespace sf;
CWindow g_WindowData;

int main()
{
	ContextSettings settings;
	settings.antialiasingLevel = 2;

	RenderWindow window(VideoMode(400, 400, 32), "FUCK NIGGERS!");
	window.setFramerateLimit(60);


	g_WindowData.width = window.getSize().x;
	g_WindowData.height = window.getSize().y;

	if (!g_WindowData.font.loadFromFile("C:\\Windows\\Fonts\\Verdana.ttf"))
	{
		//idk
	}

	Clock deltaClock;
	CPlayer penis;

	const int tiles[] =
	{
		0, 1, 2, 1, 2, 1, 0,
		0, 2, 1, 2, 1, 2, 0,
		0, 1, 2, 1, 2, 1, 0,
		0, 2, 1, 2, 1, 2, 0, 
		0, 1, 2, 1, 2, 1, 0,
		0, 2, 1, 2, 1, 2, 0,
		0, 1, 2, 1, 2, 1, 0,
		0, 2, 1, 2, 1, 2, 0,
		0, 1, 2, 1, 2, 1, 0,
		0, 2, 1, 2, 1, 2, 0,
		0, 1, 2, 1, 2, 1, 0,
		0, 2, 1, 2, 1, 2, 0,
	};

	
	CWorld world("cze.png", tiles, 7, 12, { 32, 32 });
	gTileMapProps.width = 7 * 32;
	gTileMapProps.height = 12 * 32;

	CPlayer player;

	for (int i = 0; i < 5; i++)
	{
		CObstacle obstacle(vPos);
		rgObstacles.push_back(obstacle);
		vPos.x += 32;

		if (i % 2 == 0)
			vPos.y = gTileMapProps.height - 18.f;
		else
			vPos.y = 18.f;

		if (rgObstacles.at(i).getPosition().y == gTileMapProps.height - 18.f)
			rgObstacles.at(i).m_bShouldStartWithInverseVelocity = true;
		else
			rgObstacles.at(i).m_bShouldStartWithInverseVelocity = false;
	}

	for (int i = 0; i < rgObstacles.size(); i++)
	{
		rgObstacles[i].StartMoving();
	}

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

		window.draw(world);

		player.Think();
		player.Draw(&window);
	
		for (int i = 0; i < rgObstacles.size(); i++)
		{
			rgObstacles[i].Think();
			rgObstacles[i].Draw(&window);

			if (player.HitObstacle(rgObstacles[i]))
				ResetGame(player);

		}


		window.display();
	}
}


