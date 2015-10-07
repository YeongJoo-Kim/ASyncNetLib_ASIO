#include "ASyncTcpSession.h"

ASyncTcpSession::ASyncTcpSession(boost::asio::io_service& io)
	:AsyncTcpSessionInterface(io)
{
	std::cout << "constructor of ASyncTcpSession" << endl;
}

ASyncTcpSession::~ASyncTcpSession()
{
	std::cout << "destructor of ASyncTcpSession" << endl;
}
void ASyncTcpSession::on_read()
{
	std::cout << "call on_read" << std::endl;
}