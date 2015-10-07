#pragma once
#define BOOST_NETWORK_DEBUG

#include <SDKDDKVer.h>

#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <set>
#include <mutex>
#include <boost/asio.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/function.hpp>


#include "../Utils/debug.hpp"
#include "AsyncConnectionManager.hpp"
using boost::asio::ip::tcp;

typedef std::shared_ptr<class AsyncTcpServerInterface>			AsyncTcpServerInterface_ptr;
typedef std::unique_lock<std::mutex> scoped_mutex_lock;
class AsyncTcpServerInterface : public std::enable_shared_from_this<AsyncTcpServerInterface>
{
public:
	/*
	static AsyncTcpServerInterface_ptr	create(boost::asio::io_service& io, unsigned short nPort)
	{
		return AsyncTcpServerInterface_ptr(new AsyncTcpServerInterface(io, nPort))->shared_from_this();
	};
	*/

	~AsyncTcpServerInterface()
	{
		delegate_accept = nullptr;
		delegate_receive = nullptr;
		delegate_connection_reset_by_peer = nullptr;
	};

	//delegate functions 
	std::function<void (AsyncTcpSessionInterface_ptr session)>	delegate_accept;
	std::function<void(AsyncTcpSessionInterface_ptr session)>	delegate_connection_reset_by_peer;
	std::function<void(AsyncTcpSessionInterface_ptr session)>	delegate_receive;

	void run()
	{
		listen();
		//service_.run();
	}

	void stop()
	{
		scoped_mutex_lock listening_lock(listening_mutex_);
		if (listening) 
		{  
			scoped_mutex_lock stopping_lock(stopping_mutex_);
			stopping = true;
			boost::system::error_code ignored;
			mAcceptor.close(ignored);
			listening = false;
			mIoService.post(boost::bind(&AsyncTcpServerInterface::handle_stop, this));
		}
	}
	

protected:
	AsyncTcpServerInterface(boost::asio::io_service& ioService, unsigned short nPort) :
		mIoService(ioService), mSocket(ioService),
		mAcceptor(ioService, tcp::endpoint(tcp::v4(), nPort)),
		listening_mutex_(),
		stopping_mutex_(),
		listening(false),
		stopping(false),
		port_(nPort),
		address_("localhost")
	{};

	void listen() {
		scoped_mutex_lock listening_lock(listening_mutex_);
		BOOST_NETWORK_MESSAGE("Listening on " << address_ << ':' << port_);
		if (!listening)
			start_listening();  // we only initialize our acceptor/sockets if we
								// arent already listening

			//do_accept();
		if (!listening) {
			BOOST_NETWORK_MESSAGE("Error listening on " << address_ << ':' << port_);
			boost::throw_exception(
				std::runtime_error("Error listening on provided port."));
		}
	}

	virtual AsyncTcpSessionInterface_ptr create_session()
	{
		return AsyncTcpSessionInterface::create(mIoService);
	}

	void start_listening() {
		/*
		using boost::asio::ip::tcp;
		boost::system::error_code error;
		mIoService.reset();  // this allows repeated cycles of run -> stop ->
						   // run
		tcp::resolver resolver(mIoService);
		tcp::resolver::query query(address_, port_);
		tcp::resolver::iterator endpoint_iterator = resolver.resolve(query, error);
		if (error) {
			BOOST_NETWORK_MESSAGE("Error resolving '" << address_ << ':' << port_);
			return;
		}
		tcp::endpoint endpoint = *endpoint_iterator;
		mAcceptor.open(endpoint.protocol(), error);
		if (error) {
			BOOST_NETWORK_MESSAGE("Error opening socket: " << address_ << ":"
				<< port_);
			return;
		}
		socket_options_base::acceptor_options(mAcceptor);
		mAcceptor.bind(endpoint, error);
		if (error) {
			BOOST_NETWORK_MESSAGE("Error binding socket: " << address_ << ":"
				<< port_);
			return;
		}
		mAcceptor.listen(boost::asio::socket_base::max_connections, error);
		if (error) {
			BOOST_NETWORK_MESSAGE("Error listening on socket: '"
				<< error << "' on " << address_ << ":" << port_);
			return;
		}
		new_connection.reset(new connection(mIoService, handler, *thread_pool, ctx_));
		*/
		AsyncTcpSessionInterface_ptr session = create_session();

		mAcceptor.async_accept(
			session->Socket(),
			boost::bind(&AsyncTcpServerInterface::do_accept, this, session, boost::asio::placeholders::error));

		listening = true;
		scoped_mutex_lock stopping_lock(stopping_mutex_);
		stopping = false;  // if we were in the process of stopping, we revoke
						   // that command and continue listening
		BOOST_NETWORK_MESSAGE("Now listening on socket: '" << address_ << ":" << port_ << "'");
	}

	

