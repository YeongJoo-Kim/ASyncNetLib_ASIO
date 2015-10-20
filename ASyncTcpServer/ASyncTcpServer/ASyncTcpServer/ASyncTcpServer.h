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
	void onAccept(AsyncTcpSessionInterface_ptr session);
	void onConnectionResetByPeer(AsyncTcpSessionInterface_ptr session);
	
	void WriteAll()
	{
	}

	virtual AsyncTcpSessionInterface_ptr create_session();

protected:
	short						mPort;
};

