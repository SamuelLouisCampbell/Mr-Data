#pragma once
#include <string>
#include <vector>

class StringHandling
{
public:
	StringHandling(std::wstring str)
		:
		message(str)
	{
		if (message.size() >= 1)
		{
			while (message.find('\r') != std::string::npos)
			{
				std::wstring temp = message.substr(0, message.find('\r'));
				stringies.push_back(temp);
				message.erase(0, message.find('\r') + 1);

			}
			while (message.find('\n')!= std::string::npos)
			{
				std::wstring temp = message.substr(0, message.find('\n'));
				stringies.push_back(temp);
				message.erase(0, message.find('\n') + 1);

			}
			stringies.push_back(message);
		}
	}

	std::vector<std::wstring> GetStringies() const
	{
		return stringies;
	}

private:
	std::wstring message;
	std::vector<std::wstring> stringies;

};
