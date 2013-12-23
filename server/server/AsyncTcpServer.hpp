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
		mSocket(ioService), 
		mAcceptor(ioService, tcp::endpoint(tcp::v4(), nPort))
	{
		DoAccept();
	};



private:
	int  DoAccept()
	{
		mAcceptor.async_accept(mSocket, [this](boost::system::error_code ec)
		{
			if (!ec)
			{
				AsyncTcpConnect_ptr session = std::make_shared<AsyncTcpConnection>(std::move(mSocket));

				mConnectionManager.begin(session);
				//call callback OnAccept(session);
			}

			DoAccept();
		});

		return 0;
	};

	void WriteAll(unsigned char* buffer, int len)
	{
		mConnectionManager.WriteAll(buffer, len);

		return;
	}

	bool Write(AsyncTcpConnect_ptr session, unsigned char* buffer, int len)
	{
		return mConnectionManager.Write(session, buffer, len_);
	}

	bool Recv(AsyncTcpConnect_ptr session, unsigned char* buffer, int len)
	{

		return true;
	}

	AsyncConnectionManager		mConnectionManager;


	tcp::acceptor	mAcceptor;
	tcp::socket		mSocket;
	
	//std::shared_ptr<boost::asio::io_service> self_service_;
	//boost::asio::io_service & ioService;

	boost::mutex listening_mutex_;

//	delegate  delegate_accept;
};
