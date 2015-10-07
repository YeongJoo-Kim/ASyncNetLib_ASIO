#pragma once
#include <iostream>

#include <boost/asio.hpp>
#include <boost/array.hpp>


using boost::asio::ip::tcp;
typedef std::shared_ptr<class AsyncTcpSessionInterface> AsyncTcpSessionInterface_ptr;

class AsyncTcpSessionInterface : public std::enable_shared_from_this<AsyncTcpSessionInterface>
{

public:
	static AsyncTcpSessionInterface_ptr	create(boost::asio::io_service& io) { return AsyncTcpSessionInterface_ptr(new AsyncTcpSessionInterface(io))->shared_from_this();}

	~AsyncTcpSessionInterface()
	{
		std::cout << "destroy AsyncTcpSessionInterface" << std::endl;
	};

	
	std::function<void(AsyncTcpSessionInterface_ptr session)>	delegate_conection_reset_by_peer;

	virtual void on_read() {

	}

	


	void start()
	{
		do_read();
	}

	void stop()
	{
		mSocket.close();
	}

	boost::asio::ip::tcp::socket& Socket() { return mSocket; };
protected:
	AsyncTcpSessionInterface(boost::asio::io_service& io)
		:mStrand(io), mSocket(io)
	{
		delegate_conection_reset_by_peer = nullptr;

		boost::system::error_code ec;
		tcp::endpoint endpoint = mSocket.remote_endpoint(ec);
		if (!ec)
		{
			mRemoteAddress = endpoint.address().to_string(ec);
			mRemotePortNum = endpoint.port();
		}
	};


	void do_read()
	{
		auto self(shared_from_this());
		memset(data_, 0x00, max_length);

		mSocket.async_read_some(
			boost::asio::buffer(data_),
			mStrand.wrap(boost::bind(
				&AsyncTcpSessionInterface::__callbakc_recv,
				self, 
				boost::asio::placeholders::error, 
				boost::asio::placeholders::bytes_transferred)));
	}


	void __callbakc_recv(const boost::system::error_code& ec, size_t bytes_transferred)
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
			on_read();

			do_read();
		}
	}

	bool do_write(unsigned char* buffer, std::size_t length)
	{
		auto self(shared_from_this());
#if 0
		boost::asio::async_write(mSocket, boost::asio::buffer(buffer, length), [this, self](boost::system::error_code ec, std::size_t /*length*/)
		{
			if (!ec)
			{
				return false;
			}

		});
#endif
		return true;
	}


	//AsyncConnectionManager& mConnectionManager;
	std::string		mRemoteAddress;
	uint16_t		mRemotePortNum;
	boost::asio::ip::tcp::socket mSocket;
	boost::asio::io_service::strand	mStrand;

	enum { max_length = 1024 };
	unsigned char data_[max_length];
	
	//boost::asio::streambuf data_;
	std::array<unsigned char, 8192> _buffer;
};

