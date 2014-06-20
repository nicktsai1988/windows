#include "stdafx.h"
#include "MonitorManager.h"
#include "RulerCtlWnd.h"
MonitorManager::MonitorManager(CWnd* parent)
{
	m_pWndParent=parent;
	m_dViewScale=1.0;
}
MonitorManager::~MonitorManager()
{
	for(UINT i = 0;i < m_aMonitorsControler.size();++i)
	{
		delete m_aMonitorsControler[i];
	}
}
void MonitorManager::Init()
{
	m_aMonitorsControler.clear();
	m_aSelectedControler.clear();
}
void MonitorManager::AddMonitor(MonitorInfo* pInfo)
{
	m_aMonitorsControler.push_back(new CMonitorControler(pInfo,m_dViewScale));
}

void MonitorManager::Reset()
{
	for(size_t i=0;i<m_aMonitorsControler.size();++i)
	{
		delete m_aMonitorsControler[i];
	}
	m_aMonitorsControler.clear();
	m_aSelectedControler.clear();
	m_ePos=RotateRect::POS_NULL;
	m_bLButtonDown=false;
}


void MonitorManager::SetViewScale(double scale)
{
	m_dViewScale=scale;
	for(size_t i=0;i<m_aMonitorsControler.size();++i)
	{
		m_aMonitorsControler[i]->SetViewScale(scale);
	}

}


void MonitorManager::OnDraw(CDC* pDC)
{
	for(UINT i=0;i<m_aMonitorsControler.size();++i)
	{
		m_aMonitorsControler[i]->Draw(pDC);
	}
	for(UINT i=0;i<m_aSelectedControler.size();++i)
	{
		m_aSelectedControler[i]->Draw(pDC);
		m_aSelectedControler[i]->DrawHandle(pDC);
	}
}

void MonitorManager::SetScrollViewOffset(const CSize& offset)
{
	for(UINT i=0;i<m_aMonitorsControler.size();++i)
	{
		m_aMonitorsControler[i]->SetScrollViewOffset(offset);
	}
}
void MonitorManager::SetViewOrigOffset(const CSize& offset)
{
	for(UINT i=0;i<m_aMonitorsControler.size();++i)
	{
		m_aMonitorsControler[i]->SetViewOrigOffset(offset);
	}
}

CMonitorControler* MonitorManager::GetMonitorControlerInPt(const CPoint& pt)
{
	for(size_t i=0;i<m_aMonitorsControler.size();++i)
	{
		if(m_aMonitorsControler[i]->PtInViewer(pt))
			return m_aMonitorsControler[i];
	}
	return NULL;
}



void MonitorManager::MonitorsUpdateType(const MonitorTypeElement* pTypeElement)
{
	for(size_t i=0;i<m_aMonitorsControler.size();++i)
	{
		m_aMonitorsControler[i]->UpdateType(pTypeElement);
	}
}

