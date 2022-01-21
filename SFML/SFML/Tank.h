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

#define ZOMBIE_ATLAS_WIDTH 885
#define ZOMBIE_ATLAS_HEIGHT 776
#define ZOMBIE_WIDTH 74
#define ZOMBIE_HEIGHT 97

//inspired by that templeOS game
void DrawCrosshair(RenderWindow* pRenderWindow);
struct ITankGameInfo
{
	UINT m_nKills;
	FloatRect m_TankBoundingBox;
	Vector2f  m_vecTankOrigin;
};
extern ITankGameInfo gTankGameInfo;

class CBullet : public RectangleShape
{
public:
	Vector2f m_vecVelocity;
	bool m_bMarkedForDeletion = false;
};

class CTank : public Sprite
{
private:
	RectangleShape m_BottomLine; // bottom line of the triangle will be thicker, visual indicator of the top

public:
	CTank(Texture* pTexture)
	{
		this->setTexture(*pTexture);

		Vector2f vecCenter = Vector2f(this->getGlobalBounds().width / 2.f, this->getGlobalBounds().height / 2.f);//Vector2f(g_WindowData.center[0], g_WindowData.center[1]);
		this->setOrigin(vecCenter);
		this->setPosition(Vector2f(g_WindowData.center[0], g_WindowData.center[1]));

		this->setScale({ 0.5f, 0.5f });

	}
	//this->setRadius(flRadius);
	//this->setPointCount(3); // 3 points for triangle. 
	//this->setFillColor(Color(0, 0, 0, 0));
	////this->setOutlineThickness(1.f);
	//this->setOutlineColor(clrColor);
	//vecCenter.x = (this->getPoint(0).x + this->getPoint(1).x + this->getPoint(2).x) / 3;
	//vecCenter.y = (this->getPoint(0).y + this->getPoint(1).y + this->getPoint(2).y) / 3;
	
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
	bool m_bGameOver = false;

	Vector2f m_vecVelocity;
	Vector2f m_vecAcceleration = Vector2f(30.f, 30.f);
};


class CTarget : public Sprite
{
public:
	bool IsInvalidSpawnPosition(Vector2f vecSpawnPos);
	Vector2f GetSpawnPosition();
	void Spawn();
	bool HasBeenShot(CBullet * pBullet);
	void OnTargetHurt();
	void Die(CBullet * pBullet, CTank & pTank);
	void MoveToTank(CTank & Tank);
	bool IsTouchingTank(CTank & Tank);
	void Think(CTank & tank);
	void DrawHealth(RenderWindow * pRenderWindow);
	void Draw(RenderWindow * pRenderWindow);
	bool m_bWasHit = false;
	bool m_QueueNextTarget = false;
	int m_iHealth = 100;
	std::deque<CTarget*> m_Targets; // invisible rectangle that encapsulates our triangle and defines the area where targets cant spawn.
	Color m_DefaultColor;
};

void PlayGame(RenderWindow* pRenderWindow, CTank& Tank, CTarget& Target);
