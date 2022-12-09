#pragma once

#include <iostream>
#include <WinSock2.h>
//#include <ws2tcpip.h>
#include <tchar.h>
class CPacket
{
public:
	CPacket() :sHead(0), nLength(0), sCmd(0), sSum(0)
	{

	}
	CPacket(WORD nCmd, const BYTE* pData, size_t nSize)
	{
		sHead = 0xFEFF;
		nLength = nSize + 4;
		sCmd = nCmd;
		if (nSize > 0)
		{
			strData.resize(nSize);
			memcpy((void*)strData.c_str(), pData, nSize);
		}
		else
		{
			strData.clear();
		}
		sSum = 0;
		for (int j = 0; j < strData.size(); j++)
		{
			sSum += BYTE(strData[j] & 0xFF);
		}
	}
	CPacket(const CPacket& pack)
	{
		sHead = pack.sHead;
		nLength = pack.nLength;
		sCmd = pack.sCmd;
		strData = pack.strData;
		sSum = pack.sSum;
	}
	CPacket(const BYTE* pData, size_t& nSize)
	{
		size_t i = 0;
		for (; i < nSize; i++)
		{
			if (*(WORD*)(pData + i) == 0xFEFF)
			{
				sHead = *(WORD*)(pData + i);
				i += 2;
				break;
			}
		}
		if (i+4+2+2 >= nSize)		//Packet data might not be complete.
		{
			nSize = 0;
			return;
		}
		nLength = *(DWORD*)(pData + i); i += 4;
		if (nLength + i > nSize)	//Packet is not received fully. 
		{
			nSize = 0;
			return;
		}
		sCmd = *(WORD*)(pData + i); i += 2;
		if (nLength > 4)
		{
			strData.resize(nLength - 2 - 2);
			memcpy((void*)strData.c_str(), pData + i, nLength - 4);
			i += nLength - 4;
		}
		sSum = *(WORD*)(pData + i); i += 2;
		
		WORD sum = 0;
		for (int j = 0; j < strData.size(); j++)
		{
			sum += BYTE(strData[j] & 0xFF);
		}
		if (sum == sSum)
		{
			nSize = i;
			return;
		}
		nSize = 0;
	}
	~CPacket()
	{

	}

	CPacket& operator=(const CPacket& pack)
	{
		if (this != &pack)
		{
			sHead = pack.sHead;
			nLength = pack.nLength;
			sCmd = pack.sCmd;
			strData = pack.strData;
			sSum = pack.sSum;
		}
		return *this;
	}

	int Size()
	{
		return nLength + 6;
	}

	const char* Data()
	{
		strOut.resize(nLength + 6);
		BYTE* pData = (BYTE*)strOut.c_str();
		*(WORD*)pData = sHead; pData += 2;
		*(DWORD*)(pData) = nLength; pData += 4;
		*(WORD*)pData = sCmd; pData += 2;
		memcpy(pData, strData.c_str(), strData.size()); pData += strData.size();
		*(WORD*)pData = sSum;
		return strOut.c_str();
	}

public:
	WORD sHead;				//const packet head FEFF
	DWORD nLength;			//packet length
	WORD sCmd;				//command
	std::string strData;	//packet data
	WORD sSum;				//sum check
	std::string strOut;		//Data of the whole Packet
};

#pragma pack(pop)

typedef struct MouseEvent
{
	MouseEvent()
	{
		nAction = 0;
		nButton = -1;
		ptXY.x = 0;
		ptXY.y = 0;
	}
	WORD nAction;
	WORD nButton;
	POINT ptXY;
}MOUSEEV, *PMOUSEEV;









class CServerSocket
{
public:
	static CServerSocket* getInstance()
	{
		if (m_instance == NULL)
		{
			m_instance = new CServerSocket();
		}
		return m_instance;
	}

