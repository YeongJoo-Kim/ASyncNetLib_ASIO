#pragma once
#include <iostream>
#include <deque>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include "../Utils/shared_const_buffer.hpp"



using boost::asio::ip::tcp;
typedef std::shared_ptr<class AsyncTcpSessionInterface> AsyncTcpSessionInterface_ptr;

class AsyncTcpSessionInterface : public std::enable_shared_from_this<AsyncTcpSessionInterface>
{

public:
	static AsyncTcpSessionInterface_ptr	create(boost::asio::io_service& io) { return AsyncTcpSessionInterface_ptr(new AsyncTcpSessionInterface(io))->shared_from_this();}

	~AsyncTcpSessionInterface()	{};

	
	std::function<void(AsyncTcpSessionInterface_ptr session)>	delegate_conection_reset_by_peer;

	virtual void on_read_complete(unsigned char* buffer, size_t bytes_transferred) {

	}

	virtual void on_write_complete(size_t bytes_transferred) {
	
	}

	


	void start() {
		do_read();
	}

	void stop() {
		_socket.close();

		while (write_queue.empty() == false)
		{
			write_queue.pop_front();
		}
	}

	boost::asio::ip::tcp::socket& Socket() { return _socket; };
protected:
	AsyncTcpSessionInterface(boost::asio::io_service& io)
		:mStrand(io), _socket(io),
		delegate_conection_reset_by_peer(nullptr)
	{
		boost::system::error_code ec;
		tcp::endpoint endpoint = _socket.remote_endpoint(ec);
		if (!ec)
		{
			remote_address = endpoint.address().to_string(ec);
			remote_port_number = endpoint.port();
		}
	};


	void do_read()
	{
		auto self(shared_from_this());
		memset(data_, 0x00, max_length);

		_socket.async_read_some(
			boost::asio::buffer(data_),
			mStrand.wrap(boost::bind(
				&AsyncTcpSessionInterface::__handler_recv,
				self, 
				boost::asio::placeholders::error, 
				boost::asio::placeholders::bytes_transferred)));
	}

	

	void __handler_recv(const boost::system::error_code& ec, size_t bytes_transferred)
	{
		if (ec)
		{
			std::cout << ec.message() << std::endl;
			if (ec == boost::asio::error::eof)
			{
				std::cout << "boost::asio::error::eof" << std::endl;
				if (delegate_conection_reset_by_peer != nullptr) {
					delegate_conection_reset_by_peer(shared_from_this());
				}
			}
			else if (ec == boost::asio::error::connection_reset)
			{
				std::cout << "boost::asio::error::connection_reset" << std::endl;
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
			
			//
			
			on_read_complete(data_, bytes_transferred);
			do_read();
		}
	}
#if 0
	void do_write(uint8_t* buffer, std::size_t length, const bool bImmediately = false)
	{
		auto self(shared_from_this());
		
		uint8_t* pSendData = nullptr;
		if (bImmediately == false)
		{
			pSendData = new uint8_t[length];
			memcpy(pSendData, buffer, length);

			__buffer.push_back(pSendData);
		}
		else
		{
			pSendData = buffer;
		}



		if (bImmediately == false && __buffer.size() > 1)
		{
			return;
		}
		
#if 0
		boost::asio::async_write(mSocket, boost::asio::buffer(buffer, length), 
			mStrand.wrap([this, self](boost::system::error_code ec, std::size_t /*length*/)
				{
					if (!ec)
					{
						return false;
					}
				}));
#endif
		boost::asio::async_write(_socket, boost::asio::buffer(pSendData, length),
			mStrand.wrap(boost::bind(
				&AsyncTcpSessionInterface::__handler_write, 
				self,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred)));

		return;
	}
#else
	void do_write(uint8_t* buffer, std::size_t length, const bool bImmediately = false)
	{
		shared_const_buffer _buffer(buffer, length);
		bool writeInProgress = !write_queue.empty();
		write_queue.push_back(_buffer);

		if (!writeInProgress)
		{
			do_write(_buffer);
		}

		return;
	}


	void __handler_write(const boost::system::error_code& ec, size_t bytes_transferred)
	{
		if (ec)
		{
			if (ec == boost::asio::error::eof || ec == boost::asio::error::connection_reset)
			{
				if (delegate_conection_reset_by_peer != nullptr) {
					delegate_conection_reset_by_peer(shared_from_this());
				}
			}
			else if (ec == boost::asio::error::operation_aborted)
			{
				/*
				if (mErrorEventHandler != nullptr) {
				mErrorEventHandler(err.message(), bytesTransferred);
				}
				*/
			}
			else
			{

			}


		}
		else
		{

			write_queue.pop_front();

			on_write_complete(bytes_transferred);

			if (!write_queue.empty())
			{
				//std::cout << "queue count " << write_queue.size() << std::endl;
				do_write(write_queue.front());
			}

		}
	}
#endif


	//AsyncConnectionManager& mConnectionManager;
	std::string		remote_address;
	uint16_t		remote_port_number;
	boost::asio::ip::tcp::socket _socket;
	boost::asio::io_service::strand	mStrand;

	enum { max_length = 4096 };
	uint8_t data_[max_length];
	
	//boost::asio::streambuf data_;
	std::array<uint8_t, 8192> _buffer;
	std::deque<shared_const_buffer> write_queue;
	std::deque < std::shared_ptr<boost::asio::const_buffer>> ___buffer;

	boost::asio::streambuf		buffer_write;
	boost::asio::streambuf		buffer_receive;

private:

	void do_write(shared_const_buffer &_buffer)
	{
		auto self(shared_from_this());

		boost::asio::async_write(_socket, _buffer,
			mStrand.wrap(boost::bind(
				&AsyncTcpSessionInterface::__handler_write,
				self,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred)));

		return;

	}
};

