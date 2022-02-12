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
constexpr float g_flProjectileSpeed = 900.f;
constexpr int m_iDamage = 25;
constexpr int m_iMaxHealth = 100;
//inspired by that templeOS game
void DrawCrosshair(RenderWindow* pRenderWindow);
struct ITankGameInfo
{
	UINT m_nKills;
	FloatRect m_TankBoundingBox;
	Vector2f  m_vecTankOrigin;
	bool m_bQueueNextTarget;
};
extern ITankGameInfo gGameInfo;

class CBullet : public RectangleShape
{
public:
	bool GetIsMarkedForDeletion() { return m_bIsMarkedForDeletion; };
	void SetIsMarkedForDeletion(bool bValue) { m_bIsMarkedForDeletion = bValue; };
	Vector2f GetVelocity() { return m_vecVelocity; };
	void SetVelocity(Vector2f vecValue) { m_vecVelocity = vecValue; };
	void SetVelocity(float flX, float flY) { m_vecVelocity.x = flX; m_vecVelocity.y = flY; };

private:
	Vector2f m_vecVelocity;
	bool m_bIsMarkedForDeletion = false;
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
		this->setPosition(Vector2f(g_WindowData.center.x, g_WindowData.center.y));

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
	void MouseMove();
	void MoveTank();
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
	bool HasBeenShot(CBullet * pBullet);
	void OnTargetHurt();
	void Die(CBullet * pBullet, CTank & pTank);
	void MoveToTank(CTank & Tank);
	bool IsTouchingTank(CTank & Tank);
	void Think(CTank & tank);
	void DrawHealth(RenderWindow * pRenderWindow);
	void Draw(RenderWindow * pRenderWindow);

	bool GetIsMarkedForDeletion() { return m_bIsMarkedForDeletion; };
	void SetIsMarkedForDeletion(bool bValue) { m_bIsMarkedForDeletion = bValue; };
	Color GetDefaultColor() { return m_DefaultColor; };
	int GetHealth() { return m_iHealth; };
	void SetHealth(int iValue) { m_iHealth = iValue; };
	Color m_DefaultColor;
private:
	bool m_bIsMarkedForDeletion = false;
	bool m_bWasHit = false;
	int m_iHealth = 100;

};

class CGame
{
public:
	void CreateNewTarget();
	void DeleteTarget(CTarget * pTargetToDelete, int index);
	void SpawnTargets();
	void CreateAndManageTargets(CTank * pTank, RenderWindow * pRenderWindow);
	std::deque<CTarget*> m_Targets;
	bool m_QueueNextTarget = false;
};

void PlayGame(RenderWindow* pRenderWindow, CTank* Tank, CGame* pGame);
