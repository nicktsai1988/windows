
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Layout Designer.h"

#include "MainFrm.h"
#include "Utils.h"
//using namespace DuiLib;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_WM_SIZE()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	//ID_SEPARATOR,
	ID_INDICATOR_NUM,
	//ID_SEPARATOR,
	ID_INDICATOR_SCRL,
};

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2008);
	m_pDuiToolBar=new DuiToolBar;
	m_pDuiStatusBar=new DuiStatusBar;
//	m_pDuiOperateWnd=new DuiOperateWnd;
	//m_pDuiMonitorInfo=new DuiOperateWnd;
	g_pDuiStatusBar=m_pDuiStatusBar;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

//	BOOL bNameValid;
	// set the visual manager and style based on persisted value
	OnApplicationLook(theApp.m_nAppLook);
	/*
	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("Failed to create menubar\n");
		return -1;      // fail to create
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);
	*/
	// prevent the menu bar from taking the focus on activation
	//CMFCPopupMenu::SetForceMenuFocus(FALSE);

	// create a view to occupy the client area of the frame
	if (!m_wndRuler.Create(NULL, NULL,  WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST+1, NULL))
	{
		TRACE0("Failed to create ruler ctrl window\n");
		return -1;
	}
	/*
	if( !m_wndMonitorType.Create(NULL, _T("Monitor Type"),  WS_CHILD | WS_VISIBLE | WS_VSCROLL, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST+1, NULL))
	{
		TRACE0("Failed to create monitor type window\n");
		return -1;
	}
	*/
	CRect rect;
	GetClientRect(rect);
	rect.top=60;
	rect.right=200;
	if( !m_paneMonitorType.Create(_T("Monitor Type"), this, rect,TRUE,AFX_IDW_PANE_FIRST+2,
		 WS_CHILD | WS_VISIBLE |CBRS_LEFT, AFX_CBRS_REGULAR_TABS, AFX_CBRS_RESIZE))
	{
		TRACE0("Failed to create monitor type window\n");
		return -1;
	}
	
	//EnableDocking(CBRS_ALIGN_LEFT);
	m_paneMonitorType.EnableDocking(CBRS_ALIGN_LEFT);
	DockPane(&m_paneMonitorType);
	g_pMonitorTypeWnd=m_paneMonitorType.GetMonitorTypeWnd();
	//m_paneMonitorType.SetResizeMode(FALSE);
	//m_paneMonitorType.ShowPane(TRUE,FALSE,TRUE);

//	m_wndRuler.SetScrollSize(CSize(1024,768));
	/*
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	CString strToolBarName;
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	*/


	/*
	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));
	
	*/

	/*
	CMFCToolBarButton testButton;
	testButton.m_strText=CString(_T("Test"));
	testButton.m_nID=5;
	CRect rect(1,0,22,22);
	testButton.SetRect(rect);
	m_wndToolBar.InsertButton(testButton);
	




	// TODO: Delete these five lines if you don't want the toolbar and menubar to be dockable
	//m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	//m_wndToolBar.EnableDocking(CBRS_ALIGN_TOP);
	//EnableDocking(CBRS_ALIGN_TOP);
	//DockPane(&m_wndMenuBar);
	//DockPane(&m_wndToolBar);


	// enable Visual Studio 2005 style docking window behavior
	CDockingManager::SetDockingMode(DT_SMART);
	// enable Visual Studio 2005 style docking window auto-hide behavior
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// Enable toolbar and docking window menu replacement
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// enable quick (Alt+drag) toolbar customization
	CMFCToolBar::EnableQuickCustomization();
	*/
	
	
	
	m_pDuiToolBar->SetSkinFile(_T("toolbar.xml"));
	m_pDuiToolBar->Create(*this, NULL, UI_WNDSTYLE_CHILD, 0, 0, 0);
	m_pDuiToolBar->ShowWindow(true);
	g_pDuiToolBar=m_pDuiToolBar;
	m_pDuiStatusBar->SetSkinFile(_T("statusbar.xml"));
	m_pDuiStatusBar->Create(*this,NULL,UI_WNDSTYLE_CHILD, 0, 0, 0);
	m_pDuiStatusBar->ShowWindow(true);
	g_pDuiStatusBar=m_pDuiStatusBar;
	
	//m_pDuiOperateWnd->SetSkinFile(_T("OperateWnd-new.xml"));
	//m_pDuiOperateWnd->Create(*this,NULL,UI_WNDSTYLE_CHILD, 0, 0, 0);
	//m_pDuiOperateWnd->ShowWindow(true);
	

	
	//m_pDuiMonitorInfo->SetSkinFile(_T("MonitorInfo.xml"));
	//m_pDuiMonitorInfo->Create(*this,NULL,UI_WNDSTYLE_CHILD, 0, 0, 0);
	//m_pDuiMonitorInfo->ShowWindow(true);
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs))
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.hMenu=NULL;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame message handlers

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// forward focus to the view window
	m_wndRuler.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// let the view have first crack at the command
	if (m_wndRuler.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// otherwise, do default handling
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMainFrame::OnViewCustomize()
{
	//CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* scan menus */);
	//pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}


void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWndEx::OnSize(nType, cx, cy);
	
	
	//::MoveWindow(*m_pDuiOperateWnd,cx-200,60,200,cy-60-20,TRUE);
	if(m_wndRuler.GetSafeHwnd())
	{
		m_wndRuler.MoveWindow(200,60,cx-200,cy-60-20,TRUE);
	}
	/*
	if(m_wndMonitorType.GetSafeHwnd())
	{
		m_wndMonitorType.MoveWindow(0,60,200,cy-20-60);
	}
	*/
	if(m_paneMonitorType.GetSafeHwnd())
	{
		CRect pos;
		pos.left=0;
		pos.right=200;
		pos.bottom=cy-20;
		pos.top=60;

		m_paneMonitorType.MoveWindow(pos);
	}
	
	::MoveWindow(*m_pDuiToolBar,0,0,cx,60,TRUE);
	::MoveWindow(*m_pDuiStatusBar,0,cy-20,cx,20,TRUE);
}

void CMainFrame::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	if(g_pDesignerCtlWnd && g_pDesignerCtlWnd->GetSafeHwnd())
	{
		g_pDesignerCtlWnd->OnKeyDown(nChar,nRepCnt,nFlags);
	}
	CFrameWndEx::OnKeyDown(nChar, nRepCnt, nFlags);
}


BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(WM_KEYDOWN == pMsg->message)
	{
		OnKeyDown(pMsg->wParam,LOWORD(pMsg->lParam),HIWORD(pMsg->lParam));
	}
	return CFrameWndEx::PreTranslateMessage(pMsg);
}
