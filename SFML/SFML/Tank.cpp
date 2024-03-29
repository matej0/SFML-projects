#include "Tank.h"
ITankGameInfo gGameInfo;



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

void CTank::MouseMove()
{
	RenderWindow* pRenderWindow = reinterpret_cast<RenderWindow*>(g_WindowData.pRenderWindowPointer);
	Mouse myMouse;
	Vector2i vecMousePosition = myMouse.getPosition(*pRenderWindow);
	Vector2f vecScreenCenter = this->getPosition();
	
	Vector2f vecDelta = Vector2f();
	vecDelta.x = vecMousePosition.x - vecScreenCenter.x;
	vecDelta.y = vecMousePosition.y - vecScreenCenter.y;
	float angle = RAD2DEG(std::atan2f(vecDelta.x, -vecDelta.y));
	this->setRotation(angle);
}

void CTank::MoveTank()
{
	this->m_vecVelocity.x = std::cos(DEG2RAD(this->getRotation() - 90)) * 100.f * g_WindowData.deltaTime; //standard cosine and sine functions take radians and not degrees. oops.
	this->m_vecVelocity.y = std::sin(DEG2RAD(this->getRotation() - 90)) * 100.f * g_WindowData.deltaTime;

	if (Keyboard::isKeyPressed(Keyboard::W) || Mouse::isButtonPressed(Mouse::XButton2))
		this->move(this->m_vecVelocity);
	else if (Keyboard::isKeyPressed(Keyboard::S) || Mouse::isButtonPressed(Mouse::XButton1))
		this->move(-this->m_vecVelocity);

	gGameInfo.m_vecTankOrigin = this->getPosition();
	gGameInfo.m_TankBoundingBox = this->getLocalBounds();
}

bool IsOutsideWindow(Vector2f vecPos)
{
	return (vecPos.x > g_WindowData.width || vecPos.x < 0 || vecPos.y > g_WindowData.height || vecPos.y < 0);
}

CBullet* CreateBullet(Vector2f vecPosition)
{
	CBullet* pBullet = new CBullet();
	pBullet->setSize({10.f, 3.f});
	pBullet->setOrigin(pBullet->getGlobalBounds().width / 2.f, pBullet->getGlobalBounds().height / 2.f);
	pBullet->setPosition(vecPosition);
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
}

void CTank::SpawnBullet()
{
	static Clock ShootClock;
	this->m_flLastShotTime = ShootClock.getElapsedTime().asSeconds();

	if (!this->m_Bullets.empty() && this->m_flLastShotTime < 0.1f)
		return;
	
	if (Mouse::isButtonPressed(Mouse::Left))
	{
		CBullet* pBullet = CreateBullet(this->getPosition());

		pBullet->setRotation(this->getRotation() - 90); //make it shoot out of the top of the triangle
		pBullet->SetVelocity(std::cos(DEG2RAD(pBullet->getRotation())) * g_flProjectileSpeed * g_WindowData.deltaTime, std::sin(DEG2RAD(pBullet->getRotation())) * g_flProjectileSpeed * g_WindowData.deltaTime);
		this->m_Bullets.push_back(pBullet);
		ShootClock.restart();
	}
}

void CTank::InvalidateBullet(CBullet* pBullet)
{
	pBullet->SetIsMarkedForDeletion(true);
}

void CTank::SimulateBullet()
{
	for (auto pBullet : this->m_Bullets)
	{
		if (!pBullet)
			continue;

		pBullet->move(pBullet->GetVelocity());

		if (IsOutsideWindow(pBullet->getPosition())) //destroy bullet when out of screen
			this->InvalidateBullet(pBullet);
	}
}

void CTank::DestroyInvalidBullets()
{
	for (UINT i = 0; i < this->m_Bullets.size(); i++)
	{
		if (this->m_Bullets[i]->GetIsMarkedForDeletion())
		{
			delete m_Bullets[i];
			this->m_Bullets.erase(this->m_Bullets.begin() + i); 
		}
	}
}

