/*
Trapper принимает на вход данные типов:
unsigned int, float, char и string
При создании класса указывают:
ip хоста, его порт, нужна ли async передача 
и время жизни сообщения
С помощью setter указывают:
Имя отправителя и ключ элемента данных
Для отправки используется функция sendData
Функция asyncCheckControl проверяет статус
асинхронного сообщения
Максимальная длина сообщения:
65534 с вычетом сервисных знаков (-71)
*/

#ifndef ZABBIXTRAPPER_H
#define ZABBIXTRAPPER_H

#include <string>
#include <queue>
#include <thread>
#include <iostream>
#include <sstream>
#include "boost/atomic.hpp"
#include "boost/chrono.hpp"

enum class Error : int {
	succesed = 1,
	failed_connect = 2,
	failed_send = 3,
	too_long_message = 4,
	not_enough_time = 5, //not enoug time for sync, async require additional checking
	type_error = 6,
	message_does_not_exist = 7
};

class ZabbixTrapper
{
public:
	ZabbixTrapper(const std::string zabbixHost, int zabbixPort, bool ready = false, int ttl = 100) :
		zabbixHost(zabbixHost), zabbixPort(zabbixPort), ready(ready), ttl(ttl)
	{
		std::thread sender(&ZabbixTrapper::queueControl, this);
		sender.detach();
	} // адрес хоста, его порт, нужен ли async (0 - нет, 1 - да) и time_to_life для сообщения

    virtual ~ZabbixTrapper() 
	{ 
		ready = false; 
	}

	std::atomic<bool> ready;

    virtual void setter(const std::string clientHost, const std::string clientKey); // имя узла, как оно указано на сервере, и ключ элемента данных соответственно

    template<typename Type> int sendData(Type rawData, bool sync)
	{
		std::string data;
		if ((std::is_same<Type, char>::value) || (std::is_same<Type, std::string>::value)
			|| (std::is_same<Type, unsigned int>::value) || (std::is_same<Type, float>::value))
		{
			std::ostringstream temp;
			temp << rawData;
			data = temp.str();
		}
		else
		{
			return static_cast<int>(Error::type_error);
		}
		if (sync)
		{
			Error checkSending = Error::not_enough_time;
			std::thread sender(&ZabbixTrapper::sending, this, data, &checkSending);
			sender.detach();
			std::this_thread::sleep_for(std::chrono::milliseconds(ttl));
			return static_cast<int>(checkSending);
		} 
		else
		{
			int asyncCheckSize = asyncCheck.size();
			queue.push(data);
			return asyncCheckSize+1; //возвращает количество сообщений после PUSH
									//по этому числу можно будет проверить статус отправки
		}
	};

protected:
	int ttl;
    int zabbixPort;
    std::string zabbixHost;
    std::string clientHost;
    std::string clientKey;
    std::queue<std::string> queue;
    virtual Error sending(std::string value, Error *checkSending);
    virtual void queueControl();
	virtual Error asyncCheckControl(unsigned int numberMessage);
	std::vector<Error> asyncCheck;
};

#endif // ZABBIXTRAPPER_H
