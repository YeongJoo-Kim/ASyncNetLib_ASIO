#pragma once
#include <boost/thread.hpp>
#include "AsyncTcpServer.hpp"

#include <iostream>
using namespace std;


class Server
{
public:
	Server(void);
	~Server(void);

	bool Begin(unsigned short nPort = 3333);
	void OnAccept(void)
	{
		std::cout << "OnAccept!! " << endl;
	};

	void WriteAll()
	{


	}

protected:
	short	m_nPort;
	//boost::asio::io_service io_service;
};

