#include "stdafx.h"
#include "MessageTypeUtils.h"
#include <errno.h>

bool ReadyRead(SOCKET fd,const timeval* tmval)
{
	fd_set read_set;
	FD_ZERO(&read_set);
	FD_SET(fd,&read_set);
	int ret=select(fd+1,&read_set,NULL,NULL,tmval);
	if(ret <= 0)
	{
		return false;
	}
	else
	{
		if(FD_ISSET(fd,&read_set))
			return true;
		else
			return false;
	}
}
bool ReadyWrite(SOCKET fd,const timeval* tmval)
{
	fd_set write_set;
	FD_ZERO(&write_set);
	FD_SET(fd,&write_set);
	int ret=select(fd+1,NULL,&write_set,NULL,tmval);
	if(ret <= 0)
	{
		return false;
	}
	else
	{
		if(FD_ISSET(fd,&write_set))
			return true;
		else
			return false;
	}
}

int Readn(SOCKET fd,void* buf,size_t size,const timeval* tmval)
{

	size_t nleft;
	int nread,error;
	char *ptr=(char*)buf;
	nleft=size;
	while(nleft>0)
	{
		if(ReadyRead(fd,tmval))
		{
			if((nread=recv(fd,ptr,nleft,0))<0)
			{
				error=WSAGetLastError();
				if(error==EINTR)
					nread=0;
				else
				{
					return -1;
				}
			}
			else
				if(nread==0)
					break;
			nleft-=nread;
			ptr+=nread;
		}
		else
			return -1;	
	}
	return (size-nleft);
}
int Writen(SOCKET fd,const void* buf,size_t size,const timeval* tmval)
{
	size_t nleft;
	int nwritten,error;
	const char* ptr=(char*)buf;
	nleft=size;
	while(nleft>0)
	{	
		if(ReadyWrite(fd,tmval))
		{
			if((nwritten=send(fd,ptr,nleft,0))<=0)
			{
				error=WSAGetLastError();
				if(nwritten<0&&error==EINTR)
					nwritten=0;
				else
				{
					return -1;
				}
			}
			nleft-=nwritten;
			ptr+=nwritten;
		}
		else
			return -1;
	}
	return size;
}

RequestMessageType::RequestMessageType()
{
	type=0;
	length=0;
	pContent=NULL;
}
RequestMessageType::~RequestMessageType()
{
	delete[] pContent;
}
void RequestMessageType::InitContent(size_t size)
{
	pContent=new UINT8[size];

}
void RequestMessageType::FreeContent()
{
	delete[] pContent;
	pContent=NULL;
}

ResponseMessageType::ResponseMessageType()
{
	code=0;
	length=0;
	pContent=NULL;
}
ResponseMessageType::~ResponseMessageType()
{
	delete[] pContent;
}
void ResponseMessageType::InitContent(size_t size)
{
	pContent=new UINT8[size];
}
void ResponseMessageType::FreeContent()
{
	delete[] pContent;
	pContent=NULL;
}

MonitorArray::MonitorArray()
{
	m_nCount=0;
	//m_nByteStreamLength=0;
	m_pRects=NULL;
	//m_pMonitorByteStream=NULL;
}
MonitorArray::~MonitorArray()
{
	delete[] m_pRects;
}
void MonitorArray::IintRectsSpace(size_t rectNum)
{
	m_pRects=new MonitorRect[rectNum];
}
void MonitorArray::FreeRectsSpace()
{
	delete[] m_pRects;
	m_pRects=NULL;
}