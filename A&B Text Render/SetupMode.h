#pragma once
#include "RenderMode.h"
class SetupMode
{
public:
	bool Init(const std::string filename);
	void Update();
	RMData GetRMData() const;
	bool SetupComplete() const;
	void SetSetupMode(bool setMode);
private:
	bool isInit = false;
	int serverPort = 60000;
	std::string IP = "127.0.0.1";
	float largeScale;
	float smallScale;
	float lineSpacingLarge;
	float lineSpacingSmall;
	bool setupComplete = false;
};

