#include "Clock.h"
using namespace sf;

void CClock::Initialize(RenderWindow* window)
{
	//garbage casting not really needed but compiler complains with big error otherwise.
	Vector2f vecScreenCenter = { static_cast<float>(window->getSize().x / 2), static_cast<float>(window->getSize().y / 2) };

	Color transparent(0, 0, 0, 0); //the circle needs to be transparent but the outline will be white
	this->m_Rim.setRadius(200.f);
	this->m_Rim.setOrigin(this->m_Rim.getRadius(), this->m_Rim.getRadius());
	this->m_Rim.setPosition(vecScreenCenter); 
	this->m_Rim.setFillColor(transparent);
	this->m_Rim.setOutlineThickness(1.f);
	this->m_Rim.setOutlineColor(Color::White);

	//we need this rim otherwise it looks hella ugly
	this->m_DialCover.setRadius(6.f);
	this->m_DialCover.setOrigin(this->m_DialCover.getRadius(), this->m_DialCover.getRadius());
	this->m_DialCover.setPosition(vecScreenCenter);
	this->m_DialCover.setFillColor(Color::White);

	for (int i = 1; i < 4; i++)
	{
		CDial dial;
		dial.setSize({ i * 60.f, 2.f });
		dial.setPosition(vecScreenCenter);
		dial.setFillColor(Color::White);
		dial.setType(i - 1);

		this->m_Dials.push_back(dial);
	}
}

void CClock::Draw(RenderWindow* window)
{
	window->draw(this->m_Rim);
	window->draw(this->m_DialCover);

	if (!this->m_Dials.empty())
	{
		for (unsigned int i = 0; i < this->m_Dials.size(); i++)
		{
			window->draw(this->m_Dials.at(i));
		}
	}
}

void CClock::Think()
{
	if (this->m_Dials.empty()) //we dont got no dials man.
		return; 

	//i was wondering "why no work??" but then i realize i forgot to call this axaxaaxa.
	this->GetTime();

	//need to be defined outside of loop, also 90 needs to be subtracted because angles on the clock dont correspond to angles in real life ( 0 on the clock is where 90 would be in real life )
	float angSecond = static_cast<float>((this->m_Time.m_iSecond / 60.f) * 360.f - 90.f);
	float angMinute = static_cast<float>((this->m_Time.m_iMinute / 60.f) * 360.f - 90.f);
	float angHour = static_cast<float>((this->m_Time.m_iHour / 12.f) * 360.f - 90.f);

	for (unsigned int i = 0; i < this->m_Dials.size(); i++)
	{
		//should never happen but never say never.
		if (this->m_Dials.at(i).getType() == -1)
			continue;

		switch (this->m_Dials.at(i).getType())
		{
		case 0:
			this->m_Dials.at(i).setRotation(angHour); break;
		case 1:
			this->m_Dials.at(i).setRotation(angMinute); break;
		case 2:
			this->m_Dials.at(i).setRotation(angSecond); break;
		}
	}
}

void CClock::GetTime()
{
	std::time_t t = std::time(nullptr);
	std::tm* time = std::localtime(&t);

	this->m_Time.m_iHour = time->tm_hour % 12;
	this->m_Time.m_iMinute = time->tm_min;
	this->m_Time.m_iSecond = time->tm_sec;
}