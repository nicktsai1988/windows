#pragma once
#include <vector>
#include "MessageTypeUtils.h"
class CMonitorInfoObtainer
{
public:
	CMonitorInfoObtainer(void);
	~CMonitorInfoObtainer(void);
	bool Load(ULONG address,UINT32 port);
	const std::vector<MonitorRect>& GetMonitorRects()const;
	static bool InitLibrary();
	static bool ReleaseLibrary();
private:
	int SendRequest(SOCKET fd,const RequestMessageType* message);
	int RecvResponse(SOCKET fd,ResponseMessageType* message);
	//bool ReadyRead(SOCKET fd,const timeval* tmval);
	//bool ReadyWrite(SOCKET fd,const timeval* tmval);
	//int Writen(SOCKET fd,const void* buf,size_t size,const timeval* tmval);
	//int Readn(SOCKET fd,void* buf,size_t size,const timeval* tmval);
	int ParseData(const ResponseMessageType* message);
	CString ErrorString(CString str,int code);
	
	std::vector<MonitorRect> m_aMonitorRects;
	//SOCKET sockFd;
};

