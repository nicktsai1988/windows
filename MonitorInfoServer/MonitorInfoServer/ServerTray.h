#pragma once

#include "Tray.h"

class ServerTray : private Tray
{
	public:
		ServerTray();
		~ServerTray();

		int ControllerServerTrayCreate();
		int ControllerServerTrayDestroy();
		
		virtual int OnCommand(int nId);

		int OnMenuAbout();
		int OnMenuExit();

		void ShowItem();
		void HideItem();
};