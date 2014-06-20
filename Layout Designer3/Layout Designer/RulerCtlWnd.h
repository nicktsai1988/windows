#pragma once



typedef double(*OperateFunc)(double x,double y);

// CRulerCtlWnd
#include "DesignerCtlWnd.h"

class CRulerCtlWnd : public CWnd
{
	DECLARE_DYNAMIC(CRulerCtlWnd)

public:
	enum{HORIZON_RULER_TOP=1,VERTICAL_RULER_LEFT=1,HORIZON_RULER_BOTTOM=31,VERTICAL_RULER_RIGHT=31};
	CRulerCtlWnd();
	virtual ~CRulerCtlWnd();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);


public:
	void InitRulerFont();

	void DrawRulerCursor(int x,int y,bool drawCursor=true);

	//void DrawHorizonRulerCursor(int x,bool drawCursor=true);
	//void DrawVerticalRulerCursor(int y,bool drawCursor=true);
	void Update();
	void SetBkColor(int color);
	void SetRulerBkColor(int HorizonColor,int VerticalColor);
	static double add(double x,double y);
	static double minus(double x,double y);
protected:
	void DrawHorizonMiniMark(CDC* pDC,int x,double width,int min ,int max,OperateFunc pFunc);
	void DrawVerticalMiniMark(CDC* pDC,int y,double width,int min,int max,OperateFunc pFunc);
	void DrawHorizonRuler(CDC* pDC);
	void DrawVerticalRuler(CDC* pDC);
	virtual void OnDraw(CDC *pDC);
	virtual void OnPrepareDC(CDC *pDC);
	

protected:
	DECLARE_MESSAGE_MAP()


private:

	CFont m_RulerFont;
	float m_Zoom;
	COLORREF m_BkColor;
	COLORREF m_HoriRulerBkColor,m_VertRulerBkColor;
	CRect m_RectHorizonRuler,m_RectVerticalRuler;
	CDesignerCtlWnd m_wndDesigner;
	
public:
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


