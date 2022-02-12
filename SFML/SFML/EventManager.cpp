#include "EventManager.h"

void CEventManager::DispatchEvent(std::string strType)
{
	//to do...
	this->m_EventTrack.push_back(strType);
}