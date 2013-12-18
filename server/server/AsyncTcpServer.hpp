//
// async_tcp_echo_server.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#define BOOST_ASIO_HAS_MOVE
#include <SDKDDKVer.h>

#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <set>
#include <boost/asio.hpp>
#include <boost/thread/mutex.hpp>



#include "AsyncConnectionManager.hpp"

using boost::asio::ip::tcp;

class AsyncTcpServer
{
public:
	AsyncTcpServer(boost::asio::io_service& ioService, short nPort) :
		mSocket(ioService)
		, mAcceptor(ioService, tcp::endpoint(tcp::v4(), nPort))
	{
		do_accept();
	};


/*
	void run() {
		start_listening();
		ioService.run();
	};



	void start_listening()
	{
		using boost::asio::ip::tcp;

		boost::system::error_code error;
		boost::unique_lock<boost::mutex> listening_lock(listening_mutex_);

		mAcceptor.listen(boost::asio::socket_base::max_connections, error);

		mAcceptor.async_accept(mSocket, [this](boost::system::error_code ec)
		{
			if (!ec)
			{
				std::shared_ptr<AsyncConnection> connection = std::make_shared<AsyncConnection>(std::move(mSocket));
				connection->Begin();
				m_Connections.insert(connection);
			}

			DoAccept();
		});


	}*/


private:
	int  do_accept()
	{
		mAcceptor.async_accept(mSocket, [this](boost::system::error_code ec)
		{
			if (!ec)
			{
				AsyncConnect_ptr session = std::make_shared<AsyncConnection>(std::move(mSocket));

				m_ConnectionManager.begin(session);

				//call callback OnAccept(session);
			}

			do_accept();
		});

		return 0;

	};



	bool WriteAll(unsigned char* buffer, int len)
	{
	/*	for_each(m_Connections.begin(), m_Connections.end(), [](std::shared_ptr<AsyncConnection> connection) 
		{ 
			//connection->Write(buffer, len);
		}
		);
		*/
		return true;
	}

	AsyncConnectionManager		m_ConnectionManager;


	tcp::acceptor	mAcceptor;
	tcp::socket		mSocket;
	
	//std::shared_ptr<boost::asio::io_service> self_service_;
	//boost::asio::io_service & ioService;

	boost::mutex listening_mutex_;

//	delegate  delegate_accept;
};