	void  do_accept(AsyncTcpSessionInterface_ptr session, boost::system::error_code const &ec)
	{
		scoped_mutex_lock stopping_lock(stopping_mutex_);
		if (stopping)
			return; 
	
		if (ec) {
			BOOST_NETWORK_MESSAGE("Error accepting connection, reason: " << ec);
		}

		if (ec) {
			/*if (mErrorEventHandler != nullptr) {
				mErrorEventHandler(err.message(), 0);
			}
			*/
		}
		else {


			session->delegate_conection_reset_by_peer = std::move(std::bind(&AsyncTcpServerInterface::on_connection_reset_by_peer, this, std::placeholders::_1));

			mConnectionManager.begin(session);
			/*
			if (mAcceptEventHandler != nullptr) {
				mAcceptEventHandler(session);
			}
			*/

			if (delegate_accept != nullptr) {
				delegate_accept(session);
			}
		}

		
		AsyncTcpSessionInterface_ptr new_session = create_session();

		mAcceptor.async_accept(
			new_session->Socket(),
			boost::bind(&AsyncTcpServerInterface::do_accept, this, new_session, boost::asio::placeholders::error));

		/*
		mAcceptor.async_accept(session->Socket(), [this, session](boost::system::error_code ec)
		{
			if (!ec)
			{
				//AsyncTcpSessionInterface_ptr session = std::make_shared<AsyncTcpSessionInterface>(std::move(mSocket));
				//AsyncTcpSessionInterface_ptr session = AsyncTcpSessionInterface::create(std::move(mSocket));
				session->delegate_conection_reset_by_peer = std::move(std::bind(&AsyncTcpServerInterface::on_connection_reset_by_peer, this, std::placeholders::_1));

				mConnectionManager.begin(session);
				if (delegate_accept != nullptr) {
					delegate_accept(session);
				}

				//must call do_accept
			}

		});
		*/
		

		return;
	};

	void handle_stop() {
		scoped_mutex_lock stopping_lock(stopping_mutex_);
		if (stopping)
			mIoService.stop();  // a user may have started listening again before
							  // the stop command is reached
	}

	//connection reset by peer callback
	void on_connection_reset_by_peer(AsyncTcpSessionInterface_ptr session)
	{
		if (delegate_connection_reset_by_peer)
		{
			delegate_connection_reset_by_peer(session);
		}
	}

	void WriteAll(unsigned char* buffer, int len)
	{
		mConnectionManager.WriteAll(buffer, len);

		return;
	}

	bool Write(AsyncTcpSessionInterface_ptr session, unsigned char* buffer, int len)
	{
		return mConnectionManager.Write(session, buffer, len);
	}

	bool Recv(AsyncTcpSessionInterface_ptr session, unsigned char* buffer, int len)
	{

		return true;
	}

	boost::asio::io_service&	mIoService;
	AsyncConnectionManager		mConnectionManager;
	tcp::acceptor	mAcceptor;
	tcp::socket		mSocket;

	std::mutex listening_mutex_;
	std::mutex stopping_mutex_;
	bool listening;
	bool stopping;
	unsigned short	port_;
	std::string address_;

//	delegate  delegate_accept;
};
