#define BOOST_ASIO_HAS_MOVE
#include "AsyncConnection.hpp"

using boost::asio::ip::tcp;
class AsyncConnectionManager : public std::enable_shared_from_this<AsyncConnectionManager>
{
	typedef boost::shared_ptr<AsyncConnection> AsyncConnect_ptr;

public:
	void start(AsyncConnect_ptr connection)
	{
		mConnections.insert(connection);

		connection->start();

		
	}

	void stop(AsyncConnect_ptr connection)
	{
		mConnections.erase(connection);
		connection->stop();
	}

	void stop_all()
	{
		// 모든 connection::stop를 수행함
		std::for_each(mConnections.begin(), mConnections.end(), [this](AsyncConnect_ptr connection)
			{
				connection->stop();
			}
		);

		mConnections.clear();
	}
	
private:

	std::set<AsyncConnect_ptr> mConnections;
};
