#pragma once
#include "RenderMode.h"
class SetupMode
{
public:
	void Update();
	void Render();
	RMData GetRMData() const;
	bool InSetupMode() const;
	void SetSetupMode(bool setMode);
private:
	RMData rmd;
	int serverPort = 6000;
	int clientPort = 5000;
	std::string IP = "127.0.0.1";
	bool finished = false;
};

