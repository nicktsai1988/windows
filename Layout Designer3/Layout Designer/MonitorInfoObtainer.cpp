#include "StdAfx.h"
#include "MonitorInfoObtainer.h"

bool CMonitorInfoObtainer::InitLibrary()
{
	WSADATA WSAData;
	return ( 0 == WSAStartup(MAKEWORD(2,2),&WSAData));
}

bool CMonitorInfoObtainer::ReleaseLibrary()
{
	return ( 0 == WSACleanup());
}

CMonitorInfoObtainer::CMonitorInfoObtainer(void)
{

}


CMonitorInfoObtainer::~CMonitorInfoObtainer(void)
{
	
}

bool CMonitorInfoObtainer::Load(ULONG address,UINT32 port)
{
	int ret;
	SOCKET sockFd;
	sockFd=socket(AF_INET,SOCK_STREAM,0);
	if(sockFd == INVALID_SOCKET)
	{
		//AfxMessageBox(_T("Create socket error!"));
		return false;
	}

	SOCKADDR_IN serverAddr;
	memset(&serverAddr,0,sizeof(serverAddr));
	serverAddr.sin_family=AF_INET;
	serverAddr.sin_port=port;
	serverAddr.sin_addr.S_un.S_addr=address;
	ret=connect(sockFd,(struct sockaddr*)&serverAddr,sizeof(serverAddr));
	if(SOCKET_ERROR == ret)
	{
		closesocket(sockFd);
		//AfxMessageBox(_T("Connect to server error!"));
		return false;
	}
	RequestMessageType requestMsg;
	requestMsg.type=GET;
	requestMsg.length=0;
	requestMsg.pContent=NULL;
	ret=SendRequest(sockFd,&requestMsg);
	if(ret < 0)
	{
		closesocket(sockFd);
		//AfxMessageBox(ErrorString(_T("Send request error"),ret));
		return false;
	}
	requestMsg.FreeContent();

	ResponseMessageType responseMsg;
	ret=RecvResponse(sockFd,&responseMsg);
	if(ret < 0)
	{
		closesocket(sockFd);
		//AfxMessageBox(ErrorString(_T("Recv response error"),ret));
		return false;
	}
	closesocket(sockFd);
	ret=ParseData(&responseMsg);
	if(ret < 0)
	{
		//AfxMessageBox(ErrorString(_T("Parse data error"),ret));
		return false;
	}
	responseMsg.FreeContent();
	return true;
}

CString CMonitorInfoObtainer::ErrorString(CString str,int code)
{
	CString ret;
	ret.Format(_T("%s: %d"),str,code);
	return ret;
}

int CMonitorInfoObtainer::SendRequest(SOCKET fd,const RequestMessageType* message)
{
	int ret;
	struct timeval tm;
	//2.5s
	tm.tv_sec=2;
	tm.tv_usec=500*1000;
	ret=Writen(fd,&message->type,sizeof(message->type),&tm);
	if(ret < 0)
		return ret;
	ret=Writen(fd,&message->length,sizeof(message->length),&tm);
	if(ret < 0)
		return ret;
	if(message->length)
	{
		ret=Writen(fd,message->pContent,message->length,&tm);
		if(ret < 0)
			return ret;
	}
	return 0;
}
int CMonitorInfoObtainer::RecvResponse(SOCKET fd,ResponseMessageType* message)
{
	int ret;
	struct timeval tm;
	//2.5s
	tm.tv_sec=2;
	tm.tv_usec=500*1000;
	ret=Readn(fd,&message->code,sizeof(message->code),&tm);
	if(ret < 0)
		return ret;
	ret=Readn(fd,&message->length,sizeof(message->length),&tm);
	if(ret < 0)
		return ret;
	if(message->length)
	{
		message->InitContent(message->length);
		ret=Readn(fd,message->pContent,message->length,&tm);
		if(ret < 0)
			return ret;
	}
	return 0;
}
int CMonitorInfoObtainer::ParseData(const ResponseMessageType* message)
{
	m_aMonitorRects.clear();
	if(message->code != SUCCESS)
		return -1;
	long left,top,right,bottom;
	int count=message->length/(sizeof(long)*4);
	if((count*sizeof(long)*4) != message->length)
		return -1;
	size_t index=0;
	const size_t step=sizeof(long);
	for(int i=0;i<count;++i)
	{
		memcpy(&left,message->pContent+index,step);
		index += step;
		memcpy(&top,message->pContent+index,step);
		index += step;
		memcpy(&right,message->pContent+index,step);
		index += step;
		memcpy(&bottom,message->pContent+index,step);
		index += step;
		m_aMonitorRects.push_back(MonitorRect(left,top,right,bottom));
	}
	return 0;
}

const std::vector<MonitorRect>& CMonitorInfoObtainer::GetMonitorRects()const
{
	return m_aMonitorRects;
}

