#pragma once
#include <SFML/Graphics.hpp>
#include "WindowData.h"
#include <iostream>
#include <Windows.h>
#include <random>
#include <deque>
#include <math.h>
#include <memory>
#include <algorithm>
using namespace sf;
//inspired by that templeOS game
void DrawCrosshair(RenderWindow* pRenderWindow);

struct ITankGameInfo
{
	UINT m_nKills;
};
extern ITankGameInfo gTankGameInfo;

class CBullet : public RectangleShape
{
public:
	Vector2f m_vecVelocity;
	bool m_bMarkedForDeletion = false;
};

class CTank : public CircleShape
{
private:
	RectangleShape m_BottomLine; // bottom line of the triangle will be thicker, visual indicator of the top

public:
	CTank(float flRadius, Color clrColor)
	{
		this->setRadius(flRadius);
		this->setPointCount(3); // 3 points for triangle. 
		this->setFillColor(Color(0, 0, 0, 0));
		this->setOutlineThickness(1.f);
		this->setOutlineColor(clrColor);

		Vector2f vecCenter;
		vecCenter.x = (this->getPoint(0).x + this->getPoint(1).x + this->getPoint(2).x) / 3;
		vecCenter.y = (this->getPoint(0).y + this->getPoint(1).y + this->getPoint(2).y) / 3;
		this->setOrigin(vecCenter);
		this->setPosition(Vector2f(g_WindowData.height / 2.f, g_WindowData.width / 2.f));
	}
	void Draw(RenderWindow* pWindow);
	void DrawDebugOverlay(RenderWindow * pRenderWindow);
	void MouseMove();
	void Move();
	void DrawBullet(RenderWindow * pWindow);
	void SpawnBullet();
	void InvalidateBullet(CBullet * pBullet);
	void SimulateBullet();
	void DestroyInvalidBullets();
	std::deque<CBullet*> m_Bullets;
	float m_flLastShotTime;
};

class CTarget : public RectangleShape
{
public:
	Vector2f GetSpawnPosition();
	void Spawn();
	bool HasBeenShot(CBullet * pBullet);
	void Die(CBullet * pBullet);
	void Think(CTank & tank);
	void Draw(RenderWindow * pRenderWindow);
	bool m_bWasHit = false;
	bool m_QueueNextTarget = false;
	std::deque<CTarget*> m_Targets; // invisible rectangle that encapsulates our triangle and defines the area where targets cant spawn.
};

