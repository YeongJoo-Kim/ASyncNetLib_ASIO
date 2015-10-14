#pragma once
#include <string>
#include <iostream>
#include <memory>
#include <thread>
#include <condition_variable>
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
	virtual void on_read_complete(unsigned char* buffer, size_t bytes_transferred);
	virtual void on_write_complete(size_t bytes_transferred);

	
protected:
	void worker_thread();
	bool run;
	std::condition_variable cv;
	std::thread	mWorker;
};

