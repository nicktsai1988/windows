#include "StdAfx.h"
#include "MonitorControler.h"


CMonitorControler::CMonitorControler(MonitorInfo* pInfo,double scale)
{
	m_pMonitorInfo=pInfo;
	m_RRectViewer.Init(m_pMonitorInfo,scale);
}


CMonitorControler::~CMonitorControler(void)
{
}
void CMonitorControler::Draw(CDC* pDC)
{
	m_RRectViewer.DrawSelf(pDC);
}
void CMonitorControler::DrawHandle(CDC* pDC)
{
	m_RRectViewer.DrawHandle(pDC);
}
void CMonitorControler::HightLightEdge()
{
	m_RRectViewer.HighlightEdge();
}
void CMonitorControler::Move(int x,int y)
{
	m_pMonitorInfo->m_posNew.m_Center.x+=x;
	m_pMonitorInfo->m_posNew.m_Center.y+=y;
	for(UINT i=0;i<4;++i)
	{
		m_pMonitorInfo->m_posNew.m_Points[i].x+=x;
		m_pMonitorInfo->m_posNew.m_Points[i].y+=y;
	}
}

void CMonitorControler::MoveTo(const CPoint& pos)
{
	int x=pos.x-m_pMonitorInfo->m_posNew.m_Center.x;
	int y=pos.y-m_pMonitorInfo->m_posNew.m_Center.y;
	Move(x,y);
}
void CMonitorControler::MoveTo(int x,int y)
{
	int xOffset=x-m_pMonitorInfo->m_posNew.m_Center.x;
	int yOffset=y-m_pMonitorInfo->m_posNew.m_Center.y;
	Move(xOffset,yOffset);
}

void CMonitorControler::Rotate(double angle)
{
	angle += m_pMonitorInfo->m_posNew.m_dRotation;
	if(angle >= 360.0f)
	{
		angle -=360;
	}
	else
	{
		if(angle <= -360.0f)
		{
			angle +=360;
		}
	}
	m_pMonitorInfo->m_posNew.m_dRotation=angle;
}
void CMonitorControler::RotateTo(double newangle)
{
	double angle= newangle-m_pMonitorInfo->m_posNew.m_dRotation;
	Rotate(angle);
}

void CMonitorControler::SetActive(bool active)
{
	m_RRectViewer.SetAvtive(active);
}

void CMonitorControler::SetScrollViewOffset(const CSize& offset)
{
	m_RRectViewer.SetScrollOffset(offset);
}

void CMonitorControler::SetViewOrigOffset(const CSize& offset)
{
	m_RRectViewer.SetOrigOffset(offset);
}

CRgn* CMonitorControler::GetMyViewClientRgn()
{
	return m_RRectViewer.GetMyViewClientRgn();
}
CRgn* CMonitorControler::GetMyViewHandleRgn()
{
	return m_RRectViewer.GetMyViewHandleRgn();
}

const CPoint* CMonitorControler::GetMyViewCenter()
{
	return m_RRectViewer.GetMyViewCenter();
}
const MonitorInfo* CMonitorControler::GetMyMonitorInfo()const
{
	return m_pMonitorInfo;
}
bool CMonitorControler::PtInViewer(const CPoint& pt)
{
	return m_RRectViewer.GetMyViewClientRgn()->PtInRegion(pt);
}

void CMonitorControler::ChangeType(const MonitorTypeElement* pTypeElement)
{
	m_pMonitorInfo->m_pTypeElement=pTypeElement;
	m_pMonitorInfo->m_type=pTypeElement->m_type;
}

void CMonitorControler::UpdateType(const MonitorTypeElement* pTypeElement)
{
	if(pTypeElement == m_pMonitorInfo->m_pTypeElement)
	{
		m_pMonitorInfo->m_type=pTypeElement->m_type;
	}
}

void CMonitorControler::SetViewScale(double dViewScale)
{
	m_RRectViewer.SetViewScale(dViewScale);
}
