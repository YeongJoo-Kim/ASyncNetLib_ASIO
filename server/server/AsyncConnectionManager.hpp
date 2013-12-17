#define BOOST_ASIO_HAS_MOVE
#include "AsyncConnection.hpp"

using boost::asio::ip::tcp;
class AsyncConnectionManager : public std::enable_shared_from_this<AsyncConnectionManager>
{
public:
	void start(AsyncConnect_ptr session)
	{
		mConnections.insert(session);

		session->start();

		
	}

	void stop(AsyncConnect_ptr session)
	{
		mConnections.erase(session);
		session->stop();
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
