#pragma once
#include <NetCommon.h>
#include <NetServer.h>
#include <iostream>
#include <string>
#include <locale>
#include <codecvt>

enum class CustomMsgType : uint32_t
{
	ServerAccept,
	ServerDeny,
	ServerPing,
	MessageServer,
	EchoMessage,
	HealthCheckServer,
	ServerValidated,
	MessageVec,
};

class CustomServer : public netcommon::NetServer<CustomMsgType>
{
public:
	CustomServer(uint16_t nPort)
		: 
		netcommon::NetServer<CustomMsgType>(nPort)
	{

	}
	std::string GetInfoStream() const
 	{
		return information;
	}
	std::wstring GetMessageStream() const
	{
		return messagestr;
	}
	void CheckClientsHealth()
	{
		netcommon::message<CustomMsgType> msg;
		msg.header.id = CustomMsgType::HealthCheckServer;
		MessageAllClients(msg);
	}
protected:
	virtual void OnClientValidated(std::shared_ptr<netcommon::connection<CustomMsgType>> connection) override
	{
		netcommon::message<CustomMsgType> msg;
		msg.header.id = CustomMsgType::ServerValidated;
		connection->Send(msg);
	}
	virtual bool OnClientConnect(std::shared_ptr<netcommon::connection<CustomMsgType>> connection) override
	{
		netcommon::message<CustomMsgType> msg;
		msg.header.id = CustomMsgType::ServerAccept;
		connection->Send(msg);
		std::stringstream ss;
		ss << "Client Connected [" << connection->GetUUID() << "]\n";
		information = ss.str();
		return true;
	}
	virtual void OnClientDisconnect(std::shared_ptr<netcommon::connection<CustomMsgType>> connection) override
	{
		std::stringstream ss;
		if (connection != nullptr)
		{
			ss << "Client Disconnected [" << connection->GetUUID() << "]\n";
		}
		information = ss.str();
	}
	virtual void OnMessage(std::shared_ptr<netcommon::connection<CustomMsgType>> connection, const netcommon::message<CustomMsgType>& msg) override
	{
		switch (msg.header.id)
		{
		case CustomMsgType::ServerPing:
		{
			std::stringstream ss;
			ss << "[" << connection->GetUUID() << "]: Ping\n";
			connection->Send(msg);
			information = ss.str();
			break;
		}
		case CustomMsgType::MessageServer:
		{
			std::stringstream info;
			std::string str;
 			info << "[" << connection->GetUUID() << "]: Has Sent a Message!\n";
			connection->Send(msg);
			for (size_t i = 0; i < msg.size(); i++)
			{
				str.push_back(msg.body[i]);
			}
			//Deprecated c++11 
			/*std::wstring_convert<std::codecvt_utf8_utf16<char16_t>> converter;
			messagestr = converter.from_bytes(str);*/
			int wcharsNum = MultiByteToWideChar(CP_UTF8,0 , str.c_str(), -1, NULL, 0);
			wchar_t* wstr = new wchar_t[wcharsNum];
			MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wstr, wcharsNum);
			messagestr = wstr;
			delete wstr;
			information = info.str();
			break;
		}
		case CustomMsgType::MessageVec:
		{
			break;
		}
		default:
			break;
		}
	}
private:
	std::string information = "";
	std::wstring messagestr = L"";
};