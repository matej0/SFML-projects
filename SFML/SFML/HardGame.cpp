#include "HardGame.h"
ITileMapProperties gTileMapProps;
std::vector<CObstacle> rgObstacles;
Vector2f vPos = Vector2f(48.f, 18.f);

int A_RAND(int min, int max)
{
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> xd(min, max);
	return xd(rng);
}

void CPlayer::Think()
{
	static const float flSpeed = 100.f;

	if (Keyboard::isKeyPressed(Keyboard::Left))
	{
		this->m_vecVelocity.x = -flSpeed;
		this->m_vecVelocity.y = 0.f;
	}

	if (Keyboard::isKeyPressed(Keyboard::Right))
	{
		this->m_vecVelocity.x = flSpeed;
		this->m_vecVelocity.y = 0.f;
	}

	if (Keyboard::isKeyPressed(Keyboard::Up))
	{
		this->m_vecVelocity.x = 0.f;
		this->m_vecVelocity.y = -flSpeed;
	}

	if (Keyboard::isKeyPressed(Keyboard::Down))
	{
		this->m_vecVelocity.x = 0.f;
		this->m_vecVelocity.y = flSpeed;
	}

	m_vecVelocity.x *= g_WindowData.deltaTime;
	m_vecVelocity.y *= g_WindowData.deltaTime;

	this->move(m_vecVelocity);
}

bool CPlayer::HitObstacle(CObstacle& obstacle)
{
	//credits to some guy from stackoverflow.
	float closestX = std::clamp(obstacle.getPosition().x, this->getPosition().x, this->getPosition().x + this->getGlobalBounds().width);
	float closestY = std::clamp(obstacle.getPosition().y, this->getPosition().y, this->getPosition().y + this->getGlobalBounds().height);

	// Calculate the distance between the circle's center and this closest point
	float distanceX = obstacle.getPosition().x - closestX;
	float distanceY = obstacle.getPosition().y - closestY;

	// If the distance is less than the circle's radius, an intersection occurs
	float distanceSquared = (distanceX * distanceX) + (distanceY * distanceY);
	return distanceSquared < std::pow(obstacle.getRadius(), 2);
}

void CPlayer::Draw(RenderWindow* pWindow)
{
	pWindow->draw(*this);
}

void CObstacle::StartMoving()
{
	const float flSpeed = A_RAND(3, 5);

	//this could prolly be written in a better way, too tired.
	if (this->m_bShouldStartWithInverseVelocity)
	{
		this->m_vecVelocity.x = 0.f;
		this->m_vecVelocity.y = -flSpeed;
	}
	else
	{
		this->m_vecVelocity.x = 0.f;
		this->m_vecVelocity.y = flSpeed;
	}
}

void CObstacle::Think()
{
	if (this->getPosition().y <= 0 || this->getPosition().y + this->getRadius() >= gTileMapProps.height)
		this->m_vecVelocity.y *= -1;

	this->move(m_vecVelocity);
}

void CObstacle::Draw(RenderWindow* pWindow)
{
	pWindow->draw(*this);
}

void ResetGame(CPlayer& player)
{
	rgObstacles.clear(); // dump out all the obstacles
	vPos = Vector2f(48.f, 18.f); //reset the positions of new obstacles

	//repopulate vector
	for (int i = 0; i < 5; i++)
	{
		CObstacle obstacle(vPos);
		rgObstacles.push_back(obstacle);
		vPos.x += 32;

		if (i % 2 == 0)
			vPos.y = gTileMapProps.height - 18.f;
		else
			vPos.y = 18.f;

		if (rgObstacles.at(i).getPosition().y == gTileMapProps.height - 18.f)
			rgObstacles.at(i).m_bShouldStartWithInverseVelocity = true;
		else
			rgObstacles.at(i).m_bShouldStartWithInverseVelocity = false;
	}

	player.setPosition({ 10.f, 50.f }); //reset player position

	for (int i = 0; i < rgObstacles.size(); i++)
	{
		rgObstacles[i].StartMoving();
	}
}
