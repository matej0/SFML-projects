#include "Tank.h"
ITankGameInfo gTankGameInfo;
constexpr float g_flProjectileSpeed = 700.f;

void DrawCrosshair(RenderWindow* pWindow)
{
	static Sprite crosshairSprite;
	static Texture crosshairTexture;

	if (!crosshairTexture.loadFromFile("cross.png"))
		return;

	Mouse myMouse;
	pWindow->setMouseCursorVisible(false);
	Vector2i vecMousePosition = myMouse.getPosition(*pWindow);


	crosshairSprite.setTexture(crosshairTexture);

	static Vector2f vecSpriteCenter = Vector2f(crosshairSprite.getGlobalBounds().width / 2.f, crosshairSprite.getGlobalBounds().height / 2.f);
	crosshairSprite.setOrigin(vecSpriteCenter);

	crosshairSprite.setScale(0.05f, 0.05f);
	crosshairSprite.setPosition(Vector2f(vecMousePosition));
	crosshairSprite.setColor(Color::Green);

	pWindow->draw(crosshairSprite);
}

void CTank::Draw(RenderWindow* pRenderWindow)
{
	pRenderWindow->draw(*this);
}

void CTank::DrawDebugOverlay(RenderWindow* pRenderWindow)
{
	Text lastFireShot;
	lastFireShot.setString(std::to_string(this->m_flLastShotTime));
	lastFireShot.setPosition(10, 10);
	lastFireShot.setFont(g_WindowData.font);
	lastFireShot.setCharacterSize(12);
	lastFireShot.setFillColor(Color::White);
	lastFireShot.setStyle(Text::Regular);
	pRenderWindow->draw(lastFireShot);
}

void CTank::MouseMove()
{
	RenderWindow* pRenderWindow = reinterpret_cast<RenderWindow*>(g_WindowData.pRenderWindowPointer);
	Mouse myMouse;
	Vector2i vecMousePosition = myMouse.getPosition(*pRenderWindow);
	Vector2f vecScreenCenter = Vector2f(g_WindowData.center[0], g_WindowData.center[1]);
	
	Vector2f vecDelta = Vector2f();
	vecDelta.x = vecMousePosition.x - vecScreenCenter.x;
	vecDelta.y = vecMousePosition.y - vecScreenCenter.y;
	float angle = RAD2DEG(std::atan2f(vecDelta.x, -vecDelta.y));
	this->setRotation(angle);
}

void CTank::Move()
{
	if (Keyboard::isKeyPressed(Keyboard::Left))
		this->rotate(-10.f);

	if (Keyboard::isKeyPressed(Keyboard::Right))
		this->rotate(10.f);
}

bool IsOutsideWindow(Vector2f vecPos)
{
	return (vecPos.x > g_WindowData.width || vecPos.x < 0 || vecPos.y > g_WindowData.height || vecPos.y < 0);
}

CBullet* CreateBullet(Vector2f vecPosition)
{
	CBullet* pBullet = new CBullet();
	pBullet->setSize({10.f, 2.f});
	pBullet->setOrigin(pBullet->getGlobalBounds().width / 2.f, pBullet->getGlobalBounds().height / 2.f);
	pBullet->setPosition({ g_WindowData.center[0],  g_WindowData.center[1] });
	pBullet->setFillColor(Color::Yellow);
	return pBullet;
}

void CTank::DrawBullet(RenderWindow* pWindow)
{
	for (UINT i = 0; i < this->m_Bullets.size(); i++)
	{
		CBullet* pBullet = m_Bullets[i];

		if (!pBullet)
			continue;

		if (IsOutsideWindow(pBullet->getPosition()))
			continue;

		pWindow->draw(*pBullet);
	}

	this->DestroyInvalidBullets();
	//system("cls");
	//std::cout << "Size: " << this->m_Bullets.size() << std::endl;
}

