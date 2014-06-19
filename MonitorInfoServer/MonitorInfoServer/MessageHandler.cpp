#include "StdAfx.h"
#include "MessageHandler.h"
#include <vector>


CMessageHandler::CMessageHandler(void)
{
	m_sockFd=INVALID_SOCKET;
	m_pData=NULL;
}


CMessageHandler::~CMessageHandler(void)
{
}
void CMessageHandler::Init(SOCKET fd,const void* pData)
{
	m_sockFd=fd;
	m_pData=pData;
}
int CMessageHandler::DealMessage()
{
	int ret;
	RequestMessageType requestMsg;
	timeval tm;
	tm.tv_sec=2;
	tm.tv_usec=500*1000;
	ret=RecvRequest(&requestMsg,&tm);
	if(ret < 0)
	{
		requestMsg.FreeContent();
		return ret;
	}
	ResponseMessageType responseMsg;
	ret=CreateResponse(&requestMsg,&responseMsg);
	requestMsg.FreeContent();
	if(ret < 0)
	{
		return ret;
	}
	ret=SendResponse(&responseMsg,&tm);
	responseMsg.FreeContent();
	return ret;
}
int CMessageHandler::RecvRequest(RequestMessageType* message,const timeval* tmval)
{ 
	int ret;
	ret=Readn(m_sockFd,&message->type,sizeof(message->type),tmval);
	if(ret < 0)
	{
		return ret;
	}
	ret=Readn(m_sockFd,&message->length,sizeof(message->length),tmval);
	if(ret < 0)
	{
		return ret;
	}
	if(message->length)
	{
		message->InitContent(message->length);
		return Readn(m_sockFd,message->pContent,message->length,tmval);
	}
	return 0;
}

int CMessageHandler::CreateResponse(const RequestMessageType* request,ResponseMessageType* response)
{
	if(request->type == GET)
	{
		const std::vector<MonitorRect>* pRects=(const std::vector<MonitorRect>*)m_pData;
		response->code=SUCCESS;
		response->length=pRects->size()*(sizeof(long)*4);
		response->InitContent(response->length);
		size_t index=0;
		const size_t step=sizeof(long);
		for(size_t i=0;i<pRects->size();++i)
		{
			memcpy(response->pContent+index,&(*pRects)[i].left,step);
			index += step;
			memcpy(response->pContent+index,&(*pRects)[i].top,step);
			index += step;
			memcpy(response->pContent+index,&(*pRects)[i].right,step);
			index += step;
			memcpy(response->pContent+index,&(*pRects)[i].bottom,step);
			index += step;
		}
	}
	else
		return -1;
	return 0;
}

int CMessageHandler::SendResponse(const ResponseMessageType* message,const timeval* tmval)
{
	int ret;
	ret=Writen(m_sockFd,&message->code,sizeof(message->code),tmval);
	if(ret < 0)
		return ret;
	ret=Writen(m_sockFd,&message->length,sizeof(message->length),tmval);
	if(ret < 0)
		return ret;
	if(message->length)
	{
		return Writen(m_sockFd,message->pContent,message->length,tmval);
	}
	return 0;
}
