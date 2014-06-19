// system.cpp
//
#include "StdAfx.h"
#include "ServerControl.h"
#include "ServerTray.h"
#include "resource.h"
#include "Dialog.h"
#include "MonitorInfoServer.h"

ServerControl &g_ServerControl()
{
	static ServerControl the_system;
	return the_system;
}

ServerControl::ServerControl()
{
	m_pTray=NULL;
	m_pServer=NULL;
	m_pAboutDlg=NULL;
}
ServerControl::~ServerControl()
{
	Release();
}

int ServerControl::Init()
{
	m_pTray = new ServerTray();
	m_pAboutDlg = new Dialog();
	m_pServer = new CMonitorInfoServer;
	
	if (m_pTray->ControllerServerTrayCreate() < 0)
		return -1;
	if(!m_pServer->InitData())
		return -1;
	m_pServer->InitServer(htonl(INADDR_ANY),htons(CMonitorInfoServer::DEFAULY_PORT));
	return 0;
}

BOOL ServerControl::StartServer()
{
    if (m_pAboutDlg->IsValid())
        m_pAboutDlg->Show(false);
	m_pTray->HideItem();
	if(m_pServer->Start() < 0)
		return FALSE;
    return TRUE;
}

BOOL ServerControl::StopServer()
{
    if (m_pAboutDlg->IsValid())
        m_pAboutDlg->Show(false);
	m_pTray->ShowItem();
	m_pServer->Stop();
    return TRUE;
}

int ServerControl::Release()
{
	if(m_pTray != NULL)
	{
		delete m_pTray;
		m_pTray=NULL;
	}
	if(m_pAboutDlg != NULL)
	{
		delete m_pAboutDlg;
		m_pAboutDlg=NULL;
	}
	if(m_pServer != NULL)
	{
		m_pServer->Stop();
		delete m_pServer;
		m_pServer=NULL;
	}
	return 0;
}

void CALLBACK ServerControl::TimerProc(HWND hwnd, UINT message, UINT timerID, DWORD time) 
{
	if (g_ServerControl().m_pServer)
	{
		if (!g_ServerControl().m_pServer->IsRunning())
		{
            PostQuitMessage(0);
		}
	}
}

int ServerControl::MainLoop()
{
	MSG msg;

    SetTimer(NULL, 0, 2500, ServerControl::TimerProc); 

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
    KillTimer(NULL, 0); 

	return msg.wParam;
}

int ServerControl::ShowAboutDlg()
{
    if (m_pAboutDlg->IsValid())
    {
        m_pAboutDlg->SetFocus();
        m_pAboutDlg->Show();
    }
	else
	{
		m_pAboutDlg->Create(IDD_ABOUTBOX);
		m_pAboutDlg->SetFocus();
		m_pAboutDlg->Show();
	}
	return 0;
}


