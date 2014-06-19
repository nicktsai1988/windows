// Tray.h
//

#ifndef TRAY_H
#define TRAY_H
#include <ShellAPI.h>

class Tray
{
	public:
		Tray();
		~Tray();

		int Create(int nIconId, int nMenuId);
		int Destroy();

		int SetIcon(int nIconId);
		int SetMenuItemText(int nId, TCHAR* szText);

		void ShowItem();
		void HideItem();

		LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		virtual int OnCommand(int nId);

	private:
		int ShowMenu();

	protected:
		HWND            hTray;
		HICON           hIcon;
		HMENU           hMenu, hSubMenu;
		NOTIFYICONDATA  nid;
};

#endif