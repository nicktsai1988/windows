#pragma once
#include "list"
#include "Utils.h"

// MonitorTypeWnd

class CMonitorTypeWnd : public CWnd
{
	DECLARE_DYNAMIC(CMonitorTypeWnd)

public:
	enum {LEFT_RIGHT_INTERVAL=10,INTERVAL_HEIGHT=20};
	CMonitorTypeWnd();
	virtual ~CMonitorTypeWnd();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	void SetTypeDeletable(bool deletable);
	const MonitorTypeElement* GetDefaultType()const ;
	const std::list<MonitorTypeElement*>& GetTypes()const;
	
	virtual void OnDraw(CDC* pDC);

protected:
	DECLARE_MESSAGE_MAP()
private:
	MonitorTypeElement* GetTypeInPoint(CPoint& point);

	double CalSizeRatio(const MonitorTypeElement* type);
	void UpdateRectSize();
	void UpdateRectView();
	void UpdateScrollBar();

	COLORREF m_BkColor;
	std::list<MonitorTypeElement*> m_listMonitorTypes;
	int m_iScrollHeight;
	int m_iyoldpos;
	CPoint m_pointContexMenu;
	MonitorTypeElement* m_pMaxMonitorType;
	MonitorTypeElement* m_pDefaultMonitorType;
	CFont m_fontMonitorType;
	CPen m_penMonitorType;
	bool m_bTypeDeletable;
public:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint point);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//user-defined message handles
	afx_msg void OnAddType();
	afx_msg void OnDeleteType();
	afx_msg void OnEditType();
	afx_msg void OnSetAsDefault();
};


