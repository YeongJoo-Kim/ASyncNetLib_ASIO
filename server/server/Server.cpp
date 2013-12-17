#include "stdafx.h"
#include "Server.h"


Server::Server(void)
	//:m_nPort(3333)
{
}


Server::~Server(void)
{
}

bool Server::Begin(unsigned short nPort)
{

	boost::thread worker();
	boost::asio::io_service ioService;

	AsyncTcpServer s(ioService, nPort);

	ioService.run();

	return true;
}
