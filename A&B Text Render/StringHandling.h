#pragma once
#include <string>
#include <vector>

class StringHandling
{
public:
	StringHandling(std::string str)
		:
		message(str)
	{
		if (message.size() >= 1)
		{
			while (message.find('\r') != std::string::npos)
			{
				std::string temp = message.substr(0, message.find('\r'));
				stringies.push_back(temp);
				message.erase(0, message.find('\r') + 1);

			}
			while (message.find('\n')!= std::string::npos)
			{
				std::string temp = message.substr(0, message.find('\n'));
				stringies.push_back(temp);
				message.erase(0, message.find('\n') + 1);

			}
			stringies.push_back(message);
		}
	}

	std::vector<std::string> GetStringies() const
	{
		return stringies;
	}

private:
	std::string message;
	std::vector<std::string> stringies;

};
