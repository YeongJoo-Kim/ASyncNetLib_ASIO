#define BOOST_ASIO_HAS_MOVE
#include "AsyncTcpConnection.hpp"

using boost::asio::ip::tcp;
class AsyncConnectionManager : public std::enable_shared_from_this<AsyncConnectionManager>
{
public:
	void begin(AsyncTcpConnect_ptr session)
	{
		mConnections.insert(session);

		session->start();

		
	}

	void stop(AsyncTcpConnect_ptr session)
	{
		session->stop();

		mConnections.erase(session);
		
	}

	void stop_all()
	{
		// 모든 connection::stop를 수행함
		std::for_each(mConnections.begin(), mConnections.end(), [this](AsyncTcpConnect_ptr connection)
			{
				connection->stop();
			}
		);

		mConnections.clear();
	}
	
	void WriteAll(unsigned char* buffer, unsigned int len)
	{
		std::for_each(mConnections.begin(), mConnections.end(), [this](AsyncTcpConnect_ptr connection)
		{
			//connection->Write(buffer, len);
		}
		);

	}
private:

	std::set<AsyncTcpConnect_ptr> mConnections;
};
