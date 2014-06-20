
// Layout Designer.h : main header file for the Layout Designer application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CLayoutDesignerApp:
// See Layout Designer.cpp for the implementation of this class
//

class CLayoutDesignerApp : public CWinAppEx
{
public:
	CLayoutDesignerApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation

public:
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	afx_msg void onTest();
	DECLARE_MESSAGE_MAP()
};

extern CLayoutDesignerApp theApp;
