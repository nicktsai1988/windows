#include "stdafx.h"
#include "RotateRect.h"
#include <cmath>

RotateRect::RotateRect():m_sizeScrollOffset(0,0)
{
	myViewAngle=0.0;
	myViewCenter.x=myViewCenter.y=0;
	for(int i=0;i<4;++i)
	{
		myViewPoints[i].x=0;
		myViewPoints[i].y=0;
		myViewExtendPoints[i].x=0;
		myViewExtendPoints[i].y=0;
		myViewHandlePoints[i].x=0;
		myViewHandlePoints[i].y=0;
	}
	myfillcolor=RGB(0,0,0);
	myedgecolor=RGB(0,0,0);

	myhandlefillcolor=RGB(255,255,255);
	myhandleedgecolor=RGB(192,192,192);
	lefttopfillcolor=RGB(0,0,0);
	isactive=false;
	m_pInfo=NULL;
}
RotateRect::~RotateRect()
{
	myViewRgn.DeleteObject();
	myViewExtendRgn.DeleteObject();
	for(int i=0;i<4;++i)
	{
		myViewHandleRgn[i].DeleteObject();
	}
}

RotateRect::RotateRect(const RotateRect& other)
{
	Copy(other);
}
RotateRect& RotateRect::operator=(const RotateRect& other)
{
	if(this == &other)
		return *this;
	Copy(other);
	return *this;
}
void RotateRect::Init(const MonitorInfo* pInfo,double scale)
{
	m_pInfo=pInfo;
	m_dScale=scale;
	myfillcolor=pInfo->m_type.m_nBkColor;
}
void RotateRect::DrawSelf(CDC* pDC)
{
	Update();
	CBrush brush;
	CPen pen;
	CPoint now[4];
	for(int i=0;i<4;++i)
	{
		now[i]=PointRotate(myViewPoints[i]);
	}
	brush.CreateSolidBrush(myfillcolor);
	pDC->SelectObject(&brush);

	pDC->BeginPath();
	pDC->MoveTo(now[0].x,now[0].y);
	for(int i=1;i<4;++i)
	{
		pDC->LineTo(now[i].x,now[i].y);
	}
	pDC->LineTo(now[0].x,now[0].y);
	pDC->EndPath();
	pDC->FillPath();
	
	pen.CreatePen(PS_INSIDEFRAME,1,myedgecolor);
	pDC->SelectObject(&pen);
	pDC->MoveTo(now[0].x,now[0].y);
	for(int i=1;i<4;++i)
	{
		pDC->LineTo(now[i].x,now[i].y);
	}
	pDC->LineTo(now[0].x,now[0].y);
	pen.DeleteObject();
	brush.DeleteObject();

	//if(isactive)
	//	DrawHandle(pDC);
}

