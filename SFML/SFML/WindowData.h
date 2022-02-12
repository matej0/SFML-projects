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

#if defined(__GNUC__) || defined(__clang__)
#define DEPRECATED __attribute__((deprecated))
#elif defined(_MSC_VER)
#define DEPRECATED __declspec(deprecated)
#else
#pragma message("WARNING: You need to implement DEPRECATED for this compiler")
#define DEPRECATED
#endif

struct screencoords_t
{
	float x, y;
};

class CWindow
{
public:
	int width;
	int height;
	int fps;
	sf::Font font;
	float deltaTime;
	screencoords_t center;
	PVOID pRenderWindowPointer;
	PVOID pCurTime;

	int RandomInt(int min, int max)
	{
		std::random_device                  Device;
		std::mt19937                        Generator(Device());
		std::uniform_int_distribution<int>    Distr(min, max);

		return Distr(Generator);
	};

	float RandomFloat(float min, float max)
	{
		std::random_device                  Device;
		std::mt19937                        Generator(Device());
		std::uniform_real_distribution<float>    Distr(min, max);

		return Distr(Generator);
	};
};
extern CWindow g_WindowData;