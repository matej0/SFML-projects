#pragma once
#include <SFML/Graphics.hpp>
#include "WindowData.h"
#include "Debug.h"
#include "Bird.h"
#include "Pipe.h"
#include <list>
#include <deque>

class CBirdWorld
{
public:
	void LoadTextures();
	void CreateAllPipes();
	void CleanUpPipes();
	void PipesThink();
	void DrawAllPipes(RenderWindow * pWindow);
	bool ShouldLose(CBird * Bird);
	void DrawAllElements(RenderWindow * pWindow, CBird * Bird);
	void Initialize();
	void Play(RenderWindow * pWindow);
	std::list<CPipe> m_Pipes;
	CBird* m_pBird;
	std::deque<Texture> m_BirdTextures;
	Texture m_PipeTexture;
private:
	bool m_bShouldLose = false;
	
};