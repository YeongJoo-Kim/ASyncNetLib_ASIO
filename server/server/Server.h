#pragma once
#include <boost/thread.hpp>
#include "AsyncTcpServer.hpp"


class Server
{
public:
	Server(void);
	~Server(void);

	bool Begin(unsigned short nPort = 3333);

	void WriteAll()
	{


	}

protected:
	short	m_nPort;
	//boost::asio::io_service io_service;
};

