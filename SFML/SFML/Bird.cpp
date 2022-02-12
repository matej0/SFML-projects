#include "Bird.h"


CBird::CBird(std::deque<Texture>& BirdTextures)
{
	
	this->m_BirdTextures = BirdTextures;
	this->setTexture(m_BirdTextures[WING_MIDDLE]);
	this->setOrigin(this->getTexture()->getSize().x / 2.f, this->getTexture()->getSize().y / 2.f);
	this->setPosition(200.f, g_WindowData.center.y);
}

void CBird::Animate()
{
	//GHETTO AS HELL!
	static Clock AnimationClock;
	static int iAnimationIndex = 0;

	if (AnimationClock.getElapsedTime().asSeconds() >= 0.2f)
	{
		iAnimationIndex = !iAnimationIndex;
		AnimationClock.restart();
	}

	this->setTexture(m_BirdTextures[iAnimationIndex]);
	DEBUG::DrawString(Vector2f(10.f, 22.f), 12, Text::Regular, g_WindowData.font, Color::White, "anim idx: " + std::to_string(iAnimationIndex));
}

void CBird::Draw(RenderWindow* pRenderWindow)
{
	this->Animate();
	pRenderWindow->draw(*this);
}

bool CBird::IsOutOfBounds()
{
	bool bWentOutOfBounds = (this->getPosition().y >= g_WindowData.height || this->getPosition().y <= 0);
	this->SetCanMove(!bWentOutOfBounds); // god im so fucking smart.
	return bWentOutOfBounds;
}


void CBird::Fall()
{
	Vector2f vecNewVelocity = this->GetVelocity();
	vecNewVelocity.y += this->GetGravity()* g_WindowData.deltaTime;

	this->SetVelocity(vecNewVelocity);
	this->move(this->GetVelocity());
}

void CBird::Jump()
{	
	Vector2f vecNewVelocity = this->GetVelocity();
	vecNewVelocity.y -= this->GetImpulse() * g_WindowData.deltaTime; //send an impulse in the opposite direction

	if (vecNewVelocity.y <= this->GetMaxSpeed())
		vecNewVelocity.y = this->GetMaxSpeed();

	this->SetVelocity(vecNewVelocity);
	this->move(this->GetVelocity());
}


void DrawBirdBBOX(CBird& pBird)
{
	static sf::RenderWindow* pRenderWindow = reinterpret_cast<sf::RenderWindow*>(g_WindowData.pRenderWindowPointer);
	RectangleShape BBOX;
	BBOX.setFillColor(Color(0, 0, 0, 0));
	BBOX.setOutlineColor(Color::Red);
	BBOX.setOutlineThickness(1.f);
	BBOX.setOrigin(pBird.getOrigin());
	BBOX.setPosition(pBird.getPosition());
	BBOX.setSize(Vector2f(pBird.getGlobalBounds().width, pBird.getGlobalBounds().height));
	pRenderWindow->draw(BBOX);
}

void CBird::AirMove()
{
	if (Mouse::isButtonPressed(Mouse::Left))
		this->Jump();

	this->Fall();
}

void CBird::Think()
{
	DEBUG::DrawString(Vector2f(10.f, 10.f), 12, Text::Regular, g_WindowData.font, Color::White, "velocity: " + std::to_string(this->GetVelocity().y));

	this->setRotation(this->GetVelocity().y * ROTATION_MULTIPLIER);
	this->SetHitWorld(this->IsOutOfBounds());
	this->AirMove();
}