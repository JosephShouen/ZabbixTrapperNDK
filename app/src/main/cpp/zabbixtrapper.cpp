#include "zabbixtrapper.h"
#include "boost/asio.hpp"
namespace asio = boost::asio;

static const std::vector<std::string> serverData {"{ \"request\":\"sender data\", \"data\":[ {\"host\":\"", "\",\"key\":\"", "\",\"value\":\"", "\" } ]}"};
static const unsigned int serverDataLength = serverData[0].length()+serverData[1].length()+serverData[2].length()+serverData[3].length();
static const unsigned int sizeOfReferenceToDataLen = 13;
static const unsigned int fullMessageSize = 65535;
static const unsigned int minMessageSize = 1023;
static const unsigned int queueCleanSize = 100;

void ZabbixTrapper::setter(std::string clientHost_, std::string clientKey_)
{
	clientHost = clientHost_;
	clientKey = clientKey_;
}

void ZabbixTrapper::queueControl()
{
	while (ready)
	{
		if (!queue.empty())
		{	
			Error checkSending = Error::not_enough_time;
			asyncCheck.push_back(checkSending);
			unsigned int asyncCheckSize = asyncCheck.size();
			std::string data = queue.front();
			queue.pop();
			asyncCheck[asyncCheckSize - 1] = sending(data, &checkSending);
		}
		if (queue.size() > queueCleanSize)
		{
			queue.empty();
			asyncCheck.clear();
		}
	}
}

Error ZabbixTrapper::sending(std::string value, Error *checkSending)
{
	asio::io_service io_service;
	asio::ip::tcp::socket socket(io_service);
	try
	{
		socket.connect(asio::ip::tcp::endpoint(asio::ip::address::from_string(zabbixHost), zabbixPort));
	}
	catch (std::exception & e)
	{
		*checkSending = Error::failed_connect;
		return Error::failed_connect;
	}
	size_t valueLen = value.length();
	unsigned int serviceSize = std::string(clientHost + clientKey).size() + serverDataLength;
	if ((value.length() + serviceSize) > fullMessageSize)
	{
//		valueLen = fullMessageSize - serviceSize;
		*checkSending = Error::too_long_message;
		return Error::too_long_message;
	}
		

	std::string information = std::string(serverData[0])
		+ clientHost
		+ std::string(serverData[1])
		+ clientKey
		+ std::string(serverData[2])
		+ value.substr(0, valueLen)
		+ std::string(serverData[3]);
	size_t dataLen = information.length();

	std::vector<char> msg(minMessageSize + sizeOfReferenceToDataLen);
	if (dataLen > minMessageSize)
		msg.resize(dataLen + sizeOfReferenceToDataLen);
	memset(&msg[0], 0x00, dataLen + sizeOfReferenceToDataLen);
	memcpy(&msg[0], "ZBXD", sizeof("ZBXD"));
	msg[4] = 0x01;
	memcpy(&msg[5], &dataLen, sizeof(uint16_t));
	memcpy(&msg[sizeOfReferenceToDataLen], information.c_str(), information.length());
	boost::system::error_code error;
	asio::write(socket, asio::buffer(msg, dataLen + sizeOfReferenceToDataLen), error);
	if (error) {
		*checkSending = Error::failed_send;
		return Error::failed_send;
	}
	else {
		*checkSending = Error::succesed;
		return Error::succesed;
	}
}

Error ZabbixTrapper::asyncCheckControl(unsigned int numberMessage) 
{
	if (asyncCheck.size() <= numberMessage)
	{
		return(asyncCheck[numberMessage - 1]);
	}
	return Error::message_does_not_exist;
}