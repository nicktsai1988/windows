#pragma once
#include "MessageTypeUtils.h"
class CMessageHandler
{
public:
	CMessageHandler(void);
	void Init(SOCKET fd,const void* pData);
	int DealMessage();
	~CMessageHandler(void);
private:
	int RecvRequest(RequestMessageType* message,const timeval* tmval);
	int CreateResponse(const RequestMessageType* request,ResponseMessageType* response);
	int SendResponse(const ResponseMessageType* message,const timeval* tmval);
	SOCKET m_sockFd;
	const void* m_pData;
};

