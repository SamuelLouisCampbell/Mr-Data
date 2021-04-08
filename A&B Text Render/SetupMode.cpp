#include "SetupMode.h"


float SetupMode::ReadFloat(const char* delimiter, std::ifstream& file, std::string& line, std::string::size_type& n)
{
	if (file.is_open())
	{
		std::getline(file, line);
		n = line.find(delimiter);
		std::string substr = line.substr(n + 1, line.size());
		return std::stof(substr);
	}
	else
	{
		return -1.0f;
	}
}

int SetupMode::ReadInt(const char* delimiter, std::ifstream& file, std::string& line, std::string::size_type& n)
{
	if (file.is_open())
	{
		std::getline(file, line);
		n = line.find(delimiter);
		std::string substr = line.substr(n + 1, line.size());
		return std::stoi(substr);
	}
	else
	{
		return -1;
	}
}

Color SetupMode::ReadColor(const char* delimiter, std::ifstream& file, std::string& line, std::string::size_type& n)
{
	if (file.is_open())
	{
		std::getline(file, line);
		n = line.find(delimiter);
		Color col;
		col.r = float(std::stoi(line.substr(n + 1, 2), 0, 16));
		col.g = float(std::stoi(line.substr(n + 3, 2), 0, 16));
		col.b = float(std::stoi(line.substr(n + 5, 2), 0, 16));
		col.a = float(std::stoi(line.substr(n + 7, 2), 0, 16));
		
		return { col.r / 255.0f,col.g / 255.0f,col.b / 255.0f,col.a / 255.0f };
	}
	else
	{
		return { 0.0f,1.0f,0.0f,0.0f };
	}
}

bool SetupMode::Init(const std::string filename)
{
	isInit = true;
	std::string line;
	std::string::size_type n;
	std::ifstream defFile(filename);
	if (defFile)
	{
		//serverPort
		serverPort = ReadInt("#", defFile, line, n);
	
		ts.currFillCol = ReadColor("#", defFile, line, n);
		ts.oldFillCol = ts.currFillCol;
		ts.currOutlineCol = ReadColor("#", defFile, line, n);
		ts.oldOutlineCol = ts.currOutlineCol;
		ts.largeScale = ReadFloat("#", defFile, line, n);
		ts.smallScale = ReadFloat("#", defFile, line, n);
		ts.lineSpacing = ReadFloat("#", defFile, line, n);
		ts.strokeWidth = ReadFloat("#", defFile, line, n);
		ts.kerning = ReadFloat("#", defFile, line, n);
		ts.rotaion = ReadFloat("#", defFile, line, n);
		ts.deltaZoom = ReadFloat("#", defFile, line, n);
		ts.deltaAlpha = ReadFloat("#", defFile, line, n);
		ts.offsetX = ReadFloat("#", defFile, line, n);
		ts.offsetY = ReadFloat("#", defFile, line, n);
		
		defFile.close();
	
		return true;
	}
	return false;
}

void SetupMode::Update()
{
	if (!isInit)
	{
		Init("assets/renderer_defaults.txt");
	}
	if (ImGui::Begin("Setup System"))
	{
		ImGui::InputInt("Outgoing Port Number 0 - 65535", &serverPort);
		if (ImGui::Button("Compete Setup"))
		{
			SetSetupMode(true);
		}
	}
	ImGui::End();
}

RMData SetupMode::GetRMData() const
{
	return { size_t(serverPort), ts };
}

bool SetupMode::SetupComplete() const
{
	return setupComplete;
}

void SetupMode::SetSetupMode(bool setMode)
{
	setupComplete = setMode;
}



