#pragma once
#include "RenderMode.h"
#include <fstream>

class SetupMode
{
public:
	bool Init(const std::string filename);
	void Update();
	RMData GetRMData() const;
	bool SetupComplete() const;
	void SetSetupMode(bool setMode);
private:
	float ReadFloat(const char* delimiter, std::ifstream& file, std::string& line, std::string::size_type& n);
	int ReadInt(const char* delimiter, std::ifstream& file, std::string& line, std::string::size_type& n);
	Color ReadColor(const char* delimiter, std::ifstream& file, std::string& line, std::string::size_type& n);

	bool isInit = false;
	int serverPort = 60000;
	float largeScale;
	float smallScale;
	float lineSpacing;
	bool setupComplete = false;
	std::vector<TextSettings> textSettings;
};

