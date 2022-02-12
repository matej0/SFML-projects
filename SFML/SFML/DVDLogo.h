#pragma once
#include "SFML/Graphics.hpp"
#include <Windows.h>
#include <deque>
#include <sstream>
#include <array>
#include <random>
#include "WindowData.h"
using namespace sf;


//randomly pick between 2 given numbers
int RandomIntAlt(int first, int second)
{
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist(1, 2);
	return (dist(rng) == 1) ? first : second;
}

class CLogo : public Sprite
{
public:

	CLogo(Texture* pTexture, bool bShouldChangeColorOnImpact)
	{
		if (pTexture)
		{
			this->setTexture(*pTexture);
			this->setPosition(float(g_WindowData.RandomInt(10, 300)), float(g_WindowData.RandomInt(10, 300)));
			this->setScale(0.1f, 0.1f);
			this->m_vecVelocity = { float(RandomIntAlt(3, -3)), float(RandomIntAlt(3, -3)) };
			this->m_bShouldChangeColorOnImpact = bShouldChangeColorOnImpact;
		}
	}

	~CLogo()
	{
		 
	}

	void Draw(RenderWindow* window)
	{
		window->draw(*this);
	}

	void Think(RenderWindow* window)
	{
		//if you hit the edge, reverse velocity and change color
		if ((this->getPosition().x + this->getGlobalBounds().width) >= window->getSize().x || this->getPosition().x <= 0)
		{
			this->ChangeColor();
			this->m_vecVelocity.x = -this->m_vecVelocity.x;
		}

		if ((this->getPosition().y + this->getGlobalBounds().height) >= window->getSize().y || this->getPosition().y <= 0)
		{
			this->ChangeColor();
			this->m_vecVelocity.y = -this->m_vecVelocity.y;
		}

		this->move(m_vecVelocity);
	}

	void ChangeColor()
	{
		if (!this->m_bShouldChangeColorOnImpact)
			return;

		//lol
		const std::array<Color, 6> GetColors = { Color::Red, Color::Green, Color::Magenta, Color::Blue, Color::Yellow, Color::Cyan};
		Color clrNewColor = GetColors.at(g_WindowData.RandomInt(0, 5));

		while (this->getColor() == clrNewColor)
		{
			clrNewColor = GetColors.at(g_WindowData.RandomInt(0, 5));
		}

		this->setColor(clrNewColor);
	}

private:
	bool m_bShouldChangeColorOnImpact;
	Vector2f m_vecVelocity;
};