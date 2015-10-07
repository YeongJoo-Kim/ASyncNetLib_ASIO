#pragma once
#include "AsyncTcpSessionInterface.hpp"

using boost::asio::ip::tcp;
class AsyncConnectionManager : public std::enable_shared_from_this<AsyncConnectionManager>
{
public:
	void begin(AsyncTcpSessionInterface_ptr session)
	{
		mConnections.insert(session);

		session->start();

		
	}

	void stop(AsyncTcpSessionInterface_ptr session)
	{
		session->stop();

		mConnections.erase(session);
		
	}

	void stop_all()
	{
		// 모든 connection::stop를 수행함
		std::for_each(mConnections.begin(), mConnections.end(), [this](AsyncTcpSessionInterface_ptr connection)
			{
				connection->stop();
			}
		);

		mConnections.clear();
	}
	
	void WriteAll(unsigned char* buffer, unsigned int len)
	{
		/*
		std::for_each(mConnections.begin(), mConnections.end(), [this](AsyncTcpSessionInterface_ptr connection)
		{
			//connection->Write(buffer, len);
		}
		);
		*/

	}

	bool Write(AsyncTcpSessionInterface_ptr session, unsigned char* buffer, int len)
	{

/*		AsyncTcpSessionInterface_ptr _session = mConnections[session];

		if(_session == mConnections.end()) return false;

		return _session->do_write(buffer, len);
		*/
		return true;
	}
private:

	std::set<AsyncTcpSessionInterface_ptr> mConnections;
};