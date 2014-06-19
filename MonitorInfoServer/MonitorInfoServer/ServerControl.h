// System.h
//
#pragma once

class ServerTray;
class Dialog;
class ServerSystem;
class ServerConfigSetting;
class CMonitorInfoServer;

class ServerControl
{
public:
	ServerControl();
	~ServerControl();
	int Init();
	int Release();

    /** @brief System thread main loop, used to dispatch windows message.
    */
	int MainLoop();
    
    static void CALLBACK TimerProc(HWND hwnd, UINT message, UINT timerID, DWORD time);

    /** @brief To reload the server settings.
    */
	//int RefreshServerSettings();

	int ShowAboutDlg();

    BOOL StartServer();
    BOOL StopServer();

private:
		ServerTray*			  m_pTray;
		Dialog*               m_pAboutDlg;
		CMonitorInfoServer*	  m_pServer;
};

ServerControl& g_ServerControl();

