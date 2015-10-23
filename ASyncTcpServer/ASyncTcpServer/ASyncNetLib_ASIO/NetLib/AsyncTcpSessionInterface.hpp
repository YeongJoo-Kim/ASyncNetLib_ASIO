#pragma once
#include <iostream>
#include <deque>
#include <boost/asio.hpp>
#include <boost/array.hpp>

#include "../Utils/debug.hpp"
#include "../Utils/shared_const_buffer.hpp"



using boost::asio::ip::tcp;
typedef std::shared_ptr<class AsyncTcpSessionInterface> AsyncTcpSessionInterface_ptr;

class AsyncTcpSessionInterface : public std::enable_shared_from_this<AsyncTcpSessionInterface>
{
	enum { max_length = 4096 };

public:
	static AsyncTcpSessionInterface_ptr	create(boost::asio::io_service& io) { return AsyncTcpSessionInterface_ptr(new AsyncTcpSessionInterface(io))->shared_from_this();}
	~AsyncTcpSessionInterface()	{
	}
	
	std::function<void(AsyncTcpSessionInterface_ptr session)>	delegate_conection_reset_by_peer;

	/*
	virtual void on_read_complete(unsigned char* buffer, size_t bytes_transferred) {
	}
	*/
	virtual void on_read_complete(size_t bytes_transferred) {

	}

	virtual void on_write_complete(size_t bytes_transferred) {	
	}

	
	void start() {

		remote_address = _socket.remote_endpoint().address().to_string();

		do_read(max_length);
	}

	void stop() {
		_socket.close();

		while (write_queue.empty() == false) {
			write_queue.pop_front();
		}
	}

	boost::asio::ip::tcp::socket& Socket() { 
		return _socket; 
	};

	std::string get_remote_address()
	{
		return remote_address;
	}

protected:
	AsyncTcpSessionInterface(boost::asio::io_service& io)
		:strand_recv(io), strand_write(io), _socket(io),
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

	void do_read(size_t buffer_length) {
		_socket.async_read_some(
			buffer_receive.prepare(buffer_length),
			strand_recv.wrap(boost::bind(
				&AsyncTcpSessionInterface::__handler_recv,
				shared_from_this(),
				boost::asio::placeholders::error, 
				boost::asio::placeholders::bytes_transferred)));
	}

	void __handler_recv(const boost::system::error_code& ec, size_t bytes_transferred)
	{
		if (ec)
		{
			
			if (ec == boost::asio::error::eof)
			{
				NETWORK_MESSAGE("boost::asio::error::eof");
				if (delegate_conection_reset_by_peer != nullptr) {
					delegate_conection_reset_by_peer(shared_from_this());
				}
			}
			else if (ec == boost::asio::error::connection_reset)
			{
				NETWORK_MESSAGE("boost::asio::error::connection_reset");
				if (delegate_conection_reset_by_peer != nullptr) {
					delegate_conection_reset_by_peer(shared_from_this());
				}
			}
			else if (ec == boost::asio::error::operation_aborted)
			{

				NETWORK_MESSAGE("boost::asio::error::operation_aborted");
				/*	if (mErrorEventHandler != nullptr) {
				mErrorEventHandler(err.message(), bytesTransferred);
				}
				*/
			}
			else
			{
				NETWORK_MESSAGE("Error Value : " << ec.value());
			}

			NETWORK_MESSAGE(" - " << ec.message());
			std::cout << std::endl;
			//error callback...
		}
		else
		{
			buffer_receive.commit(bytes_transferred);
/*
			uint8_t* data = new uint8_t[bytes_transferred + 1]();
			data[bytes_transferred] = 0;

			std::istream stream(&buffer_receive);
			stream.read((char*)data, bytes_transferred);

			on_read_complete(data, bytes_transferred);

			delete[] data;
*/
			on_read_complete(bytes_transferred);
			do_read(max_length);
		}
	}