bool CTarget::IsInvalidSpawnPosition(Vector2f vecSpawnPos) //TODO: FIX THIS RETARDED SHIT!
{
	/*if (gGameInfo.m_TankBoundingBox.contains(vecSpawnPos))
		return true;

	//you cant spawn on top of an another target if there are no other targets.
	if (this->m_Targets.empty())
		return false; 

	for (auto pTarget : m_Targets)
	{
		if (this->getGlobalBounds().contains(pTarget->getPosition()) || this->getGlobalBounds().intersects(pTarget->getGlobalBounds()))
			return true;
	}
	
	return false;*/
}

Vector2f CTarget::GetSpawnPosition() //THIS TOO!
{
	Vector2f vecSpawnPos = Vector2f();

	vecSpawnPos.x = g_WindowData.RandomInt(40, g_WindowData.width - 40);
	vecSpawnPos.y = g_WindowData.RandomInt(40, g_WindowData.height - 40);

	//while (gGameInfo.m_TankBoundingBox.contains(vecSpawnPos))
	//{
		//if spawn pos is invalid, keep generating new positions until one is valid. idk if this is the best way to do this but i dont think its very resource intensive so who cares.
	//	vecSpawnPos.x = g_WindowData.Random(40, 1140);
	//	vecSpawnPos.y = g_WindowData.Random(40, 740);
	//}

	return vecSpawnPos;
}

CTarget* CreateTarget()
{
	static Texture pTargetTexture;
	pTargetTexture.loadFromFile("Daco.png", IntRect((ZOMBIE_ATLAS_WIDTH - (ZOMBIE_WIDTH * 2)), (ZOMBIE_ATLAS_HEIGHT - (ZOMBIE_HEIGHT * 2)), ZOMBIE_WIDTH, ZOMBIE_HEIGHT));
	CTarget* pNewTarget = new CTarget();

	pNewTarget->setTexture(pTargetTexture);
	pNewTarget->setOrigin(pNewTarget->getGlobalBounds().width / 2.f, pNewTarget->getGlobalBounds().height / 2.f);
	pNewTarget->setPosition(pNewTarget->GetSpawnPosition());
	pNewTarget->m_DefaultColor = pNewTarget->getColor();

	gGameInfo.m_bQueueNextTarget = false; //new target was spawned
	return pNewTarget;
}

bool CTarget::HasBeenShot(CBullet* pBullet)
{
	return (this->getGlobalBounds().contains(pBullet->getPosition()) || pBullet->getGlobalBounds().intersects(this->getGlobalBounds()));
}

void CTarget::OnTargetHurt()
{
	//other stuff was supposed to happen here.
	this->setColor(Color::Red);
}

void CTarget::Die(CBullet* pBullet, CTank& pTank)
{
	if (this->m_DefaultColor != this->getColor())
	{
		static Clock clock;

		if (clock.getElapsedTime().asMilliseconds() > 200.f)
		{
			this->setColor(this->m_DefaultColor);
			clock.restart();
		}
	}

	if (this->HasBeenShot(pBullet))
	{
		pBullet->SetIsMarkedForDeletion(true); // delete bullet after it hits a target.
		this->SetHealth(this->GetHealth() - m_iDamage);
		this->OnTargetHurt();

		if (this->GetHealth() <= 0)
		{
			this->SetIsMarkedForDeletion(true);
			gGameInfo.m_bQueueNextTarget = true;
			gGameInfo.m_nKills++;
		}
	}
}

//credits: the internet
//std::lerp has been added in c++20 and im using c++17 gg
float lerp(float a, float b, float t)
{
	return a + t * (b - a);
}

void CTarget::MoveToTank(CTank& Tank)
{
	static float flFactor = 0.0f; 
	flFactor = (g_WindowData.deltaTime * 0.1f);
	this->setPosition({ lerp(this->getPosition().x, Tank.getPosition().x, flFactor), lerp(this->getPosition().y, Tank.getPosition().y, flFactor) });
}

bool CTarget::IsTouchingTank(CTank& Tank)
{
	return (this->getGlobalBounds().contains(Tank.getPosition()));
}

void CTarget::Think(CTank &Tank)
{
	this->MoveToTank(Tank);

	for (auto pBullets : Tank.m_Bullets)
		this->Die(pBullets, Tank);
}

Color GetHealthColor(int iHP, int iMaxHP)
{
	int iGreen = (255 * iHP) / iMaxHP;
	int iRed = 255 - iGreen;

	return Color(iRed, iGreen, 0);
}

