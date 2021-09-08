#pragma once
#include <SFML/Graphics.hpp>
#include "WindowData.h"
#include <iostream>
#include <Windows.h>
#include <random>
#include <deque>
#include <math.h>
#define M_PI           3.14159265358979323846
using namespace sf;
int RandomNum(int min, int max);

class CSnake;
class CFood : public RectangleShape
{
public:
	CFood();
	void Spawn(CSnake& snake);
	void Draw(RenderWindow * window);
private:
	Vector2f m_vecPosition;
};

class CSnake
{
public:
	void Initialize();
	void Draw(RenderWindow* window);
	void Think();
	void Grow();
	void Eat(CFood &food);
	bool Die();
	void SetFace(Texture * pFace, RenderWindow * window);
	void GameOver(RenderWindow * window, CFood & food);
	unsigned int m_iScore = 0;
	Vector2f m_vecVelocity;
	std::deque<RectangleShape> Body = { };
	RectangleShape Head;
	bool m_bGameOver;
	Text m_ScoreText;
	Texture m_SnakeFace;
};
void InitializeSnakeGame(CSnake& snake, CFood& food, RenderWindow* window);
void RunSnake(CSnake& snake, CFood& food, RenderWindow* window);

