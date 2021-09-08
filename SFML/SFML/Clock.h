#pragma once
#include "SFML/Graphics.hpp"
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <time.h>
using namespace sf;

struct ClockTime
{
	unsigned int m_iHour, m_iMinute, m_iSecond;
};

//rectangle -> dials of the clock ( sfml doesnt have an individual line type thing so you need to use rectangles and call "Shape::rotate()" )
class CDial : public RectangleShape
{
public:

	void setType(int type)
	{
		m_Type = type;
	}

	unsigned int getType()
	{
		return m_Type;
	}

private:
	unsigned int m_Type = -1; //type of dial = hour, minute or second ( 0, 1, 2 )
};

class CClock
{
public:
	void Initialize(RenderWindow* window);
	void Draw(RenderWindow* window);
	void Think();
	void GetTime();
private:
	CircleShape m_Rim, m_DialCover;
	std::vector<CDial> m_Dials; //3 dials
	ClockTime m_Time;
};