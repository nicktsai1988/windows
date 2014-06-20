// MonitorTypePane.cpp : implementation file
//

#include "stdafx.h"
#include "Layout Designer.h"
#include "MonitorTypePane.h"


// CMonitorTypePane

IMPLEMENT_DYNAMIC(CMonitorTypePane, CDockablePane)

CMonitorTypePane::CMonitorTypePane()
{

}

CMonitorTypePane::~CMonitorTypePane()
{
}


BEGIN_MESSAGE_MAP(CMonitorTypePane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()



// CMonitorTypePane message handlers




int CMonitorTypePane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if( !m_wndMonitorType.Create(NULL, NULL,  WS_CHILD | WS_VISIBLE | WS_VSCROLL, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST+3, NULL))
	{
		TRACE0("Failed to create monitor type window\n");
		return -1;
	}
	
	// TODO:  Add your specialized creation code here
	//m_dialogMonitorType.Create(CMonitorTypeDialog::IDD,this);
	//m_dialogMonitorType.ShowWindow(SW_SHOW);
	return 0;
}


void CMonitorTypePane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	
	if(m_wndMonitorType.GetSafeHwnd())
	{
		m_wndMonitorType.MoveWindow(0,0,cx,cy);
	}
	
	/*
	if(m_dialogMonitorType.GetSafeHwnd())
	{
		m_dialogMonitorType.MoveWindow(0,0,cx,cy);
	}
	*/
	// TODO: Add your message handler code here
	
}

BOOL CMonitorTypePane::FloatPane(CRect rectFloat, AFX_DOCK_METHOD dockMethod , bool bShow)
{
	return FALSE;
}
void CMonitorTypePane::OnContextMenu(CWnd* pWnd, CPoint point)
{

}

CMonitorTypeWnd* CMonitorTypePane::GetMonitorTypeWnd()
{
	return &m_wndMonitorType;
}
