#include "ASyncTcpSession.h"

ASyncTcpSession::ASyncTcpSession(boost::asio::io_service& io)
	:AsyncTcpSessionInterface(io)
{
	std::cout << "constructor of ASyncTcpSession" << endl;
	run = true;
	//mWorker = std::thread(&ASyncTcpSession::worker_thread, this);
}

ASyncTcpSession::~ASyncTcpSession()
{
	std::cout << "destructor of ASyncTcpSession" << endl;
	run = false;
	//mWaitObject.notify_one();
	

//	mWorker.join();
}
void ASyncTcpSession::on_read_complete(unsigned char* buffer, size_t bytes_transferred)
{
	//std::cout << "on_read_complete : " << buffer << "(" << bytes_transferred << ")" <<std::endl;
	//std::cout << "on_read_complete : " << "(" << bytes_transferred << ")" << std::endl;
	//do_write(buffer, bytes_transferred);
	/*
	{
		std::lock_guard<std::mutex> lock(mQueueMutex);
		mQueue.push_back(shared_const_buffer(buffer, bytes_transferred));
	}

	mWaitObject.notify_one();

	*/
	
	do_write(buffer, bytes_transferred);
}

void ASyncTcpSession::on_write_complete(size_t bytes_transferred)
{
	//std::cout << "call on_write_complete(" << bytes_transferred << ")" << std::endl;

	//mQueue.pop_front();
}

void ASyncTcpSession::worker_thread()
{
	std::cout << "start thread" << std::endl;
	while ( run )
	{
		std::unique_lock<std::mutex> lock(mWaitMutex);
		mWaitObject.wait(lock);
		//std::cout << "loop thread" << std::endl;

		//shared_const_buffer _buffer(mQueue.front().buffer_., mQueue.front().length());
		while (!mQueue.empty())
		{
			do_write(mQueue.front().data(), mQueue.front().length());
			{
				//std::cout << mQueue.size() << " " << mQueue.front().length() << endl;
				std::lock_guard<std::mutex> lock(mQueueMutex);
				mQueue.pop_front();
				
			}
		}
		/*
		//do_write(_buffer.)
		if (!mQueue.empty())
		{
			//

			mWaitObject.notify_one();

		}
		*/
		//std::this_thread::sleep_for(std::chrono::milliseconds(1)); 

	}

}