#define BOOST_ASIO_HAS_MOVE
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
/*
		auto self(shared_from_this());
		boost::asio::async_read(mSocket, boost::asio::buffer(read_msg_.data(), chat_message::header_length), [this, self](boost::system::error_code ec, std::size_t / *length* /)
		{
			if (!ec && read_msg_.decode_header())
			{
				do_read_body();
			}
			else
			{
				room_.leave(shared_from_this());
			}
		});*/
	}

	void do_read_body()
	{
/*
		auto self(shared_from_this());
		boost::asio::async_read(mSocket, boost::asio::buffer(read_msg_.body(), read_msg_.body_length()), [this, self](boost::system::error_code ec, std::size_t / *length* /)
		{
			if (!ec)
			{
				room_.deliver(read_msg_);
				do_read_header();
			}
			else
			{
				room_.leave(shared_from_this());
			}
		});*/
	}

/*	void deliver(const chat_message& msg)
	{

		bool write_in_progress = !write_msgs_.empty();
		write_msgs_.push_back(msg);
		if (!write_in_progress)
		{
			do_write();
		}
	}
*/
	//void write(const )

	void do_write()
	{
/*
		auto self(shared_from_this());
		boost::asio::async_write(mSocket, boost::asio::buffer(write_msgs_.front().data(), write_msgs_.front().length()), [this, self](boost::system::error_code ec, std::size_t / *length* /)
		{
			if (!ec)
			{
				write_msgs_.pop_front();
				if (!write_msgs_.empty())
				{
					do_write();
				}
			}
			else
			{
				//room_.leave(shared_from_this());
			}
		});*/
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