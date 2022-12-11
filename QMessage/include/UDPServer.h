#pragma once

#include <WinSock2.h>
#include <iostream>
#include "oqs_cpp.h"
#include "util.h"
#include "encrypt.h"
#include "cryptlib.h"
#include "rijndael.h"
#include "secblock.h"
#include "osrng.h"
#include "files.h"
#include "hex.h"
#include "filters.h"
#include "modes.h"



BOOL initSockEnv();

typedef struct args
{
	SOCKET sockSrv;
	SOCKADDR_IN addrCli;
	CryptoPP::SecByteBlock key;
	CryptoPP::SecByteBlock iv;
}*Pargs;

unsigned WINAPI thread_recvfrom(void* arg);

class CChat
{
public:
	CryptoPP::SecByteBlock key;
	CryptoPP::SecByteBlock iv;
	SOCKET sockSrv;
	SOCKADDR_IN addrCli;
	std::string cipher, plain;
	args a;
	CChat(CryptoPP::SecByteBlock key, CryptoPP::SecByteBlock iv, SOCKET sockSrv, SOCKADDR_IN addrCli) : key(key), iv(iv), sockSrv(sockSrv), addrCli(addrCli)
	{
		a = {
			sockSrv,
			addrCli,
			key,
			iv
		};
	}
	//recvfrom(sockSrv, recvBuf, 800, 0, (SOCKADDR*)&addrCli, &len))
	args getArgs()
	{	

		Pargs arguments = (Pargs)&a;
		return a;
	}

	BOOL startSending()
	{
		//sending until the program stop
		using namespace CryptoPP;
		int len = sizeof(SOCKADDR_IN);
		std::string plain;




		unsigned int ThreadId;
		//args a = getArgs();
		_beginthreadex(NULL, 0, &thread_recvfrom, &a, 0, &ThreadId);

		while (1)
		{
			cipher.clear();
			std::getline(std::cin, plain);
			try
			{
				CBC_Mode< AES >::Encryption e;
				e.SetKeyWithIV(key, key.size(), iv);
				CryptoPP::StringSource s(plain, true, new StreamTransformationFilter(e, new StringSink(cipher)));
			}
			catch (const Exception& e)
			{
				std::cerr << e.what() << std::endl;
				return FALSE;
			}

			const char* sendBuf = cipher.data();
			sendto(sockSrv, sendBuf, cipher.length(), 0, (SOCKADDR*)&addrCli, len);

		}
		return FALSE;
	}
};



unsigned WINAPI thread_start_server(void* arg);



BOOL start_server(bool* running);