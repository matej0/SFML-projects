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
#include "Debug.h"
using namespace sf;

#define WING_UP 0
#define WING_DOWN 1
#define WING_MIDDLE 2
#define ROTATION_MULTIPLIER 15.f

class CBird : public Sprite
{
public:
	CBird(std::deque<Texture>& BirdTextures);
	void Animate();
	void Draw(RenderWindow * pRenderWindow);
	bool IsOutOfBounds();
	void AirMove();
	void Jump();
	void Fall();
	void Think();
public:
	Vector2f GetVelocity()
	{
		return m_vecVelocity;
	};
	void SetVelocity(float flX, float flY)
	{
		m_vecVelocity.x = flX;
		m_vecVelocity.y = flY;
	};
	void SetVelocity(Vector2f vecNewVelocity)
	{
		m_vecVelocity = vecNewVelocity;
	};
	const float GetGravity()
	{
		return m_flGravity;
	};
	const float GetImpulse()
	{
		return m_flImpulseAmount;
	};
	const float GetMaxSpeed()
	{
		return m_flMaxSpeed;
	};
	bool GetCanMove()
	{
		return m_bCanMove;
	};
	void SetCanMove(bool value)
	{
		m_bCanMove = value;
	};
	bool GetHitWorld()
	{
		return m_bHitWorld;
	};
	void SetHitWorld(bool value)
	{
		m_bHitWorld = value;
	};
private:
	bool m_bHitWorld = false;
	bool m_bCanMove = true;
	const float m_flMaxSpeed = -3.f;
	const float m_flImpulseAmount = 100.f;
	const float m_flGravity = 10.f;
	Vector2f m_vecVelocity;
	std::deque<Texture> m_BirdTextures;
};