#pragma once
#include <string>
#include <iostream>
#include <memory>
#include <boost/thread.hpp>
#include "../ASyncNetLib_ASIO/NetLib/AsyncTcpSessionInterface.hpp"
#include "../ASyncNetLib_ASIO/Utils/Log.hpp"

using namespace std;
typedef std::shared_ptr<class ASyncTcpSession> SyncSession_ptr;
class ASyncTcpSession : public AsyncTcpSessionInterface
{
public:
	ASyncTcpSession(boost::asio::io_service& io);
	~ASyncTcpSession(void);
	virtual void on_read_complete();
	virtual void on_write_complete();
protected:
};

