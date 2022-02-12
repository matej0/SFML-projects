#pragma once
#include <SFML/Graphics.hpp>
#include "WindowData.h"
#include <iostream>
#include <Windows.h>
#include <random>
#include <deque>
#include <cmath>
#include <math.h>
#include <memory>
#include <algorithm>
using namespace sf;
#include "Debug.h"
#define PIPE_TEXTURE_WIDTH 104

class CPipe
{
public:
	float ComputeRandomHeight();
	void SpawnPipes(Texture & PipeTexture, float x);
	void Draw(RenderWindow * pWindow);
	void MovePipes();
	bool PipesAreOutOfScreen();
	void ResetPositions(bool bShouldResetPositions);
	void Think();
	Sprite m_Top, m_Bottom;
	float m_flStartPositionX = 0;
private:
};