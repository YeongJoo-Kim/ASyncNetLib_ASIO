#include "ASyncTcpSession.h"

ASyncTcpSession::ASyncTcpSession(boost::asio::io_service& io)
	:AsyncTcpSessionInterface(io)
{
	std::cout << "constructor of ASyncTcpSession" << endl;
	
}

ASyncTcpSession::~ASyncTcpSession() {
	std::cout << "destructor of ASyncTcpSession" << endl;

}
void ASyncTcpSession::on_read_complete(size_t bytes_transferred) {

	uint8_t* data_ = new uint8_t[bytes_transferred + 1]();

	data_[bytes_transferred] = 0;

	std::istream stream(&buffer_receive);
	stream.read((char*)data_, bytes_transferred);

	write(data_, bytes_transferred);
}

void ASyncTcpSession::on_write_complete(size_t bytes_transferred) {
	//std::cout << "call on_write_complete(" << bytes_transferred << ")" << std::endl;

	//mQueue.pop_front();
}
