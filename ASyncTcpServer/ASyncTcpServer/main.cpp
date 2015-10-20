// SyncServerSvr.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//
#include <SDKDDKVer.h>
#include "stdafx.h"
#include <iostream>
#include "ASyncTcpServer/ASyncTcpServer.h"

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/convenience.hpp>


// 콘솔 컨트롤 핸들러가 호출할 함수
boost::function0<void> console_ctrl_function;
BOOL WINAPI console_ctrl_handler(DWORD ctrl_type)
{
	switch (ctrl_type)
	{
	case CTRL_C_EVENT:
	case CTRL_BREAK_EVENT:
	case CTRL_CLOSE_EVENT:
	case CTRL_SHUTDOWN_EVENT:	
		// 콘솔에서 다음 컨트롤 타입 이벤트가 생기면 호출된다.
		console_ctrl_function();
		return TRUE;
	default:
		return FALSE;
	}
}

int main(int argc, char* argv[])
{

	std::shared_ptr<boost::asio::io_service>	mIo = std::shared_ptr<boost::asio::io_service>(new boost::asio::io_service());
	std::shared_ptr<void>						mIoWork = std::shared_ptr<boost::asio::io_service::work>(new boost::asio::io_service::work(*mIo));
	std::vector<boost::thread*>					mThreads;

	
	
	ASyncTcpServer server(*mIo, 20000);

	try
	{
#if 0
		if (argc != 2)
		{
			std::cerr << "Usage: async_tcp_echo_server <port>\n";
			return 1;
		}
		server.Begin(atoi(argv[1]));
#else
		server.Begin();
#endif




	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}

	//boost::thread thread_1(boost::bind(&boost::asio::io_service::run, mIo));
	for (int i = 0; i < 5; i++)
	{
		boost::thread* t = new boost::thread(boost::bind(&boost::asio::io_service::run, mIo));

		mThreads.push_back(t);
	}

	char ch = 0;
	while (getchar() != 'q')
	{
		//ch = getchar();

		switch (ch)
		{
		case 'a':
			//Server
			break;
		default:
			break;
		}

	}

	mIo->stop();

	for (boost::thread* t : mThreads) t->join();

	return 0;
}



#if 0
// SyncServerSvr.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//
#include <SDKDDKVer.h>
#include "stdafx.h"
#include <iostream>
#include "SyncServer/SyncServer.h"

int main()
{
	std::shared_ptr<boost::asio::io_service>	mIo = std::shared_ptr<boost::asio::io_service>(new boost::asio::io_service());
	std::shared_ptr<void>						mIoWork = std::shared_ptr<boost::asio::io_service::work>(new boost::asio::io_service::work(*mIo));

	SyncServer sync(*mIo, 20000);

	sync.start();


	//cout << "press any key..." << endl;
	//getchar();
	mIo->run();

	mIo->stop();


    return 0;
}
#endif

