#pragma once

#define NETWORK_DEBUG
//#undef NETWORK_DEBUG
#ifdef NETWORK_DEBUG
#include <iostream>
#ifndef NETWORK_MESSAGE
//__FILE__, __LINE__
#define NETWORK_MESSAGE(msg)                   \
	std::cerr << "[" << __FUNCTION__ << "] : " \
			<< msg \
			<< std::endl;
#endif
#else
#ifndef NETWORK_MESSAGE
#define NETWORK_MESSAGE(msg)
#endif
#endif
