#ifndef SERVER_H
#define SERVER_H
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

namespace Server 
{
	void add_page(std::string page);
	void start_server();
}
#endif // !SERVER_H