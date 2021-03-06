#pragma once
#include <string>
#include <iostream>
#include <memory>
#include <thread>
#include <condition_variable>
#include <boost/thread.hpp>
#include "../ASyncNetLib_ASIO/NetLib/AsyncTcpSessionInterface.hpp"

using namespace std;
typedef std::shared_ptr<class ASyncTcpSession> ASyncTcpSession_ptr;
class ASyncTcpSession : public AsyncTcpSessionInterface {
public:
	ASyncTcpSession(boost::asio::io_service& io);
	~ASyncTcpSession(void);
	virtual void on_read_complete(size_t bytes_transferred);
	virtual void on_write_complete(size_t bytes_transferred);

	
protected:

};

