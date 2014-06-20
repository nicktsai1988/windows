#include <WinSock2.h>

#define SUCCESS 0

#define GET 1
#define POST 2

inline UINT8 HighByte(UINT16 data)
{
	return data>>8;
}
inline UINT8 LowByte(UINT16 data)
{
	return data&0xFF;
}

inline UINT16 RecombineByte(UINT8 high,UINT8 low)
{
	return (high<<8)+low;
}


bool ReadyRead(SOCKET fd,const timeval* tmval);
bool ReadyWrite(SOCKET fd,const timeval* tmval);
int Writen(SOCKET fd,const void* buf,size_t size,const timeval* tmval);
int Readn(SOCKET fd,void* buf,size_t size,const timeval* tmval);

struct MonitorRect:public RECT
{
	//0 left
	//1 top
	//2 right
	//3 bottom
	//int edges[4];
	MonitorRect()
	{
		left=right=top=bottom=0;
	}
	MonitorRect(long l,long t,long r,long b)
	{
		left=l;
		top=t;
		right=r;
		bottom=b;
	}
};

struct MonitorArray
{
	UINT16 m_nCount;
	MonitorRect* m_pRects;
	MonitorArray();
	~MonitorArray();
	void IintRectsSpace(size_t rectNum);
	void FreeRectsSpace();
};

struct RequestMessageType
{
	UINT8 type;
	UINT16 length;
	UINT8* pContent;
	RequestMessageType();
	~RequestMessageType();
	void InitContent(size_t size);
	void FreeContent();
};
struct ResponseMessageType
{
	UINT8 code;
	UINT16 length;
	UINT8* pContent;
	ResponseMessageType();
	~ResponseMessageType();

	void InitContent(size_t size);
	void FreeContent();
};