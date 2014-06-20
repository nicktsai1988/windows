#pragma once
#include "stdafx.h"
class Canvas
{
public:
	enum{MIN_GRID_VIEW_SIZE=40};
	Canvas(CWnd *pParent);
	~Canvas(void);
	void SetSize(UINT width,UINT height);
	void SetColor(COLORREF nBkColor,COLORREF nGridColor);
	void SetScrollViewOffset(const CSize& offset);
	void SetViewOrigOffset(const CSize& offset);
	const CSize& GetSize();
	void SetViewScale(double dScale);
	void Draw(CDC *pDC);

	const CRect& GetCanvasViewRect();
	double GetGridViewSize();
	const CPoint& GetMonitorViewOrig(); 

private:
	void UpdateCanvas();
	void DrawCanvas(CDC* pDC);
	void DrawGrid(CDC* pDC);

	int CalGreatestCommonDivisor(int m,int n);
	void CalGridSize();
	
	CWnd* m_pWndParent;
	CRect m_CanvasViewRect;
	CSize m_CanvasViewSize;
	CPoint m_MonitorViewOrig;

	CSize m_sizeViewOrigOffset;
	CSize m_sizeScrollViewOffset;

	COLORREF m_nBackColor;
	COLORREF m_nGridColor;
	double m_dGridSize;
	double m_dScale;
};

