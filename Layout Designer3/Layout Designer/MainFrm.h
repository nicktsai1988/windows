
// MainFrm.h : interface of the CMainFrame class
//

#pragma once
#include "DesignerCtlWnd.h"
#include "DuiToolbar.h"
#include "DuiStatusBar.h"
#include "DuiOperateWnd.h"
#include "RulerCtlWnd.h"
#include "MonitorTypePane.h"
#include "MonitorTypeWnd.h"

class CMainFrame : public CFrameWndEx
{
	
public:
	CMainFrame();
protected: 
	DECLARE_DYNAMIC(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	//CMFCMenuBar       m_wndMenuBar;
	//CMFCToolBar       m_wndToolBar;
	//CMFCStatusBar     m_wndStatusBar;
	CRulerCtlWnd    m_wndRuler;
	//CMonitorTypeWnd m_wndMonitorType;
	CMonitorTypePane m_paneMonitorType;
	DuiToolBar* m_pDuiToolBar;
	DuiStatusBar* m_pDuiStatusBar;
	

	

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


