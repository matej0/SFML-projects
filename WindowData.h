#pragma once
#include "SFML/Graphics.hpp"
#include <Windows.h>
#include <deque>
#include <sstream>
#include <array>
#include <random>
#pragma warning( disable : 4244 ) 
#define PI 3.14159265358979323846f
#define RAD2DEG( x ) ( ( float )( x ) * ( float )( 180.0f / ( float )( PI ) ) )
#define DEG2RAD( x ) ( ( float )( x ) * ( float )( ( float )( PI ) / 180.0f ) )
class CWindow
{
public:
	unsigned int width;
	unsigned int height;
	sf::Font font;
	double curtime;
	float deltaTime;
	float center[2];
	PVOID pRenderWindowPointer;

	int Random(int min, int max)
	{
		std::random_device                  Device;
		std::mt19937                        Generator(Device());
		std::uniform_int_distribution<int>    Distr(min, max);

		return Distr(Generator);
	};
};
extern CWindow g_WindowData;