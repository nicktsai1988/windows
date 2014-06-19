// tray.cpp
//
#include "StdAfx.h"
#include "Tray.h"
#include "resource.h"

 
#define TRAYCLASS _T("MonitorInfoServer_Tray")

#define WM_SHELLNOTIFY WM_USER+5

const TCHAR* tszToolTip=_T("MonitorInfo Server");

Tray::Tray()
{
	hTray = NULL;
	hIcon = NULL;
	hSubMenu = hMenu = NULL;
	memset(&nid, 0, sizeof(nid));
}

Tray::~Tray()
{
	Destroy();
}

LRESULT CALLBACK Tray_WndProcStub(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	Tray *tray = (Tray*)GetWindowLong(hWnd, GWL_USERDATA);

	if (tray)
		return tray->WndProc(hWnd, uMsg, wParam, lParam);
	else
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int Tray::Create(int nIconId, int nMenuId)
{
	WNDCLASS wc;
	HINSTANCE hInstance = (HINSTANCE)GetModuleHandle(NULL);

	hMenu = LoadMenu(hInstance, MAKEINTRESOURCE(nMenuId));
	hSubMenu = GetSubMenu(hMenu, 0);
	//SetMenuDefaultItem(hSubMenu, 0, 1);
	if (!hMenu || !hSubMenu)
		return -1;

	if (SetIcon(nIconId) < 0)
		return -1;

	wc.cbWndExtra = 0;
	wc.cbClsExtra = 0;
	wc.style = 0;
	wc.lpfnWndProc = Tray_WndProcStub;
	wc.hInstance = hInstance;
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = TRAYCLASS;
	wc.hIcon = NULL;
	wc.hCursor = NULL;

	RegisterClass(&wc);
    
	hTray = CreateWindow(TRAYCLASS, _T("Server Tray Icon"), 0, 0, 0, 0, 0,
		NULL, NULL, hInstance, this);

	if (!hTray)
		return -1;

	SetWindowLong(hTray, GWL_USERDATA, (LONG)this);

	nid.cbSize = sizeof(nid);
	nid.hWnd = hTray;
	nid.uID = 0;
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	nid.uCallbackMessage = WM_SHELLNOTIFY;
	nid.hIcon = hIcon;
	_tcscpy(nid.szTip, tszToolTip);
	
	::Shell_NotifyIcon(NIM_ADD, &nid);

	return 0;
}

int Tray::Destroy()
{
	if (nid.hWnd)
	{
		Shell_NotifyIcon(NIM_DELETE, &nid);
		memset(&nid, 0, sizeof(nid));
	}

	if (hIcon) {
		DestroyIcon(hIcon); hIcon = NULL;
	}

	if (hMenu) {
		DestroyMenu(hMenu);	hMenu = NULL;
	}

	if (hTray) {
		DestroyWindow(hTray); hTray = NULL;
	}

	UnregisterClass(TRAYCLASS, GetModuleHandle(NULL));
	
	return 0;
}

int Tray::SetIcon(int nIconId)
{
	if (hIcon) {
		DestroyIcon(hIcon);
		hIcon = NULL;
	}

	hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(nIconId));
	if (!hIcon)
		return -1;

	if (nid.hWnd)
	{
		nid.hIcon = hIcon;
		Shell_NotifyIcon(NIM_MODIFY, &nid);
	}
	return 0;
}

int Tray::SetMenuItemText(int nId, TCHAR*szText)
{
	if (!hSubMenu)
		return -1;

	if (!ModifyMenu(hSubMenu, nId, MF_BYCOMMAND | MF_STRING, nId, szText))
		return -1;
	return 0;
}

LRESULT Tray::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_COMMAND:
			
			if (HIWORD(wParam) == 0)
				OnCommand(LOWORD(wParam));

			break;

		case WM_SHELLNOTIFY:	
			
			if (wParam != 0)
				break;

			if (lParam == WM_RBUTTONUP)
			{
				ShowMenu();
			}
			else if (lParam == WM_LBUTTONDBLCLK)
			{
				UINT nDefCmd = GetMenuDefaultItem(hSubMenu, 0, 0);
				if (nDefCmd != (UINT)(-1))
					SendMessage(hWnd, WM_COMMAND, nDefCmd, 0);
			}
			else
				break;

			return TRUE;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int Tray::ShowMenu()
{
	SetForegroundWindow(hTray);
	
	POINT pt;
	GetCursorPos(&pt);
	
	TrackPopupMenu(hSubMenu, TPM_RIGHTALIGN, pt.x, pt.y, NULL, hTray, NULL);
	PostMessage(hTray, WM_NULL, 0, 0);

	return 0;
}

int Tray::OnCommand(int nId)
{
	return 0;
}

void Tray::ShowItem()
{
	
}

void Tray::HideItem()
{
	
}