void CTank::SpawnBullet()
{
	static Clock ShootClock;
	this->m_flLastShotTime = ShootClock.getElapsedTime().asSeconds();

	if (!this->m_Bullets.empty() && this->m_flLastShotTime < 0.1f)
		return;
	
	if (Mouse::isButtonPressed(Mouse::Left))
	{
		CBullet* pBullet = CreateBullet({ g_WindowData.center[0],  g_WindowData.center[1] });

		pBullet->setRotation(this->getRotation() - 90); //make it shoot out of the top of the triangle
		pBullet->m_vecVelocity.x = std::cos(DEG2RAD(pBullet->getRotation())) * g_flProjectileSpeed * g_WindowData.deltaTime; //standard cosine and sine functions take radians and not degrees. oops.
		pBullet->m_vecVelocity.y = std::sin(DEG2RAD(pBullet->getRotation())) * g_flProjectileSpeed * g_WindowData.deltaTime;

		this->m_Bullets.push_back(pBullet);
		ShootClock.restart();
	}
}

void CTank::InvalidateBullet(CBullet* pBullet)
{
	pBullet->m_bMarkedForDeletion = true;
}

void CTank::SimulateBullet()
{
	for (UINT i = 0; i < this->m_Bullets.size(); i++)
	{
		CBullet* pBullet = this->m_Bullets[i];

		if (!pBullet)
			continue;

		pBullet->move(pBullet->m_vecVelocity);

		if (IsOutsideWindow(pBullet->getPosition())) //destroy bullet when out of screen
		{
			this->InvalidateBullet(pBullet);
		}
	}
}

void CTank::DestroyInvalidBullets()
{
	for (UINT i = 0; i < this->m_Bullets.size(); i++)
	{
		if (this->m_Bullets[i]->m_bMarkedForDeletion)
		{
			delete m_Bullets[i];
			this->m_Bullets.erase(this->m_Bullets.begin() + i); 
		}
	}
}

Vector2f CTarget::GetSpawnPosition()
{
	Vector2f vecSpawnPos = Vector2f();

	vecSpawnPos.x = g_WindowData.Random(40, 740);
	vecSpawnPos.y = g_WindowData.Random(40, 740);

	return vecSpawnPos;
}

void CTarget::Spawn()
{
	this->m_QueueNextTarget = false;

	CTarget* pNewTarget = new CTarget();

	pNewTarget->setSize(Vector2f(25.f, 25.f));
	pNewTarget->setFillColor(Color(0, 0, 0, 0));
	pNewTarget->setOutlineColor(Color::White);
	pNewTarget->setOutlineThickness(2.f);

	pNewTarget->setOrigin(pNewTarget->getGlobalBounds().width / 2.f, pNewTarget->getGlobalBounds().height / 2.f);
	pNewTarget->setPosition(this->GetSpawnPosition());

	this->m_Targets.push_back(pNewTarget);
}

bool CTarget::HasBeenShot(CBullet* pBullet)
{
	return (this->getGlobalBounds().contains(pBullet->getPosition()));
}

void CTarget::Die(CBullet* pBullet)
{
	for (UINT i = 0; i < this->m_Targets.size(); i++)
	{
		CTarget* pTarget = this->m_Targets[i];

		if (pTarget->HasBeenShot(pBullet))
		{
			pBullet->m_bMarkedForDeletion = true; // delete bullet after it hits a target.
			delete pTarget;
			this->m_Targets.erase(this->m_Targets.begin() + i);
			this->Spawn();
			gTankGameInfo.m_nKills++;
		}
	}
}

void CTarget::Think(CTank &tank)
{
	for (auto pBullets : tank.m_Bullets)
		this->Die(pBullets);
}

void CTarget::Draw(RenderWindow* pRenderWindow)
{
	for (static size_t i = 0; i < 3; i++)
		Spawn();

	for (auto pTarget : m_Targets)
	{
		pRenderWindow->draw(*pTarget);
	}
}