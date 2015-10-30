#pragma once
#include "AsyncTcpSessionInterface.hpp"

using boost::asio::ip::tcp;
class AsyncConnectionManager : public std::enable_shared_from_this<AsyncConnectionManager> {
public:
	void begin(AsyncTcpSessionInterface_ptr session) {
		connections.insert(session);
		session->start();
	}

	void stop(AsyncTcpSessionInterface_ptr session) {
		session->stop();

		connections.erase(session);
	}

	void stop_all() {
		// 모든 connection::stop를 수행함
		std::for_each(connections.begin(), connections.end(), [this](AsyncTcpSessionInterface_ptr connection)
			{
				connection->stop();
			}
		);
		connections.clear();
	}
	
	void write_all(uint8_t* buffer, size_t len) {
		/*
		std::for_each(mConnections.begin(), mConnections.end(), [this](AsyncTcpSessionInterface_ptr connection)
		{
			//connection->Write(buffer, len);
		}
		);
		*/

		std::for_each(connections.begin(), connections.end(), [=](AsyncTcpSessionInterface_ptr connection)
		{
			connection->do_write(buffer, len);
		}
		);

	}

	bool write(AsyncTcpSessionInterface_ptr session, uint8_t* buffer, int len) {

/*		AsyncTcpSessionInterface_ptr _session = connections[session];

		if(_session == mConnections.end()) return false;

		return _session->do_write(buffer, len);
		*/
		return true;
	}
protected:

	std::set<AsyncTcpSessionInterface_ptr> connections;
};