#pragma once
#define WIN32_MEAN_AND_LEAN
#include <WS2tcpip.h>
#include <exception>
#include <sstream>
#include <cstring>
#include <thread>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

class UDPClient
{
public:
	UDPClient(const unsigned short port_in = 5000, const char* ip_addr = "127.0.0.1")
		:
		port(port_in),
		ip_addr(ip_addr)
	{

		WSADATA data;
		WORD version = MAKEWORD(2, 2);
		int wsOk = WSAStartup(version, &data);
		if (wsOk != 0)
		{
			std::stringstream ss;
			ss << "Cannot Bind Socket " << WSAGetLastError();
			OutputDebugStringA(ss.str().c_str());
			throw std::exception(ss.str().c_str());
		}

		//bind to socket
		out = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		//Fill out hint structure
		serverHint.sin_family = AF_INET;
		serverHint.sin_port = htons(port);
		if (inet_pton(AF_INET, ip_addr, &serverHint.sin_addr) != 1)
		{
			std::stringstream ss;
			ss << "Bad IP Address " << WSAGetLastError();
			OutputDebugStringA(ss.str().c_str());
			throw std::exception(ss.str().c_str());
		}
	}
	~UDPClient()
	{
		closesocket(out);
		WSACleanup();
	}
	void UDP_Send(std::string msg)
	{
		int sendOK = sendto(out, msg.c_str(), msg.size() + 1, 0, (sockaddr*)&serverHint, sizeof(serverHint));

		if (sendOK == SOCKET_ERROR)
		{
			std::stringstream ss;
			ss << "Cannot Send Data " << WSAGetLastError();
			OutputDebugStringA(ss.str().c_str());
			throw std::exception(ss.str().c_str());
		}
	}

private:
	const unsigned short port; 
	const char* ip_addr;
	SOCKET out;
	sockaddr_in serverHint;
};
