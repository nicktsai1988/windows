#pragma once
#include "MonitorTypeWnd.h"
#include "MonitorTypeDialog.h"

// CMonitorTypePane

class CMonitorTypePane : public CDockablePane
{
	DECLARE_DYNAMIC(CMonitorTypePane)

public:
	CMonitorTypePane();
	virtual ~CMonitorTypePane();
	virtual BOOL FloatPane(CRect rectFloat, AFX_DOCK_METHOD dockMethod = DM_UNKNOWN, bool bShow = true);
	CMonitorTypeWnd* GetMonitorTypeWnd();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);

private:
	CMonitorTypeWnd m_wndMonitorType;
	//CMonitorTypeDialog m_dialogMonitorType;
};


