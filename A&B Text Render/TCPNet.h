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
	HealthCheckServer,
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
	std::string GetMessageStream() const
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
			std::stringstream str;
 			info << "[" << connection->GetUUID() << "]: Has Sent a Message!\n";
			connection->Send(msg);
			for (size_t i = 0; i < msg.size(); i++)
			{
				str << msg.body[i];
			}
			information = info.str();
			messagestr = str.str();
			break;
		}
		default:
			break;
		}
	}
	

private:
	std::string information = "";
	std::string messagestr = "";

};