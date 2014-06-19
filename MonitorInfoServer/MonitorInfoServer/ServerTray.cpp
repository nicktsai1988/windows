// ControllerServerTray.cpp
//
#include "StdAfx.h"
#include "ServerTray.h"
#include "resource.h"
#include "ServerControl.h"

ServerTray::ServerTray()
{
}

ServerTray::~ServerTray()
{
	ControllerServerTrayDestroy();
}

int ServerTray::ControllerServerTrayCreate()
{
	if (Create(IDI_MONITORINFOSERVER, IDR_TRAYMENU) < 0)
		return -1;
	_tcscpy(nid.szTip, _T("MonitorInfo Server: On"));
	Shell_NotifyIcon(NIM_MODIFY, &nid);
	return 0;
}

int ServerTray::ControllerServerTrayDestroy()
{
	Destroy();
	return 0;
}

int ServerTray::OnCommand(int nId)
{
	switch(nId)
	{
	case ID_TRAYMENU_ABOUT:
		OnMenuAbout();
		break;
	case ID_TRAYMENU_EXIT:
		OnMenuExit();
		break;
	}
	return 0;
}

int ServerTray::OnMenuAbout()
{
	g_ServerControl().ShowAboutDlg();
	return 0;
}


int ServerTray::OnMenuExit()
{
	PostQuitMessage(0);
	return 0;
}

void ServerTray::ShowItem()
{
	Tray::ShowItem();
}

void ServerTray::HideItem()
{
	Tray::HideItem();
}