	void write(uint8_t* buffer, std::size_t length) {
		
		std::unique_lock<std::mutex> lock(mutex_write);
		bool writeInProgress = !write_queue.empty();

		write_queue.push_back(shared_const_buffer(buffer, length));

		if (!writeInProgress) {
			do_write();
		}


	}

	void do_write() {

		boost::asio::async_write(_socket, write_queue.front(),
			strand_write.wrap(boost::bind(
				&AsyncTcpSessionInterface::__handler_write,
				shared_from_this(),
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred)));
	}
	/*
	void do_write(uint8_t* buffer, std::size_t length) {
		
		std::unique_lock<std::mutex> lock(mutex_write);
		

		bool writeInProgress = buffer_write.size() > 0;

		std::ostream stream(&buffer_write);
		if (buffer && length > 0) {
			stream.write((const char*)buffer, length);
		}
		
		if (!writeInProgress)
		{
			boost::asio::async_write(_socket, buffer_write,
				strand_write.wrap(boost::bind(
					&AsyncTcpSessionInterface::__handler_write,
					shared_from_this(),
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred)));

			buffer_write.consume(buffer_write.size());
		}
		
		return;
	}
	*/
	void __handler_write(const boost::system::error_code& ec, size_t bytes_transferred)
	{
		if (ec)
		{
			if (ec == boost::asio::error::eof )
			{
				NETWORK_MESSAGE("boost::asio::error::eof");
				if (delegate_conection_reset_by_peer != nullptr) {
					delegate_conection_reset_by_peer(shared_from_this());
				}
			}
			else if (ec == boost::asio::error::connection_reset)
			{
				NETWORK_MESSAGE("boost::asio::error::connection_reset");
				if (delegate_conection_reset_by_peer != nullptr) {
					delegate_conection_reset_by_peer(shared_from_this());
				}
			}
			else if (ec == boost::asio::error::operation_aborted)
			{
				NETWORK_MESSAGE("boost::asio::error::operation_aborted");
				/*
				if (mErrorEventHandler != nullptr) {
				mErrorEventHandler(err.message(), bytesTransferred);
				}
				*/
			}
			else
			{
				NETWORK_MESSAGE("Error Value : " << ec.value());
			}
			NETWORK_MESSAGE(" - " << ec.message());
		}
		else
		{
			/*
			std::unique_lock<std::mutex> lock(mutex_write);

			if (buffer_write.size() > 0)
			{
				std::cout << "oops ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^!!!!!!!!! " << std::endl;
				do_write(nullptr, 0);
			}
			*/

			std::unique_lock<std::mutex> lock(mutex_write);
			write_queue.pop_front();
			if (!write_queue.empty()) {
				do_write();
			}
		}
	}


protected :

	boost::asio::streambuf		buffer_write;
	boost::asio::streambuf		buffer_receive;
	std::mutex					mutex_write;

	std::string					remote_address;
	uint16_t					remote_port_number;
	boost::asio::ip::tcp::socket _socket;
	boost::asio::io_service::strand	strand_recv;
	boost::asio::io_service::strand	strand_write;
	std::deque<shared_const_buffer> write_queue;

	void __error_handler(const std::string function, boost::system::error_code& ec)
	{
		std::cerr << "[" << function << "] : ";
		if (ec == boost::asio::error::eof)
		{
			std::cerr << "[__handler_write] boost::asio::error::eof" << std::endl;
			if (delegate_conection_reset_by_peer != nullptr) {
				delegate_conection_reset_by_peer(shared_from_this());
			}
		}
		else if (ec == boost::asio::error::connection_reset)
		{
			std::cerr << "boost::asio::error::connection_reset" << std::endl;
			if (delegate_conection_reset_by_peer != nullptr) {
				delegate_conection_reset_by_peer(shared_from_this());
			}
		}
		else if (ec == boost::asio::error::operation_aborted)
		{
			std::cerr << "boost::asio::error::operation_aborted" << std::endl;
		}
		else
		{
			std::cerr << "Error Value : " << ec.value() << std::endl;
		}
		std::cerr << " - " << ec.message() << std::endl;
	}


	
};

