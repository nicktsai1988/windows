
// ChildView.h : interface of the CDesignerCtlWnd class
//


#pragma once
#include "MonitorManager.h"
#include "Utils.h"
#include "Canvas.h"
#include <vector>
#include <list>

// CDesignerCtlWnd window

#define ID_MONITOR_TYPEBEGIN 1000
#define ID_MONITOR_TYPEEND 1064

#define WM_USER_DETECT WM_USER+1
#define WM_USER_OPEN	WM_USER+2
#define WM_USER_SAVE WM_USER+3
#define WM_USER_SAVEAS WM_USER+4
#define WM_USER_REVERT WM_USER+5
#define WM_USER_UNDO WM_USER+6
#define WM_USER_REDO WM_USER+7
#define WM_USER_SELALL WM_USER+8
#define WM_USER_ORIGVIEW WM_USER+9
#define WM_USER_DESIGNVIEW WM_USER+10
#define WM_USER_ZOOM WM_USER+11
#define WM_USER_HELP WM_USER+12


struct MonitorTypeIndex
{
	UINT m_nIndex;
	const MonitorTypeElement* m_pElement;
};

class CDesignerCtlWnd : public CWnd
{
// Construction
public:
	CDesignerCtlWnd(CWnd* parent);
	virtual ~CDesignerCtlWnd();
// Attributes
public:
	void SetWndScrollSize(const CSize& size);
	const CSize& GetWndScrollSize();
	const CRect& GetCanvasViewRect();
	double GetGridViewSize();
	CPoint GetMonitorViewOrig();
	double GetViewScale();
	void Reset();
	void MonitorsUpdateType(const MonitorTypeElement* pTypeElement);

// Operations
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnDraw(CDC* pDC);
	virtual void OnPrepareDC(CDC* pDC);

// Overrides

// Implementation

	// Generated message map functions
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg LRESULT OnDetect(WPARAM wp,LPARAM lp);
	afx_msg LRESULT OnOpen(WPARAM wp,LPARAM lp);
	afx_msg LRESULT OnSave(WPARAM wp,LPARAM lp);
	afx_msg LRESULT OnSaveAs(WPARAM wp,LPARAM lp);
	afx_msg LRESULT OnRevert(WPARAM wp,LPARAM lp);
	afx_msg LRESULT OnUndo(WPARAM wp,LPARAM lp);
	afx_msg LRESULT OnRedo(WPARAM wp,LPARAM lp);
	afx_msg LRESULT OnSelectAll(WPARAM wp,LPARAM lp);
	afx_msg LRESULT OnDesignView(WPARAM wp,LPARAM lp);
	afx_msg LRESULT OnOriginalView(WPARAM wp,LPARAM lp);
	afx_msg LRESULT OnZoom(WPARAM wp,LPARAM lp);
	afx_msg LRESULT OnHelp(WPARAM wp,LPARAM lp);
private:

	bool MatchTypeAndTransPos();
	bool MatchMonitorType();
	bool TransformMonitorPosition();
	void TransformInContainer(const CRect& container,const CPoint& lastNewBottomRight,std::list<MonitorInfo*>& infoList);

	CRect CalContainerRectBeforeTransform();
	CRect CalContainerRectAfterTransform();

	void CalMonitorNewPos(MonitorInfo& info);

	void CalAutoViewScale();

	BOOL LoadMonitorInfo();
	void DoZoom(double dZoomValue);
	void InitMenuItem(CMenu& menu,const CMonitorControler* pControler);
	void InitViewer();
	void UpdateScrollBar(int cx,int cy);
	//data
	int m_ixoldpos,m_iyoldpos;
	CSize m_WndScrollSize;

	CWnd* m_pParentWnd;
	CMonitorControler* m_pControlerInCursor;

	MonitorManager m_monitorManager;
	std::vector<CRect> m_aOrigMonitorRects;
	std::vector<MonitorInfo> m_aMonitorInfo;
	std::vector<MonitorTypeIndex> m_aTypeIndex;
	
	Canvas m_Canvas;
	int m_iViewZoomIndex;
	double m_aViewZoom[5];
	COLORREF m_BkColor;
	COLORREF m_GridLineColor;
	CString m_strSrvAddress,m_strSrvPort;
public:
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint point);
	afx_msg void OnChangeType(UINT uID);
	afx_msg void OnEditMonitor();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

