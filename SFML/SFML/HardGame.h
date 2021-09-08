#pragma once
#include <SFML/Graphics.hpp>
#include "WindowData.h"
#include <iostream>
#include <Windows.h>
#include <random>
#include <deque>
#include <math.h>
#include <algorithm>

using namespace sf;
int A_RAND(int min, int max);

struct ITileMapProperties
{
	unsigned int width;
	unsigned int height;
};
extern ITileMapProperties gTileMapProps;

class CObstacle;

class CPlayer : public RectangleShape
{
public:
	CPlayer()
	{
		this->setOutlineThickness(5.f);
		this->setOutlineColor(Color(100, 0, 0, 255));
		this->setFillColor(Color::Red);
		this->setPosition({ 10.f, 50.f });
		this->setSize({ 12.f, 12.f });
	}
	void Think();
	bool HitObstacle(CObstacle & obstacle);
	void Draw(RenderWindow * pWindow);

private:
	Vector2f m_vecVelocity;
};


class CObstacle : public CircleShape
{
public:
	CObstacle(Vector2f vPos)
	{
		this->setFillColor(Color::Blue);
		this->setOutlineThickness(3.f);
		this->setOutlineColor(Color(0, 0, 100, 255));
		this->setRadius(7.f);
		this->setOrigin(this->getRadius(), this->getRadius());
		this->setPosition(vPos);
	}

	void StartMoving();

	void Think();

	void Draw(RenderWindow * pWindow);

	Vector2f m_vecVelocity;
	bool m_bShouldStartWithInverseVelocity;

private:

};

class CWorld : public Drawable, public Transformable
{
public:
	CWorld(std::string strFileName, const int* iTiles, UINT uiWidth, UINT uiHeight, Vector2u vecTileSize)
	{
		if (!this->m_TextureAtlas.loadFromFile(strFileName))
		{
			//ass idk
		}

		this->m_Vertices.setPrimitiveType(Quads);
		this->m_Vertices.resize(uiWidth * uiHeight * 4);

		for (UINT i = 0; i < uiWidth; ++i)
		{
			for (UINT j = 0; j < uiHeight; ++j)
			{
				int tileNumber = iTiles[i + j * uiWidth];

				int tu = tileNumber % (m_TextureAtlas.getSize().x / vecTileSize.x);
				int tv = tileNumber / (m_TextureAtlas.getSize().x / vecTileSize.x);

				sf::Vertex* quad = &m_Vertices[(i + j * uiWidth) * 4];

				quad[0].position = sf::Vector2f(i * vecTileSize.x, j * vecTileSize.y);
				quad[1].position = sf::Vector2f((i + 1) * vecTileSize.x, j * vecTileSize.y);
				quad[2].position = sf::Vector2f((i + 1) * vecTileSize.x, (j + 1) * vecTileSize.y);
				quad[3].position = sf::Vector2f(i * vecTileSize.x, (j + 1) * vecTileSize.y);

				quad[0].texCoords = sf::Vector2f(tu * vecTileSize.x, tv * vecTileSize.y);
				quad[1].texCoords = sf::Vector2f((tu + 1) * vecTileSize.x, tv * vecTileSize.y);
				quad[2].texCoords = sf::Vector2f((tu + 1) * vecTileSize.x, (tv + 1) * vecTileSize.y);
				quad[3].texCoords = sf::Vector2f(tu * vecTileSize.x, (tv + 1) * vecTileSize.y);
			}
		}
	
	}

	virtual void draw(RenderTarget& target, RenderStates states) const
	{
		// apply the transform
		states.transform *= getTransform();

		// apply the tileset texture
		states.texture = &m_TextureAtlas;

		// draw the vertex array
		target.draw(m_Vertices, states);
	}

private:
	VertexArray m_Vertices;
	Texture m_TextureAtlas;
};

extern std::vector<CObstacle> rgObstacles;
extern Vector2f vPos;
void ResetGame(CPlayer& player);