void MonitorManager::OnMouseMove(UINT nFlags, CPoint point,CWnd* pRulerWnd)
{
	if(m_aSelectedControler.size() == 0)
	{
		return;
	}
	CRgn* handlergn=NULL;
	HCURSOR cursor=AfxGetApp()->LoadStandardCursor(IDC_ARROW);
	for(size_t i=0;i<m_aSelectedControler.size();++i)
	{
		handlergn=m_aSelectedControler[i]->GetMyViewHandleRgn();
		for(int j=0;j<4;++j)
		{
			if(handlergn[j].PtInRegion(point))
			{
				cursor=AfxGetApp()->LoadStandardCursor(IDC_CROSS);
				break;
			}
		}
	}
	SetCursor(cursor);

	if(m_bLButtonDown)
	{
		ASSERT(m_pCurrentControler);
		if(m_ePos == RotateRect::POS_RECT)
		{
			CSize offset=point-m_PointRectPrev;
			offset.cx /= m_dViewScale;
			offset.cy /= m_dViewScale;

			for(UINT i=0;i<m_aSelectedControler.size();++i)
			{
				m_aSelectedControler[i]->Move(offset.cx,offset.cy);
			}
			if(m_aSelectedControler.size() == 1)
			{
				((CRulerCtlWnd*)pRulerWnd)->DrawRulerCursor(m_aSelectedControler[0]->GetMyViewCenter()->x+CRulerCtlWnd::VERTICAL_RULER_RIGHT,
															m_aSelectedControler[0]->GetMyViewCenter()->y+CRulerCtlWnd::HORIZON_RULER_BOTTOM);

				const MonitorInfo* monitorinfo=m_aSelectedControler[0]->GetMyMonitorInfo();
				SetMonitorText(monitorinfo);
			}
			else
			{
				SetMonitorText(_T("N/A"));
			}
			SetStatusText(_T("Moving"));
			m_PointRectPrev=point;
		}
		else
		{
			if(m_ePos == RotateRect::POS_HANDLE)
			{
				
				double angle= GetRoundOffValue(CalRotateAngel(*(m_pCurrentControler->GetMyViewCenter()),point,m_PointHandlePrev));
				
				for(UINT i=0;i<m_aSelectedControler.size();++i)
				{
					m_aSelectedControler[i]->Rotate(angle);
				}
				

				if(m_aSelectedControler.size() == 1)
				{
					const MonitorInfo* monitorinfo=m_aSelectedControler[0]->GetMyMonitorInfo();
					SetMonitorText(monitorinfo);
				}
				else
				{
					SetMonitorText(_T("N/A"));
				}
				SetStatusText(_T("Rotating"));
				m_PointHandlePrev=point;
			}
			else
			{
				AfxMessageBox(_T("Left Key down error"));
			}
		}
	}
}
void MonitorManager::OnLButtonDown(UINT nFlags, CPoint point,CWnd* pRulerWnd)
{
	//m_pWndParent->SetCapture();
	CMonitorControler* current=NULL;
	CRgn *rgn;
	bool found=false,inselected=false;
	for(UINT i=0;(i<m_aMonitorsControler.size()) && (!found); ++i)
	{
		rgn=m_aMonitorsControler[i]->GetMyViewClientRgn();
		if(rgn->PtInRegion(point))
		{
			current = m_aMonitorsControler[i];
			m_pCurrentControler=current;
			m_bLButtonDown=true;
			m_ePos=RotateRect::POS_RECT;
			m_PointRectPrev=point;
			found=true;
		}
		else
		{
			rgn=m_aMonitorsControler[i]->GetMyViewHandleRgn();
			for(UINT j=0;(j<4) && (!found);++j)
			{
				if(rgn[j].PtInRegion(point))
				{
					current = m_aMonitorsControler[i];
					m_pCurrentControler=current;
					m_bLButtonDown=true;
					m_ePos=RotateRect::POS_HANDLE;
					m_PointHandlePrev=point;
					found=true;
				}
			}
		}
	}

	if(current != NULL)
	{
		std::vector<CMonitorControler*>::iterator it,pos;
		for(it= m_aSelectedControler.begin();it != m_aSelectedControler.end();++it)
		{
			if(current == *it)
			{
				inselected=true;
				pos=it;
			}
		}
		bool bCtrlDown=(GetKeyState(VK_CONTROL)<0)?true:false;
		if(bCtrlDown)
		{
			if(inselected)
			{
				current->SetActive(false);
				m_aSelectedControler.erase(pos);
			}
			else
			{
				current->SetActive(true);
				m_aSelectedControler.push_back(current);
			}
		}
		else
		{
			if(!inselected)
			{
				for(UINT i=0;i<m_aSelectedControler.size();++i)
				{
					m_aSelectedControler[i]->SetActive(false);
				}
				m_aSelectedControler.clear();
				current->SetActive(true);
				m_aSelectedControler.push_back(current);
			}	
		}
		((CRulerCtlWnd*)pRulerWnd)->DrawRulerCursor(current->GetMyViewCenter()->x+CRulerCtlWnd::VERTICAL_RULER_RIGHT,
			current->GetMyViewCenter()->y+CRulerCtlWnd::HORIZON_RULER_BOTTOM);
		SetMonitorText(current->GetMyMonitorInfo());
	}
	else
	{
		for(UINT i=0;i<m_aSelectedControler.size();++i)
		{
			m_aSelectedControler[i]->SetActive(false);
		}
		m_aSelectedControler.clear();
		SetMonitorText(_T("N/A"));
	}
}
void MonitorManager::OnLButtonUp(CWnd* pRulerWnd)
{
	if(m_bLButtonDown)
	{
		((CRulerCtlWnd*)pRulerWnd)->DrawRulerCursor(-1,-1,false);
	}
	SetStatusText(_T("Ready"));
	/*
	if(m_aSelectedControler.size() == 0)
	{
		SetMonitorText(_T("N/A"));
	}
	*/
	m_bLButtonDown=false;
	m_pCurrentControler=NULL;
	m_ePos=RotateRect::POS_NULL;
}



void MonitorManager::OnRButtonDown(UINT nFlags, CPoint point)
{

}
bool MonitorManager::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	
	if(m_aSelectedControler.size() == 0)
		return false;

	switch(nChar)
	{
	case VK_UP:
		for(UINT i=0;i<m_aSelectedControler.size();++i)
		{
			m_aSelectedControler[i]->Move(0,-1*int(nRepCnt));
		}
		return true;
	case VK_DOWN:
		for(UINT i=0;i<m_aSelectedControler.size();++i)
		{
			m_aSelectedControler[i]->Move(0,1*int(nRepCnt));
		}
		return true;
	case VK_LEFT:
		for(UINT i=0;i<m_aSelectedControler.size();++i)
		{
			m_aSelectedControler[i]->Move(-1*int(nRepCnt),0);
		}
		return true;
	case VK_RIGHT:
		for(UINT i=0;i<m_aSelectedControler.size();++i)
		{
			m_aSelectedControler[i]->Move(1*int(nRepCnt),0);
		}
		return true;
	case 'A':
		for(UINT i=0;i<m_aSelectedControler.size();++i)
		{
			m_aSelectedControler[i]->Rotate(1*int(nRepCnt));
		}
		return true;
	case 'D':
		for(UINT i=0;i<m_aSelectedControler.size();++i)
		{
			m_aSelectedControler[i]->Rotate(-1*int(nRepCnt));
		}
		return true;
		/*
	case 'C':
		for(UINT i=0;i<m_aSelectedControler.size();++i)
		{
			//m_aSelectedControler[i]->ResetAngle();
		}
		break;
		*/
	}
	return false;
}
/*
const std::vector<RotateRect*>& MonitorManager::GetRotateRects()
{
	return m_aMonitorsControler;
}
*/
#define PI 3.1415926
double MonitorManager::CalRotateAngel(const CPoint& center,const CPoint& now,const CPoint& orig)
{
	double result;
	double angel1,angel2;
	angel1=atan2((double(orig.y-center.y)),(double(orig.x-center.x)));
	angel2=atan2((double(now.y-center.y)),(double(now.x-center.x)));
	result = ((angel1-angel2)/PI)*180 ;
	if(result <= -180)
	{
		result+=360;
	}
	else
	{
		if(result > 180)
			result -= 360;
	}
	return result;
}