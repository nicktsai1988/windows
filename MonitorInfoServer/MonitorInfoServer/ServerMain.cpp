// main.cpp
//
#include "StdAfx.h"
#include "ServerControl.h"
#include "MonitorInfoServer.h"

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpCmdLine,
                     int nCmdShow)
{
    SetThreadExecutionState(ES_CONTINUOUS|ES_DISPLAY_REQUIRED|ES_SYSTEM_REQUIRED);
    HANDLE hMutex = CreateMutex(NULL, FALSE, _T("MonitorInfo Server"));

    if (hMutex == INVALID_HANDLE_VALUE ||
        GetLastError() == ERROR_ALREADY_EXISTS)
    {
        MessageBox(NULL, (_T("MonitorInfo Server is already running")), _T("MonitorInfo Server"), MB_ICONINFORMATION);
        return 0;
    }

	if (g_ServerControl().Init() < 0)
	{
		CloseHandle(hMutex);
		MessageBox(NULL, _T("Initialize the server error!"), _T("MonitorInfo Server"), MB_ICONERROR);
		return 0;
	}
	CMonitorInfoServer::InitLibrary();

    g_ServerControl().StartServer();

	int ret = g_ServerControl().MainLoop();

	g_ServerControl().StopServer();
	g_ServerControl().Release();
	CMonitorInfoServer::ReleaseLibrary();
	CloseHandle(hMutex);
	return ret;
}