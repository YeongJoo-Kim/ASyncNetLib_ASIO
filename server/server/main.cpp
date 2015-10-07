#include "stdafx.h"
#include "Server.h"

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

 	Server server;
 
 	try
 	{
 		if (argc != 2)
 		{
 			std::cerr << "Usage: async_tcp_echo_server <port>\n";
 			return 1;
 		}
 
 		
 
 		server.Begin(atoi(argv[1]));
 	}
 	catch (std::exception& e)
 	{
 		std::cerr << "Exception: " << e.what() << "\n";
 	}



	SetConsoleCtrlHandler(console_ctrl_handler, TRUE);

	char ch = 0;
	while(getchar() != 'q')
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

	return 0;
}
