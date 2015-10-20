#include "ASyncTcpServer.h"

ASyncTcpServer::ASyncTcpServer(boost::asio::io_service& io_service, unsigned short nPort)
	: AsyncTcpServerInterface(io_service, nPort)
{
	
}


ASyncTcpServer::~ASyncTcpServer()
{
	stop();
}

bool ASyncTcpServer::Begin()
{
	delegate_accept = std::move(std::bind(&ASyncTcpServer::onAccept, this, std::placeholders::_1));
	delegate_connection_reset_by_peer = std::move(std::bind(&ASyncTcpServer::onConnectionResetByPeer, this, std::placeholders::_1));

	run();

	return true;
}

void ASyncTcpServer::onAccept(AsyncTcpSessionInterface_ptr session)
{
	ASyncTcpSession_ptr _session = std::dynamic_pointer_cast<ASyncTcpSession>(session);

	std::cout << "OnAccept!! " << endl;
};

AsyncTcpSessionInterface_ptr ASyncTcpServer::create_session()
{
	return std::make_shared<ASyncTcpSession>(mIoService);
} 

void ASyncTcpServer::onConnectionResetByPeer(AsyncTcpSessionInterface_ptr session)
{
	std::cout << "[ASyncTcpServer] Connection Reset By Peer!! " << endl;

};