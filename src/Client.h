#pragma once
#include "DemoServiceLocator.h"
#include "DisplayString.h"

class Client
{
public:
	Client(const char* argv);
	~Client();
	bool					unload();
	bool					isInit() const;
	bool					init();
	bool					InitEngine();
	bool					IsEngineInit() const;
	[[nodiscard]] std::shared_ptr<Engine> GetEngine() const;
	void					Terminate();
	bool					deltaLoop();
	void					drawFPS();
private:
	bool								m_isInit = false;
	bool								m_isEngineInit = false;
	std::string							m_exec_path;
	std::shared_ptr<Engine>				m_pAppEngine = nullptr;
};