	bool InitSocket()
	{
		if (m_sock == -1)
		{
			return false;
		}

		sockaddr_in serv_adr;
		memset(&serv_adr, 0, sizeof(serv_adr));
		serv_adr.sin_family = AF_INET;
		serv_adr.sin_addr.s_addr = INADDR_ANY;
		serv_adr.sin_port = htons(6001);


		if (::bind(m_sock, (sockaddr*)&serv_adr, sizeof(serv_adr)) == SOCKET_ERROR)
		{
			wprintf(L"bind failed with error %u\n", WSAGetLastError());
			return false;
		}

		if (listen(m_sock, 1) == SOCKET_ERROR)
			return false;

		return true;
	}

	bool AcceptClient() 
	{
		sockaddr_in client_adr;
		int cli_sz = sizeof(client_adr);
		SOCKET client = accept(m_sock, (sockaddr*)&client_adr, &cli_sz);
		if (client == INVALID_SOCKET)
		{
			return false;
		}
		return true;
	}

#define BUFFER_SIZE 4096
	int DealCommand()
	{
		if (m_client == INVALID_SOCKET)
			return -1;
		char* buffer = new char[BUFFER_SIZE];
		memset(buffer, 0, BUFFER_SIZE);
		size_t index = 0;
		while (true)
		{
			size_t len = recv(m_client, buffer, BUFFER_SIZE - index, 0);
			if (len <= 0)
			{
				return -1;
			}
			index += len;
			len = index;
			m_packet = CPacket((BYTE*)buffer, len);
			if (len > 0)
			{
				memmove(buffer, buffer + len, BUFFER_SIZE - len);
				index -= len;
				return m_packet.sCmd;
			}
		}
		return -1;
	}

	bool Send(const char* pData, size_t nSize)
	{
		if (m_client == -1)
			return false;
		return send(m_client, pData, nSize, 0) > 0;
	}
	bool Send(CPacket& pack)
	{
		if (m_client == -1)
			return false;
		return send(m_client, pack.Data(), pack.Size(), 0) > 0;
	}

	bool GetFilePath(std::string& strPath)
	{
		if ((m_packet.sCmd >= 2) && (m_packet.sCmd <= 4))
		{
			strPath = m_packet.strData;
			return true;
		}
		return false;
	}


	bool GetMouseEvent(MOUSEEV& mouse)
	{
		if (m_packet.sCmd == 5)
		{
			memcpy(&mouse,  m_packet.strData.c_str(), sizeof(MOUSEEV));
			return true;
		}
		return false;
	}
private:
	SOCKET m_client;
	SOCKET m_sock;
	CPacket m_packet;
	CServerSocket& operator=(const CServerSocket& ss) {}
	CServerSocket(const CServerSocket& ss) 
	{
		m_sock = ss.m_sock;
		m_client = ss.m_client;
	}
	CServerSocket() 
	{
		m_client = INVALID_SOCKET;
		if (InitSockEvn() == FALSE)
		{
			std::cout << "cannot Initialze socket evironment, failed" << std::endl;
		}
		m_sock = socket(PF_INET, SOCK_STREAM, 0);
		if (m_sock == INVALID_SOCKET)
			wprintf(L"socket function failed with error = %d\n", WSAGetLastError());
	}
	~CServerSocket() 
	{
		closesocket(m_sock);
		WSACleanup();
	}
	BOOL InitSockEvn()
	{
		WSADATA data;
		if (WSAStartup(MAKEWORD(1, 1), &data) != 0)
		{
			return FALSE;
		}
		return TRUE;
	}
	static void releaseInstance()
	{
		if (m_instance != NULL)
		{
			CServerSocket* tmp = m_instance;
			m_instance = NULL;
			delete tmp;
		}
	}

	static CServerSocket* m_instance;

	class CHelper
	{
	public:
		CHelper()
		{
			CServerSocket::getInstance();
		}
		~CHelper()
		{
			CServerSocket::releaseInstance();
		}
	};
	static CHelper m_helper;
};


//extern CServerSocket server;