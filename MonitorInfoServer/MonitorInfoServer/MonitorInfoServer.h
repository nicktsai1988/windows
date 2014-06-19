
#include "MessageTypeUtils.h"
#include <vector>
#include <windows.h>

DWORD WINAPI ThreadProc(LPVOID pParam);

class CMonitorInfoServer
{
public:
	enum{DEFAULY_PORT=9888};
	static bool InitLibrary();
	static bool ReleaseLibrary();
	CMonitorInfoServer();
	~CMonitorInfoServer();
	void InitServer(ULONG address,UINT32 port);
	bool InitData();
	int Start();
	int Stop();
	bool IsRunning()
	{
		return m_bRunning;
	}
private:
	friend DWORD WINAPI ThreadProc(PVOID pParam);
	HANDLE m_hSrvThread;
	volatile bool m_bExit;
	volatile bool m_bRunning;
	ULONG m_nAddress;
	UINT32 m_nPort;
	
	const std::vector<MonitorRect>* m_pRects;
	std::vector<MonitorRect> m_aMonitorRects;
};
