#pragma once
#define WIN32_MEAN_AND_LEAN
#include <WS2tcpip.h>
#include <exception>
#include <sstream>
#include <cstring>
#include <iostream>
#include "BenchMarker.h"
#pragma comment(lib, "ws2_32.lib")

class UDPServer
{

public:
	UDPServer(unsigned short port_in)
		:
		port(port_in)
	{
		// Startup Winsock
		WSADATA data;
		WORD version = MAKEWORD(2, 2);
		int wsOk = WSAStartup(version, &data);
		if (wsOk != 0)
		{
			throw std::exception("Cannot Start Winsock Networking");
		}

		//Bind socket to port, Any Address
		s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		//Hint structure
		sockaddr_in serverHint;
		serverHint.sin_addr.S_un.S_addr = ADDR_ANY;
		serverHint.sin_family = AF_INET;
		serverHint.sin_port = htons(port);

		if (bind(s, (sockaddr*)&serverHint, sizeof(serverHint)) == SOCKET_ERROR)
		{
			std::stringstream ss;
			ss << "Cannot Bind Socket " << WSAGetLastError();
			OutputDebugStringA(ss.str().c_str());
			throw std::exception(ss.str().c_str());
		}
		else
		{
			OutputDebugString(L"Created and bound server socket Success!\n");
		}
	}
	~UDPServer()
	{
		OutputDebugString(L"WSA Cleanup called on socket\n");
		closesocket(s);
		WSACleanup();
	}

	std::string GetStatusReadout() const
	{
		PROFILE_FUNCTION();
		return status;
	}

	bool Recieve()
	{
		PROFILE_FUNCTION();
		ZeroMemory(&client, clientLength);

		if (dataAvailable(s))
		{
			ZeroMemory(messageBuffer, bufferSize);
			int bytesIn = recvfrom(s, messageBuffer, bufferSize, 0, (sockaddr*)&client, &clientLength);
			if (bytesIn == SOCKET_ERROR)
			{
				OutputDebugString(L"Error recieving from client\n");
			}
			char clientIP[256];
			ZeroMemory(clientIP, 256);

			inet_ntop(AF_INET, &client.sin_addr, clientIP, 256);

			message = messageBuffer;
			std::string systemmsg = message.substr(0, 8U);
			message.erase(0, 8U);

			std::ostringstream oss;
			oss << "Control State: " << clientIP << " : " << systemmsg << std::endl;
			status = oss.str();
			return true;
		}
		return false;
	}

	std::string GetMessageForGUI()
	{
		PROFILE_FUNCTION();
		std::ostringstream oss;
		oss << "Message Reads: " << message << std::endl;
		return oss.str();
	}
	std::string GetNetworkMessage()
	{
		PROFILE_FUNCTION();
		std::string message = messageBuffer;
		return message;
	}
	std::wstring GetNetworkMessageW()
	{
		PROFILE_FUNCTION();
		size_t size = sizeof(messageBuffer) + 1;
		static wchar_t wbuffer[bufferSize];
		size_t outSize;
		mbstowcs_s(&outSize, wbuffer, size, messageBuffer, size);
		std::wstring message = wbuffer;
		return message;
	}

private:
	bool dataAvailable(int sock, int interval = 600)
	{
		PROFILE_FUNCTION();
		fd_set fds;
		FD_ZERO(&fds);
		FD_SET(sock, &fds);

		timeval tv;
		tv.tv_sec = 0;
		tv.tv_usec = interval;

		return (select(sock + 1, &fds, 0, 0, &tv) == 1);
	}
private:
	SOCKET s;
	sockaddr_in client;
	int clientLength = sizeof(client);
	static constexpr int bufferSize = 512;
	unsigned short port;
	char messageBuffer[bufferSize] = {};
	std::string status;
	std::string message;
	
};