void RotateRect::DrawHandle(CDC *pDC)
{
	Update();
	CBrush brush;
	CPen pen;
	CPoint handles[4];
	for(int i=0;i<4;++i)
	{
		handles[i]=PointRotate(myViewHandlePoints[i]);
		myViewHandleRgn[i].DeleteObject();
	}

	myViewHandleRgn[0].CreateEllipticRgn(handles[0].x-ROTATE_RECT_HANDLE_RADIOUS,handles[0].y-ROTATE_RECT_HANDLE_RADIOUS,
						  handles[0].x+ROTATE_RECT_HANDLE_RADIOUS+1,handles[0].y+ROTATE_RECT_HANDLE_RADIOUS+1);
	brush.CreateSolidBrush(lefttopfillcolor);
	pDC->FillRgn(&myViewHandleRgn[0],&brush);
	brush.DeleteObject();

	brush.CreateSolidBrush(myhandlefillcolor);
	for(int i=1;i<4;++i)
	{
		myViewHandleRgn[i].CreateEllipticRgn(handles[i].x-ROTATE_RECT_HANDLE_RADIOUS,handles[i].y-ROTATE_RECT_HANDLE_RADIOUS,
			handles[i].x+ROTATE_RECT_HANDLE_RADIOUS+1,handles[i].y+ROTATE_RECT_HANDLE_RADIOUS+1);
		pDC->FillRgn(&myViewHandleRgn[i],&brush);
	}
	brush.DeleteObject();
}
/*
void RotateRect::Move(int x,int y)
{
	mycenter.x+=x;
	mycenter.y+=y;
	for(int i=0;i<4;++i)
	{
		myViewPoints[i].x+=x;
		myViewPoints[i].y+=y;
		myViewExtendPoints[i].x+=x;
		myViewExtendPoints[i].y+=y;
		myViewHandlePoints[i].x+=x;
		myViewHandlePoints[i].y+=y;
	}
}
void RotateRect::MoveTo(const CPoint& pos)
{
	long x=pos.x-mycenter.x;
	long y=pos.y-mycenter.y;
	Move(x,y);
}
void RotateRect::Rotate(double angle)
{
	myangle+=angle;
	if(myangle >= 360)
		myangle -= 360;
	if(myangle <= -360)
		myangle += 360;
}

*/
CRgn* RotateRect::GetMyViewClientRgn()
{
	Update();
	CPoint now[4];
	for(int i=0;i<4;++i)
	{
		now[i]=PointRotate(myViewPoints[i]);
	}
	myViewRgn.DeleteObject();
	myViewRgn.CreatePolygonRgn(now,4,ALTERNATE);
	return &myViewRgn;
}

CRgn* RotateRect::GetMyExtendRgn()
{
	Update();
	CPoint extend[4];
	for(int i=0;i<4;++i)
	{
		extend[i]=PointRotate(myViewExtendPoints[i]);
	}
	myViewExtendRgn.DeleteObject();
	myViewExtendRgn.CreatePolygonRgn(extend,4,ALTERNATE);
	return &myViewExtendRgn;
}

CRgn* RotateRect::GetMyViewHandleRgn()
{
	Update();
	CPoint handles[4];
	for(int i=0;i<4;++i)
	{
		handles[i]=PointRotate(myViewHandlePoints[i]);
		myViewHandleRgn[i].DeleteObject();
	}
	for(int i=0;i<4;++i)
	{
		myViewHandleRgn[i].CreateEllipticRgn(handles[i].x-ROTATE_RECT_HANDLE_RADIOUS,handles[i].y-ROTATE_RECT_HANDLE_RADIOUS,
			handles[i].x+ROTATE_RECT_HANDLE_RADIOUS+1,handles[i].y+ROTATE_RECT_HANDLE_RADIOUS+1);
	}
	return &myViewHandleRgn[0];
}

const CPoint* RotateRect::GetMyViewCenter()
{
	Update();
	return &myViewCenter;
}


void RotateRect::SetAvtive(bool active)
{
	isactive=active;
	if(active)
		myfillcolor=RGB(0,255,0);
	else
		myfillcolor=m_pInfo->m_type.m_nBkColor;
}


#define STEP (3.1415926/180)
CPoint RotateRect::PointRotate(const CPoint& previous)
{
	CPoint ret;
	ret.x=(previous.y-myViewCenter.y)*sin(myViewAngle*STEP)+(previous.x-myViewCenter.x)*cos(myViewAngle*STEP)+myViewCenter.x;
	ret.y=(previous.y-myViewCenter.y)*cos(myViewAngle*STEP)-(previous.x-myViewCenter.x)*sin(myViewAngle*STEP)+myViewCenter.y;
	return ret;
}
void RotateRect::Copy(const RotateRect& other)
{
	myViewCenter=other.myViewCenter;
	myViewAngle=other.myViewAngle;
	myfillcolor=other.myfillcolor;
	isactive=other.isactive;
	myedgecolor=other.myedgecolor;
	myhandlefillcolor=other.myhandlefillcolor;
	myhandleedgecolor=other.myhandleedgecolor;
	lefttopfillcolor=other.lefttopfillcolor;
	for(int i=0;i<4;++i)
	{
		myViewPoints[i]=other.myViewPoints[i];
		myViewExtendPoints[i]=other.myViewExtendPoints[i];
		myViewHandlePoints[i]=other.myViewHandlePoints[i];
	}
	myViewRgn.DeleteObject();
	myViewExtendRgn.DeleteObject();
	for(UINT i=0;i<4;++i)
	{
		myViewHandleRgn[i].DeleteObject();
	}
}

