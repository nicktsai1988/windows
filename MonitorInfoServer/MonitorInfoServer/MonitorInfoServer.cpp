#include "stdafx.h"

#include "MonitorInfoServer.h"
#include "MessageHandler.h"
#include "Monitor.h"

#pragma comment(lib,"ws2_32.lib")

bool CMonitorInfoServer::InitLibrary()
{
	WSADATA WSAData;
	return ( 0 == WSAStartup(MAKEWORD(2,2),&WSAData));
}
bool CMonitorInfoServer::ReleaseLibrary()
{
	return ( 0 == WSACleanup());
}
CMonitorInfoServer::CMonitorInfoServer()
{
	m_bExit=false;
	m_pRects=NULL;
	m_hSrvThread=NULL;
	m_pRects=NULL;
	m_bRunning=false;

}
CMonitorInfoServer::~CMonitorInfoServer()
{
	if((false == m_bExit) &&(NULL != m_hSrvThread ))
	{
		m_bExit=true;
		WaitForSingleObject(m_hSrvThread,INFINITE);
		CloseHandle(m_hSrvThread);
		m_hSrvThread=NULL;
	}
}
void CMonitorInfoServer::InitServer(ULONG address,UINT32 port)
{
	m_nAddress=address;
	m_nPort=port;
}
bool CMonitorInfoServer::InitData()
{
	Monitor monitorLoader;
	std::vector<MONITOR_INFO> monitors;
	if(!monitorLoader.LoadMonitorInfo(monitors))
	{
		return false;
	}
	if(!monitors.size())
	{
		return false;
	}
	m_aMonitorRects.clear();
	for(size_t i=0;i<monitors.size();++i)
	{
		m_aMonitorRects.push_back(MonitorRect(monitors[i].MonitorRect));
	}
	m_pRects=&m_aMonitorRects;
	return true;
}
int CMonitorInfoServer::Start()
{
	m_bExit=false;
	m_bRunning=true;//if false,the server could not be running
	m_hSrvThread=CreateThread(NULL,0,&ThreadProc,PVOID(this),0,NULL);
	if(m_hSrvThread == NULL)
	{
		m_bExit=true;
		m_bRunning=false;
		return -1;
	}
	return 0;
}

int CMonitorInfoServer::Stop()
{
	
	if((false == m_bExit)&& (NULL != m_hSrvThread))
	{
		m_bExit=true;
		WaitForSingleObject(m_hSrvThread,INFINITE);
		CloseHandle(m_hSrvThread);
		m_hSrvThread=NULL;
	}
	return 0;
}

DWORD WINAPI ThreadProc(LPVOID pParam)
{
	CMonitorInfoServer* obj=(CMonitorInfoServer*)pParam;
	obj->m_bRunning=true;
	SOCKET srvSock;
	int ret;
	srvSock=socket(AF_INET,SOCK_STREAM,0);
	if(INVALID_SOCKET == srvSock)
	{
		obj->m_bRunning=false;
		return -1;
	}
	u_long imode=1;
	if(SOCKET_ERROR ==ioctlsocket(srvSock,FIONBIO, &imode))
	{
		
		closesocket(srvSock);
		obj->m_bRunning=false;
		return -1;
	}
	SOCKADDR_IN srvAddr;
	memset(&srvAddr,0,sizeof(srvAddr));
	srvAddr.sin_family=AF_INET;
	srvAddr.sin_addr.S_un.S_addr=obj->m_nAddress;
	srvAddr.sin_port=obj->m_nPort;
	if(SOCKET_ERROR == bind(srvSock,(const SOCKADDR*)&srvAddr,sizeof(srvAddr)))
	{
		closesocket(srvSock);
		obj->m_bRunning=false;
		return -1;
	}
	if(SOCKET_ERROR == listen(srvSock,5))
	{
		closesocket(srvSock);
		obj->m_bRunning=false;
		return -1;
	}
	SOCKET clientSock;
	SOCKADDR_IN clientAddr;
	int addrLen=sizeof(clientAddr);
	timeval tm;
	tm.tv_sec=2;
	tm.tv_usec=500*1000;

	while(!obj->m_bExit)
	{
		if(ReadyRead(srvSock,&tm))
		{
			memset(&clientAddr,0,sizeof(clientAddr));
			clientSock=accept(srvSock,(SOCKADDR*)&clientAddr,&addrLen);
			if(INVALID_SOCKET == clientSock)
			{
				continue;
			}
			u_long imode=1;
			if(SOCKET_ERROR ==ioctlsocket(clientSock,FIONBIO, &imode))
			{
				closesocket(clientSock);
				continue;
			}
			CMessageHandler msgHandler;
			msgHandler.Init(clientSock,obj->m_pRects);
			ret=msgHandler.DealMessage();
			closesocket(clientSock);
		}
	}
	closesocket(srvSock);
	obj->m_bRunning=false;
	return 0;
}
