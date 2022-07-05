#pragma once
#include <string>

#include "DisplayString.h"

class Client
{
public:
	Client(const char* argv);
	~Client();
	bool unload();
	bool isInit() const;
	bool init();
	bool deltaLoop();
	void drawFPS();
private:
	bool			m_isInit = false;
	std::string		m_exec_path;
	std::shared_ptr<DisplayString>	fps;
};
