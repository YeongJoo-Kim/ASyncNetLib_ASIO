#include "stdafx.h"
#include "Server.h"
//#include<boost/tokenizer.hpp>

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

//	s.delegate_accept += &OnAccept;
	s.delegate_accept = boost::bind(&Server::OnAccept, *this);
	ioService.run();
	
	return true;
}