void RotateRect::Update()
{
	for(UINT i=0;i<4;++i)
	{
		myViewPoints[i].x=static_cast<int>(m_pInfo->m_posNew.m_Points[i].x*m_dScale+0.5);
		myViewPoints[i].y=static_cast<int>(m_pInfo->m_posNew.m_Points[i].y*m_dScale+0.5);

		myViewPoints[i] += m_sizeOrigOffset;
		//myViewPoints[i] -= m_sizeScrollOffset;

		myViewExtendPoints[0].x= myViewPoints[0].x-ROTATE_RECT_EXTEND_OFFSET;
		myViewExtendPoints[0].y= myViewPoints[0].y-ROTATE_RECT_EXTEND_OFFSET;
		myViewExtendPoints[1].x= myViewPoints[1].x+ROTATE_RECT_EXTEND_OFFSET;
		myViewExtendPoints[1].y= myViewPoints[1].y-ROTATE_RECT_EXTEND_OFFSET;
		myViewExtendPoints[2].x= myViewPoints[2].x+ROTATE_RECT_EXTEND_OFFSET;
		myViewExtendPoints[2].y= myViewPoints[2].y+ROTATE_RECT_EXTEND_OFFSET;
		myViewExtendPoints[3].x= myViewPoints[3].x-ROTATE_RECT_EXTEND_OFFSET;
		myViewExtendPoints[3].y= myViewPoints[3].y+ROTATE_RECT_EXTEND_OFFSET;

		myViewHandlePoints[0].x= (myViewPoints[0].x-ROTATE_RECT_HANDLE_OFFSET);
		myViewHandlePoints[0].y= (myViewPoints[0].y-ROTATE_RECT_HANDLE_OFFSET);
		myViewHandlePoints[1].x= (myViewPoints[1].x+ROTATE_RECT_HANDLE_OFFSET);
		myViewHandlePoints[1].y= (myViewPoints[1].y-ROTATE_RECT_HANDLE_OFFSET);
		myViewHandlePoints[2].x= (myViewPoints[2].x+ROTATE_RECT_HANDLE_OFFSET);
		myViewHandlePoints[2].y= (myViewPoints[2].y+ROTATE_RECT_HANDLE_OFFSET);
		myViewHandlePoints[3].x= (myViewPoints[3].x-ROTATE_RECT_HANDLE_OFFSET);
		myViewHandlePoints[3].y= (myViewPoints[3].y+ROTATE_RECT_HANDLE_OFFSET);
	}
	myViewCenter.x=static_cast<int>(m_pInfo->m_posNew.m_Center.x*m_dScale+0.5);
	myViewCenter.y=static_cast<int>(m_pInfo->m_posNew.m_Center.y*m_dScale+0.5);
	
	myViewCenter += m_sizeOrigOffset;
	//myViewCenter -= m_sizeScrollOffset;

	myViewAngle=m_pInfo->m_posNew.m_dRotation;
}

void RotateRect::SetViewScale(double dScale)
{
	m_dScale=dScale;
}

void RotateRect::SetScrollOffset(const CSize& offset)
{
	m_sizeScrollOffset=offset;
}

void RotateRect::HighlightEdge()
{

}

void RotateRect::SetOrigOffset(const CSize& orig)
{
	m_sizeOrigOffset=orig;
}