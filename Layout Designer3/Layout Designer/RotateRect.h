#pragma once
#include "stdafx.h"
#include "Utils.h"
class RotateRect
{
public:
	enum POSITION{POS_NULL,POS_HANDLE,POS_RECT};
	enum{ROTATE_RECT_HANDLE_OFFSET=4,ROTATE_RECT_HANDLE_RADIOUS=4,ROTATE_RECT_EXTEND_OFFSET=(5*2)};
	RotateRect();
	~RotateRect();
	RotateRect(const RotateRect& other);
	RotateRect& operator=(const RotateRect& other);
	void Init(const MonitorInfo* pInfo,double scale);
	void DrawSelf(CDC *pDC);
	void DrawHandle(CDC *pDC);
	void HighlightEdge();
	const CPoint* GetMyViewCenter();
	CRgn* GetMyViewClientRgn();
	CRgn* GetMyExtendRgn();
	CRgn* GetMyViewHandleRgn();

	void SetAvtive(bool active);
	void SetViewScale(double dScale);
	void SetScrollOffset(const CSize& offset);
	void SetOrigOffset(const CSize& offset);
	//void SetMyColor(COLORREF fillcolor,COLORREF edgecolor);
	//void ResetAngle();
private:
	CPoint PointRotate(const CPoint& previous);
	void Copy(const RotateRect& other);
	void Update();
	CPoint myViewCenter;
	CPoint myViewPoints[4];
	CPoint myViewExtendPoints[4];
	CPoint myViewHandlePoints[4];
	double myViewAngle;
	CRgn myViewRgn;
	CRgn myViewExtendRgn;

	CRgn myViewHandleRgn[4];

	COLORREF lefttopfillcolor;
	COLORREF myfillcolor;
	COLORREF myedgecolor;

	COLORREF myhandlefillcolor;
	COLORREF myhandleedgecolor;

	bool isactive;
	const MonitorInfo* m_pInfo;
	double m_dScale;
	CSize m_sizeScrollOffset;
	CSize m_sizeOrigOffset;
};