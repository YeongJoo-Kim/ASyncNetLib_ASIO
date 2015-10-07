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
void ASyncTcpSession::on_read_complete(unsigned char* buffer, size_t bytes_transferred)
{
	//std::cout << "on_read_complete : " << buffer << "(" << bytes_transferred << ")" <<std::endl;
	//std::cout << "on_read_complete : " << "(" << bytes_transferred << ")" << std::endl;
	do_write(false, buffer, bytes_transferred);
}

void ASyncTcpSession::on_write_complete(size_t bytes_transferred)
{
	//std::cout << "call on_write_complete(" << bytes_transferred << ")" << std::endl;
}