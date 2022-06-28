#pragma once
#include <string>

class Client
{
public:
	Client(const char* argv);
	~Client();
	bool unload();
	bool isInit() const;
	bool init();
	bool deltaLoop();
private:
	bool		m_isInit = false;
	std::string m_exec_path;
};
