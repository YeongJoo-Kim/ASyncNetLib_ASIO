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

void ASyncTcpSession::__callbakc_recv(const boost::system::error_code& ec, size_t bytes_transferred)
{
	if (ec)
	{
		if (ec == boost::asio::error::eof)
		{
			if (delegate_conection_reset_by_peer != nullptr) {
				delegate_conection_reset_by_peer(shared_from_this());
			}
		}
		else if (ec == boost::asio::error::operation_aborted)
		{
			/*	if (mErrorEventHandler != nullptr) {
			mErrorEventHandler(err.message(), bytesTransferred);
			}
			*/
		}
		else
		{

		}

		//error callback...
	}
	else
	{
		/*
		if (mReadEventHandler != nullptr) {

		//std::cout << bytesTransferred << endl;
		char* data = new char[bytesTransferred + 1]();
		data[bytesTransferred] = 0;
		mResponse.commit(bytesTransferred);
		istream stream(&mResponse);
		stream.read(data, bytesTransferred);
		mReadEventHandler(Buffer::create(data, bytesTransferred), this);
		delete[] data;
		}
		if (mReadCompleteEventHandler != nullptr &&
		mBufferSize > 0 && bytesTransferred < mBufferSize) {
		mReadCompleteEventHandler();
		}
		*/
		std::cout << data_;
		do_read();
	}
}