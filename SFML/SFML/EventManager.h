#pragma once
#include <iostream>
#include <string>
#include <string_view>
#include <list>
#include <sstream>
#include <algorithm>

class CEventManager
{
public:
	void DispatchEvent(std::string strType);
private:
	std::list<std::string> m_EventTrack;
};

