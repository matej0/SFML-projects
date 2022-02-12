#include "Pipe.h"

float CPipe::ComputeRandomHeight()
{
	float flHeight = 0;
	flHeight = g_WindowData.RandomFloat(100.f, 250.f);
	return flHeight;
}

void CPipe::SpawnPipes(Texture& PipeTexture, float x)
{
	float flRandomHeight = this->ComputeRandomHeight();
	this->m_flStartPositionX = x;

	
	m_Top.setTexture(PipeTexture);
	m_Top.setTextureRect(IntRect({ 0, 0, PIPE_TEXTURE_WIDTH, int(flRandomHeight) }));
	m_Top.setOrigin({ float(m_Top.getTextureRect().width), float(m_Top.getTextureRect().height) });
	m_Top.setRotation(180);
	m_Top.setPosition(this->m_flStartPositionX, 0.f);

	m_Bottom.setTexture(PipeTexture);
	m_Bottom.setTextureRect(IntRect({ 0, 0, PIPE_TEXTURE_WIDTH, g_WindowData.height - m_Top.getTextureRect().height - 200}));
	m_Bottom.setPosition(this->m_flStartPositionX, (g_WindowData.height - m_Bottom.getTextureRect().height));
}

void CPipe::Draw(RenderWindow* pWindow)
{
	pWindow->draw(m_Top);
	pWindow->draw(m_Bottom);
}

void CPipe::MovePipes()
{
	const float flSpeed = 200.f;
	float flOffsetX = -flSpeed * g_WindowData.deltaTime;

	m_Top.move(flOffsetX, 0.f);
	m_Bottom.move(flOffsetX, 0.f);
}

bool CPipe::PipesAreOutOfScreen()
{
	return (m_Top.getPosition().x + m_Top.getGlobalBounds().width <= 0 && m_Bottom.getPosition().x + m_Top.getGlobalBounds().width <= 0);
}

void CPipe::ResetPositions(bool bShouldResetPositions)
{
	if (bShouldResetPositions)
	{
		float flRandomHeight = this->ComputeRandomHeight();

		m_Top.setTextureRect(IntRect({ 0, 0, PIPE_TEXTURE_WIDTH, int(flRandomHeight) }));
		m_Top.setOrigin({ float(m_Top.getTextureRect().width), float(m_Top.getTextureRect().height) });
		m_Top.setPosition(g_WindowData.width, 0.f);


		m_Bottom.setTextureRect(IntRect({ 0, 0, PIPE_TEXTURE_WIDTH, g_WindowData.height - m_Top.getTextureRect().height - 200 }));
		m_Bottom.setPosition(g_WindowData.width, (g_WindowData.height - m_Bottom.getTextureRect().height));
	}
}

void CPipe::Think()
{
	this->ResetPositions(this->PipesAreOutOfScreen());
	this->MovePipes();
}