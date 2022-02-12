#include "BirdWorld.h"


void CBirdWorld::LoadTextures()
{
	Texture BirdWingUp;
	BirdWingUp.loadFromFile("red-wing-up.png");
	this->m_BirdTextures.push_back(BirdWingUp);
	
	Texture BirdWingDown;
	BirdWingDown.loadFromFile("red-wing-down.png");
	this->m_BirdTextures.push_back(BirdWingDown);

	Texture BirdWingMiddle;
	BirdWingMiddle.loadFromFile("red-wing-middle.png");
	this->m_BirdTextures.push_back(BirdWingMiddle);

	this->m_PipeTexture.loadFromFile("pipe.png");
}

void CBirdWorld::CreateAllPipes()
{
	float flStartPosition = g_WindowData.width;

	for (int i = 0; i < 3; i++)
	{
		CPipe NewPipe;
		NewPipe.SpawnPipes(this->m_PipeTexture, flStartPosition);
		this->m_Pipes.push_back(NewPipe);
		flStartPosition += 300.f;
	}
}

void CBirdWorld::CleanUpPipes()
{
	this->m_Pipes.clear();
}

void CBirdWorld::PipesThink()
{
	if (this->m_Pipes.empty())
		return;

	for (auto &Pipe : this->m_Pipes)
	{	
		Pipe.Think();
	}
}

void CBirdWorld::DrawAllPipes(RenderWindow* pWindow)
{
	if (this->m_Pipes.empty())
		return;

	for (auto &Pipe : this->m_Pipes)
		Pipe.Draw(pWindow);
}


bool CBirdWorld::ShouldLose(CBird* Bird)
{
	if (Bird->IsOutOfBounds())
		return true;

	for (auto Pipe : this->m_Pipes)
	{
		if (Bird->getGlobalBounds().intersects(Pipe.m_Bottom.getGlobalBounds()) || Bird->getGlobalBounds().intersects(Pipe.m_Top.getGlobalBounds()))
			return true;
	}

	return false;
}

void CBirdWorld::DrawAllElements(RenderWindow* pWindow, CBird* Bird)
{
	if (!Bird)
	{
		std::cout << "Bird was NULL!" << std::endl;
		return;
	}

	this->DrawAllPipes(pWindow);
	Bird->Draw(pWindow);
}

void CBirdWorld::Initialize()
{
	//if a bird exists, delete it.
	if (m_pBird)
		delete m_pBird;

	this->m_pBird = new CBird(this->m_BirdTextures);
	this->CreateAllPipes();
}

//Handles both drawing and simulating.
void CBirdWorld::Play(RenderWindow* pWindow)
{
	if (this->ShouldLose(this->m_pBird))
	{
		this->CleanUpPipes();
		this->Initialize();
	}

	this->PipesThink();
	this->m_pBird->Think();

	this->DrawAllElements(pWindow, this->m_pBird);
}