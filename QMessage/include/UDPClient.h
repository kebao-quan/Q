#pragma once

#include <WinSock2.h>
#include <iostream>
#include <ws2tcpip.h>
#include <tchar.h>
#include "oqs_cpp.h"
#include "cryptlib.h"
#include "rijndael.h"
#include "secblock.h"
#include "osrng.h"
#include "files.h"
#include "hex.h"
#include "filters.h"
#include "modes.h"
#include "oqs_cpp.h"
#include "util.h"
#include "UDPServer.h"


struct client_args
{
	std::string hostname;
	int port;
	bool* running;
};

BOOL start_client(std::string hostname, int port, bool* running);

unsigned WINAPI thread_start_client(void* args);


