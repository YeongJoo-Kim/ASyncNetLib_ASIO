#define BOOST_ASIO_HAS_MOVE
#include <iostream>

#include <boost/asio.hpp>
#include <boost/array.hpp>


using boost::asio::ip::tcp;

class AsyncConnection : public std::enable_shared_from_this<AsyncConnection>
{
	

public:
	AsyncConnection(tcp::socket socket)
		: mSocket(std::move(socket))

	{
		boost::system::error_code ec;
		tcp::endpoint endpoint = mSocket.remote_endpoint(ec);
		if (!ec)
		{
			mRemoteAddress = endpoint.address().to_string(ec);
			mRemotePortNum = endpoint.port();
		}
	};

	void start()
	{

		do_read();
	}

	void stop()
	{
		mSocket.close();
	}


private:

	void do_read()
	{
		auto self(shared_from_this());

		memset(data_, 0x00, max_length);

		mSocket.async_read_some(boost::asio::buffer(data_, max_length), [this, self](boost::system::error_code ec, std::size_t length)
		{
			if (!ec)
			{
				std::cout << (char*)data_ << std::endl;

				do_read();
			}
		});
	}




	void do_write(unsigned char* buffer, std::size_t length)
	{
		auto self(shared_from_this());

		boost::asio::async_write(mSocket, boost::asio::buffer(buffer, length), [this, self](boost::system::error_code ec, std::size_t /*length*/)
		{
			if (!ec)
			{
				
			}
		});
	}


	//AsyncConnectionManager& mConnectionManager;
	std::string		mRemoteAddress;
	uint16_t		mRemotePortNum;
	boost::asio::ip::tcp::socket mSocket;

	enum { max_length = 1024 };
	unsigned char data_[max_length];
	
	boost::array<unsigned char, 8192> _buffer;
};

typedef std::shared_ptr<AsyncConnection> AsyncConnect_ptr;