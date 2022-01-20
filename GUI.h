#pragma once
#include "SFML/Graphics.hpp"
#include "WindowData.h"
#include <iostream>
#include <Windows.h>
#include <random>
#include <deque>
#include <math.h>
using namespace sf;
const Color clrCheckBoxOn = Color(0, 255, 0, 255);
const Color clrCheckBoxOff = Color(51, 51, 51, 255);
#define CHECKBOX_SIZE 20.f
#define CHECKBOX_POSITION_X 20.f
#define CHECKBOX_POSITION_Y 20.f

#define COMBOBOX_SIZE_X 60.f
#define COMBOBOX_SIZE_Y 20.f

struct IControlInfo
{
	float m_CheckBoxOffset = 0.0f;
};
extern IControlInfo g_ControlInfo;

class CCheckBox : public RectangleShape
{
public:

	bool IsBeingHoveredOver(RenderWindow* pWindow)
	{
		return this->getGlobalBounds().contains(Vector2f(Mouse::getPosition(*pWindow)));
	}

	void Register(std::string strTitle)
	{
		Vector2f vPos = Vector2f(CHECKBOX_POSITION_X, CHECKBOX_POSITION_Y + g_ControlInfo.m_CheckBoxOffset);

		this->setPosition(vPos);
		this->setSize(Vector2f(CHECKBOX_SIZE, CHECKBOX_SIZE));
		this->m_strTitle = strTitle;

		title.setFont(g_WindowData.font);
		title.setCharacterSize(14);
		title.setStyle(Text::Regular);

		title.setPosition(vPos.x + this->getGlobalBounds().width + 10.f, vPos.y);

		title.setString(this->m_strTitle);
		title.setFillColor(Color::White);

		g_ControlInfo.m_CheckBoxOffset = g_ControlInfo.m_CheckBoxOffset + (CHECKBOX_POSITION_Y * 2.f);
	}

	void Draw(RenderWindow* pWindow, bool* bpToggle)
	{
		static bool bIsClicked = false;

		if (this->IsBeingHoveredOver(pWindow) && !Mouse::isButtonPressed(Mouse::Left))
		{
			if (bIsClicked)
				*bpToggle = !*bpToggle;


			bIsClicked = false;
		}

		if (this->IsBeingHoveredOver(pWindow) && Mouse::isButtonPressed(Mouse::Left) && !bIsClicked)
			bIsClicked = true;
	
		this->setFillColor(*bpToggle ? clrCheckBoxOn : clrCheckBoxOff);

		pWindow->draw(this->title);
		pWindow->draw(*this);
	}

private:
	std::string m_strTitle;
	Text title;
};

class CComboBox : public RectangleShape
{
public:
	bool IsBeingHoveredOver(RenderWindow* pWindow)
	{
		return this->getGlobalBounds().contains(Vector2f(Mouse::getPosition(*pWindow)));
	}

	void Register(std::string strTitle)
	{

	}

	void Draw(RenderWindow* pWindow, bool* bpToggle)
	{

	}
private:

};

class CGui
{
public:
	void RegisterAll();
	void Draw(RenderWindow* pWindow);
private:
	CCheckBox meme1;
};