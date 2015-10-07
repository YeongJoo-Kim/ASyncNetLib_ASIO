#include "ASyncTcpServer.h"
/*
int pos[100];
for (i = 0; i<100; i++) pos[i] = i;
for (i = 0; i<100; i++) {
	int a = random(100);
	int k = pos[i];
	pos[i] = pos[a];
	pos[a] = k;
}
*/
ASyncTcpServer::ASyncTcpServer(boost::asio::io_service& io_service, unsigned short nPort)
	: AsyncTcpServerInterface(io_service, nPort)
{
	
}


ASyncTcpServer::~ASyncTcpServer()
{
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
	SyncSession_ptr _session = std::dynamic_pointer_cast<ASyncTcpSession>(session);

	std::cout << "OnAccept!! " << endl;

	//do_accept();
};

AsyncTcpSessionInterface_ptr ASyncTcpServer::create_session()
{
	return std::make_shared<ASyncTcpSession>(mIoService);
} 

void ASyncTcpServer::onConnectionResetByPeer(AsyncTcpSessionInterface_ptr session)
{
	std::cout << "Connection Reset By Peer!! " << endl;

};
#if 0
void SyncServer::start()
{
	

	// Add callbacks to work with the server asynchronously.
	mServer->connectAcceptEventHandler(&SyncServer::onAccept, this);
	mServer->connectCancelEventHandler(&SyncServer::onCancel, this);
	mServer->connectErrorEventHandler(&SyncServer::onError, this);

	// Start listening.
	accept();

}


void SyncServer::onAccept(TcpSessionRef session)
{
	
	cout << "Connected"  << endl;
	// Get the session from the argument and set callbacks.
	// Note that you can use lambdas.
	/*
	mSession = session;
	mSession->connectCloseEventHandler([&]() {
		cout << "Session closed" << endl;
	});
	mSession->connectErrorEventHandler(&SyncServer::onError, this);
	mSession->connectReadCompleteEventHandler(&SyncServer::onReadComplete, this);
	mSession->connectReadEventHandler(&SyncServer::onRead, this);
	mSession->connectWriteEventHandler(&SyncServer::onWrite, this);

	// Start reading data from the client. 
	mSession->read();
	*/

	
	session->connectCloseEventHandler([&]() {
		cout << "Session closed" << endl;
	});
	session->connectErrorEventHandler(&SyncServer::onError, this);
	session->connectReadCompleteEventHandler(&SyncServer::onReadComplete, this);
	session->connectReadEventHandler(&SyncServer::onRead, this);
	session->connectWriteEventHandler(&SyncServer::onWrite, this);

	// Start reading data from the client. 
	session->read(8);
	accept();
}

void SyncServer::onReadComplete()
{
	cout << "Read complete" << endl;
	// Close the session.
	mSession->close();

	// Cancel any remaining IO operations (there shouldn't be any).
	// This will tell the server to start listening for the next 
	// connection. See the onCancel method.
	mServer->cancel();
}

void SyncServer::onError(string err, size_t bytesTransferred)
{
	string text = "Error";
	if (!err.empty()) {
		text += ": " + err;
	}

	cout << text << endl;
}

void SyncServer::onRead(ci::BufferRef buffer, SessionInterface* session)
{
//	cout << buffer->getSize() << "  bytes read" << endl;
	// Data is packaged as a ci::Buffer. This allows 
	// you to send any kind of data. Because it's more common to
	// work with strings, the session object has static convenience 
	// methods for converting between std::string and ci::Buffer.
	
	//string response = TcpSession::bufferToString(buffer);

//	cout << response << endl;
	// We're simply echoing what the client sent us. Write the response
	// back to them to acknowledge the succesful communication.
	//mSession->write(TcpSession::stringToBuffer(response));
	//((TcpSession*)session)->write(TcpSession::stringToBuffer(response));
	((TcpSession*)session)->write(buffer);


}

void SyncServer::onCancel()
{
	
	cout << "Canceled" << endl;
	// Automatically listen on cancel.
	accept();
}

void SyncServer::onWrite(size_t bytesTransferred, SessionInterface* session)
{
	

//	cout << bytesTransferred << " bytes written" << endl;

	// Read after writing to look for an EOF, or disconnect signals
	// from the client.
	//mSession->read();
	
}

void SyncServer::accept()
{
	if (mServer) {
		// This is how you start listening for a connection. Once
		// a connection occurs, a session will be created and passed
		// in through the onAccept method.
		mServer->accept(mPort);

		//mText.push_back("Listening on port: " + toString(mPort));

		cout << "Listening on port: " << mPort << endl;
	}
}
#endif