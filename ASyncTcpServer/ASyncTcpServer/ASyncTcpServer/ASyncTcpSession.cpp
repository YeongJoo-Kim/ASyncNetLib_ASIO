#include "ASyncTcpSession.h"

ASyncTcpSession::ASyncTcpSession(boost::asio::io_service& io)
	:AsyncTcpSessionInterface(io)
{
	std::cout << "constructor of ASyncTcpSession" << endl;
	run = true;
	//mWorker = std::thread(&ASyncTcpSession::worker_thread, this);
}

ASyncTcpSession::~ASyncTcpSession()
{
	std::cout << "destructor of ASyncTcpSession" << endl;
	run = false;
//	mWorker.join();
}
void ASyncTcpSession::on_read_complete(unsigned char* buffer, size_t bytes_transferred)
{
	//std::cout << "on_read_complete : " << buffer << "(" << bytes_transferred << ")" <<std::endl;
	//std::cout << "on_read_complete : " << "(" << bytes_transferred << ")" << std::endl;
	do_write(buffer, bytes_transferred);
}

void ASyncTcpSession::on_write_complete(size_t bytes_transferred)
{
	//std::cout << "call on_write_complete(" << bytes_transferred << ")" << std::endl;
}

void ASyncTcpSession::worker_thread()
{
	std::cout << "start thread" << std::endl;
	while (run)
	{
		std::cout << "loop thread" << std::endl;

	}

}