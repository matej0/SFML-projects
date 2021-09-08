#include "GUI.h"
IControlInfo g_ControlInfo;

bool bmeme1 = false;


void CGui::RegisterAll()
{
	meme1.Register("meme1");

}

void CGui::Draw(RenderWindow* pWindow)
{
	meme1.Draw(pWindow, &bmeme1);
}