int GetHealthWidth(int iHP, int iMaxHP)
{
	return (int)std::round(50 * iHP / iMaxHP);
}

RectangleShape CreateHealthBar(Vector2f vecPos, Color clrHealth, int iWidth)
{
	RectangleShape HealthBar;

	static Vector2f vecCenter = Vector2f(HealthBar.getGlobalBounds().width / 2.f, HealthBar.getGlobalBounds().height / 2.f);
	HealthBar.setOrigin(vecCenter);
	HealthBar.setPosition(vecPos);
	HealthBar.setSize({ static_cast<float>(iWidth) - 1.f, 3.f });
	HealthBar.setFillColor(clrHealth);

	return HealthBar;
}

void CTarget::DrawHealth(RenderWindow* pRenderWindow)
{
	int iHP = this->GetHealth();
	int iMaxHP = 100;

	Vector2f vecPos = this->getPosition();
	vecPos.x -= (this->getGlobalBounds().width / 2.f) - 10.f;
	vecPos.y -= (this->getGlobalBounds().height / 2.f) - 10.f;


	RectangleShape Health = CreateHealthBar(vecPos, GetHealthColor(iHP, iMaxHP), GetHealthWidth(iHP, iMaxHP));
	pRenderWindow->draw(Health);
}

void DrawTankBBOX(CTank& pTarget, RenderWindow* pWindow)
{
	RectangleShape BBOX;
	BBOX.setFillColor(Color(0, 0, 0, 0));
	BBOX.setOutlineColor(Color::Red);
	BBOX.setOutlineThickness(1.f);
	BBOX.setOrigin(pTarget.getOrigin());
	BBOX.setPosition(pTarget.getPosition());
	BBOX.setSize(Vector2f(pTarget.getLocalBounds().width, pTarget.getLocalBounds().height));
	pWindow->draw(BBOX);
}

void DrawTargetBBOX(CTarget* pTarget, RenderWindow* pWindow)
{
	RectangleShape BBOX;
	BBOX.setFillColor(Color(0, 0, 0, 0));
	BBOX.setOutlineColor(Color::Red);
	BBOX.setOutlineThickness(1.f);
	BBOX.setOrigin(pTarget->getOrigin());
	BBOX.setPosition(pTarget->getPosition());
	BBOX.setSize(Vector2f(pTarget->getGlobalBounds().width, pTarget->getGlobalBounds().height));
	pWindow->draw(BBOX);
}

void CTarget::Draw(RenderWindow* pRenderWindow)
{
	if (this->GetIsMarkedForDeletion())
		return;

	this->DrawHealth(pRenderWindow);
	pRenderWindow->draw(*this);
}

void CGame::CreateNewTarget()
{
	CTarget* pTarget = CreateTarget();
	this->m_Targets.push_back(pTarget);
}

void CGame::DeleteTarget(CTarget* pTargetToDelete, int index)
{
	delete pTargetToDelete;
	this->m_Targets.erase(this->m_Targets.begin() + index);
}

void CGame::SpawnTargets()
{
	//create 5 targets.
	for (static UINT i = 0; i < 5; i++)
		this->CreateNewTarget();
}

void CGame::CreateAndManageTargets(CTank* pTank, RenderWindow* pRenderWindow)
{
	this->SpawnTargets();

	for (UINT i = 0; i < this->m_Targets.size(); i++)
	{
		CTarget* pTarget = m_Targets[i];

		pTarget->Think(*pTank);

		if (pTarget->GetIsMarkedForDeletion())
			this->DeleteTarget(pTarget, i);

		pTarget->Draw(pRenderWindow);
	}

	if (gGameInfo.m_bQueueNextTarget)
		this->CreateNewTarget();
}



void PlayGame(RenderWindow* pRenderWindow, CTank* Tank, CGame* pGame)
{
	Tank->MouseMove();
	Tank->MoveTank();
	{
		Tank->SpawnBullet();
		Tank->SimulateBullet();
		Tank->DrawBullet(pRenderWindow);
		pGame->CreateAndManageTargets(Tank, pRenderWindow);
	}
	Tank->Draw(pRenderWindow);

	DrawCrosshair(pRenderWindow);
}