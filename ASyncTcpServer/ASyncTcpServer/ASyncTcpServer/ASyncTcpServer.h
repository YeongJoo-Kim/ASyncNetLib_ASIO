#pragma once
#include <string>
#include <iostream>
#include <memory>
#include <boost/thread.hpp>
#include "../ASyncNetLib_ASIO/NetLib/AsyncTcpServerInterface.hpp"
#include "../ASyncNetLib_ASIO/Utils/Log.hpp"

#include "ASyncTcpSession.h"

using namespace std;

class ASyncTcpServer : public AsyncTcpServerInterface
{
public:
	ASyncTcpServer(boost::asio::io_service& io_service, unsigned short nPort);
	~ASyncTcpServer(void);

	bool Begin();
	void onConnectionResetByPeer(AsyncTcpSessionInterface_ptr session);
	
	void WriteAll()
	{
	}

	virtual AsyncTcpSessionInterface_ptr create_session();
	virtual void on_accept(AsyncTcpSessionInterface_ptr session);
	virtual void on_accept_error(AsyncTcpSessionInterface_ptr session, boost::system::error_code const &ec);

protected:
	short						mPort;
};

