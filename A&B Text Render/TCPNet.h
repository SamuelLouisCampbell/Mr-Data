#pragma once
#include <NetCommon.h>
#include <NetServer.h>

enum class CustomMsgType : uint32_t
{
	ServerAccept,
	ServerDeny,
	ServerPing,
	MessageServer,
	EchoMessage,
};

class CustomServer : public netcommon::NetServer<CustomMsgType>
{
public:
	CustomServer(uint16_t nPort)
		: 
		netcommon::NetServer<CustomMsgType>(nPort)
	{

	}
	std::string GetInfoStream()
	{
		return information;
	}
	std::string GetMessageStream()
	{
		return message;
	}
protected:
	virtual bool OnClientConnect(std::shared_ptr<netcommon::connection<CustomMsgType>> connection) override
	{
		netcommon::message<CustomMsgType> msg;
		msg.header.id = CustomMsgType::ServerAccept;
		connection->Send(msg);
		return true;
	}
	virtual void OnClientDisconnect(std::shared_ptr<netcommon::connection<CustomMsgType>> connection) override
	{
		std::cout << "Removing client [" << connection->GetUUID() << "]\n";
	}
	virtual void OnMessage(std::shared_ptr<netcommon::connection<CustomMsgType>> connection, const netcommon::message<CustomMsgType>& msg) override
	{
		switch (msg.header.id)
		{
		case CustomMsgType::ServerPing:
		{
			std::stringstream ss;
			ss << "[" << connection->GetUUID() << "]: Server Ping\n";
			connection->Send(msg);
			information = ss.str();
			break;
		}
		case CustomMsgType::MessageServer:
		{
			std::stringstream info;
			std::stringstream str;
 			info << "[" << connection->GetUUID() << "]: Has Sent a Message!\n";
			connection->Send(msg);
			str << msg.body.data();
			information = info.str();
			message = str.str();
			break;
		}
		default:
			break;
		}
	}

private:
	std::string information;
	std::string message;

};