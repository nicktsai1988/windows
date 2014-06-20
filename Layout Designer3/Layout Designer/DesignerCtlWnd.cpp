
// ChildView.cpp : implementation of the CDesignerCtlWnd class
//

#include "stdafx.h"
#include "Layout Designer.h"
#include "DesignerCtlWnd.h"
#include "RulerCtlWnd.h"
#include "ExportInfo.h"
#include "MonitorTypeWnd.h"
#include "EditMonitorDialog.h"
#include "MonitorInfoObtainer.h"
#include "HostSettingDialog.h"




#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDesignerCtlWnd

CDesignerCtlWnd::CDesignerCtlWnd(CWnd* parent):m_pParentWnd(parent),m_monitorManager(this),
	m_WndScrollSize(0,0),m_Canvas(this)
{
	m_ixoldpos=m_iyoldpos=0;
	m_BkColor=RGB(192,192,192);
	m_GridLineColor=RGB(166,202,240);
	double zoom[5]={1.0,1.0,0.5,0.25,0.1};
	memcpy(m_aViewZoom,zoom,sizeof(double)*5);
	m_iViewZoomIndex=0;
	m_pControlerInCursor=NULL;
	m_strSrvPort=_T("9888");
}

CDesignerCtlWnd::~CDesignerCtlWnd()
{
}

BEGIN_MESSAGE_MAP(CDesignerCtlWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
//	ON_WM_ERASEBKGND()
ON_WM_ERASEBKGND()
ON_WM_CREATE()
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
ON_WM_RBUTTONUP()
ON_WM_CONTEXTMENU()
ON_WM_KEYDOWN()
ON_COMMAND_RANGE(ID_MONITOR_TYPEBEGIN,ID_MONITOR_TYPEEND,&CDesignerCtlWnd::OnChangeType)
ON_COMMAND(ID_POPMENU_EDITMONITOR,&CDesignerCtlWnd::OnEditMonitor)
ON_MESSAGE(WM_USER_DETECT,&CDesignerCtlWnd::OnDetect)
ON_MESSAGE(WM_USER_SAVE,&CDesignerCtlWnd::OnSave)
ON_MESSAGE(WM_USER_ZOOM,&CDesignerCtlWnd::OnZoom)


END_MESSAGE_MAP()



// CDesignerCtlWnd message handlers

BOOL CDesignerCtlWnd::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CDesignerCtlWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	//OnPrepareDC(&dc);
	OnDraw(&dc);
	
	// TODO: Add your message handler code here
	//CWnd::OnPaint();
	
	// Do not call CWnd::OnPaint() for painting messages
}
void CDesignerCtlWnd::OnPrepareDC(CDC* PDC)
{
	//CPoint ptOrg(-GetScrollPos(SB_HORZ),-GetScrollPos(SB_VERT));
	//PDC->SetViewportOrg(ptOrg);
}


void CDesignerCtlWnd::OnDraw(CDC* pDC)
{
	CRect clientRect;

	GetClientRect(clientRect);
	CDC memDC;
	CBitmap memBitmap;
	BOOL bMemDC=FALSE;
	if(memDC.CreateCompatibleDC(pDC))
	{
		if(memBitmap.CreateCompatibleBitmap(pDC,clientRect.Width(),clientRect.Height()))
		{
			memDC.SelectObject(&memBitmap);
		}
		else
		{
			memDC.DeleteDC();
		}
	}

	bMemDC=memDC.GetSafeHdc() != NULL;
	CDC *pDrawDC=bMemDC?&memDC:pDC;
	
	CBrush brush;
	brush.CreateSolidBrush(m_BkColor);
	pDrawDC->FillRect(clientRect,&brush);
	brush.DeleteObject();
	
	m_Canvas.Draw(pDrawDC);
	m_monitorManager.OnDraw(pDrawDC);
	if(bMemDC)
	{
		pDC->BitBlt(clientRect.left,clientRect.top,clientRect.Width(),clientRect.Height(),
			&memDC,0,0,SRCCOPY);
		memDC.DeleteDC();
		memBitmap.DeleteObject();
	}
}

void CDesignerCtlWnd::SetWndScrollSize(const CSize& size)
{
	m_WndScrollSize=size;
	m_Canvas.SetSize((size.cx*2)/3,(size.cy*2)/3);
}
const CSize& CDesignerCtlWnd::GetWndScrollSize()
{
	return m_WndScrollSize;
}

const CRect& CDesignerCtlWnd::GetCanvasViewRect()
{
	return m_Canvas.GetCanvasViewRect();
}
double CDesignerCtlWnd::GetGridViewSize()
{
	return m_Canvas.GetGridViewSize();
}

void CDesignerCtlWnd::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	int minpos = 0; 
	int maxpos = 0;  
	GetScrollRange(SB_HORZ, &minpos, &maxpos);   
	maxpos = GetScrollLimit(SB_HORZ);

	// Get the current position of scroll box.  
	int curpos = GetScrollPos(SB_HORZ);  

	// Determine the new position of scroll box.  
	switch (nSBCode)  
	{  
	case SB_LEFT:      // Scroll to far left.  
		curpos = minpos;  
		break;  

	case SB_RIGHT:      // Scroll to far right.  
		curpos = maxpos;  
		break;  

	case SB_ENDSCROLL:   // End scroll.  
		break;  

	case SB_LINELEFT:      // Scroll left.  
		if (curpos > minpos)  
			curpos--;  
		break;  

	case SB_LINERIGHT:   // Scroll right.  
		if (curpos < maxpos)  
			curpos++;  
		break;  

	case SB_PAGELEFT:    // Scroll one page left.  
		{  
			// Get the page size.   
			SCROLLINFO   info;  
			GetScrollInfo(SB_HORZ, &info, SIF_ALL);  

			if (curpos > minpos)  
				curpos = max(minpos, curpos - (int) info.nPage);  
		}  
		break;

	case SB_PAGERIGHT:      // Scroll one page right.  
		{  
			// Get the page size.   
			SCROLLINFO   info;  
			GetScrollInfo(SB_HORZ, &info, SIF_ALL);  

			if (curpos < maxpos)  
				curpos = min(maxpos, curpos + (int) info.nPage);  
		}  
		break;  

	case SB_THUMBPOSITION: // Scroll to absolute position. nPos is the position  
		curpos = nPos;      // of the scroll box at the end of the drag operation.  
		break;  

	case SB_THUMBTRACK:   // Drag scroll box to specified position. nPos is the  
		curpos = nPos;     // position that the scroll box has been dragged to.  
		break;  
	}

	// Set the new position of the thumb (scroll box).  
	SetScrollPos(SB_HORZ, curpos);  
	
	m_monitorManager.SetScrollViewOffset(CSize(curpos,m_iyoldpos));
	m_Canvas.SetScrollViewOffset(CSize(curpos,m_iyoldpos));
	m_monitorManager.SetViewOrigOffset(GetMonitorViewOrig());
	m_ixoldpos = curpos;
	
	m_pParentWnd->Invalidate();
	UpdateWindow();
	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
	
}


void CDesignerCtlWnd::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	int minpos = 0;  
	int maxpos = 0;  
	GetScrollRange(SB_VERT, &minpos, &maxpos);   
	maxpos = GetScrollLimit(SB_VERT);

	// Get the current position of scroll box.  
	int curpos = GetScrollPos(SB_VERT);  

	// Determine the new position of scroll box.  
	switch (nSBCode)  
	{  
	case SB_TOP:      // Scroll to far left.  
		curpos = minpos;  
		break;  

	case SB_BOTTOM:      // Scroll to far right.  
		curpos = maxpos;  
		break;  

	case SB_ENDSCROLL:   // End scroll.  
		break;  

	case SB_LINEUP:      // Scroll left.  
		if (curpos > minpos)  
			curpos--;  
		break;  

	case SB_LINEDOWN:   // Scroll right.  
		if (curpos < maxpos)  
			curpos++;  
		break;  

	case SB_PAGEUP:    // Scroll one page left.  
		{  
			// Get the page size.   
			SCROLLINFO   info;  
			GetScrollInfo(SB_VERT, &info, SIF_ALL);  

			if (curpos > minpos)  
				curpos = max(minpos, curpos - (int) info.nPage);  
		}  
		break;  

	case SB_PAGEDOWN:      // Scroll one page right.  
		{  
			// Get the page size.   
			SCROLLINFO   info;  
			GetScrollInfo(SB_VERT, &info, SIF_ALL);  

			if (curpos < maxpos)  
				curpos = min(maxpos, curpos + (int) info.nPage);  
		}  
		break;

	case SB_THUMBPOSITION: // Scroll to absolute position. nPos is the position  
		curpos = nPos;      // of the scroll box at the end of the drag operation.  
		break;  

	case SB_THUMBTRACK:   // Drag scroll box to specified position. nPos is the  
		curpos = nPos;     // position that the scroll box has been dragged to.  
		break;  
	}  

	// Set the new position of the thumb (scroll box).  
	SetScrollPos(SB_VERT, curpos);  
	
	m_monitorManager.SetScrollViewOffset(CSize(m_ixoldpos,curpos));
	m_Canvas.SetScrollViewOffset(CSize(m_ixoldpos,curpos));
	m_monitorManager.SetViewOrigOffset(GetMonitorViewOrig());
	m_iyoldpos=curpos;
	
	m_pParentWnd->Invalidate();
	//Invalidate(TRUE);
	UpdateWindow();
	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);	
}


void CDesignerCtlWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	UpdateScrollBar(cx,cy);
	m_pParentWnd->Invalidate();
	UpdateWindow();
}

void CDesignerCtlWnd::UpdateScrollBar(int cx,int cy)
{
	SCROLLINFO si;  
	si.cbSize = sizeof(si);  
	si.fMask = SIF_RANGE | SIF_PAGE;  
	si.nMin = 0;  
	si.nMax = m_WndScrollSize.cx;  
	si.nPage = cx;  
	SetScrollInfo(SB_HORZ,&si,TRUE);  
	si.nMax = m_WndScrollSize.cy;  
	si.nPage = cy; 
	SetScrollInfo(SB_VERT,&si,TRUE);  

	int icurxpos = GetScrollPos(SB_HORZ);  
	int icurypos = GetScrollPos(SB_VERT);  


	//if (icurxpos < m_ixoldpos || icurypos < m_iyoldpos)  
	//{  
		m_monitorManager.SetScrollViewOffset(CSize(icurxpos,icurypos));
		m_Canvas.SetScrollViewOffset(CSize(icurxpos,icurypos));	
	//}
	m_monitorManager.SetViewOrigOffset(GetMonitorViewOrig());
	m_ixoldpos = icurxpos;  
	m_iyoldpos = icurypos; 
}


void CDesignerCtlWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CWnd::OnMouseMove(nFlags, point);
	if(!m_pParentWnd)
		return;
	m_monitorManager.OnMouseMove(nFlags,point,m_pParentWnd);
	Invalidate();
}




BOOL CDesignerCtlWnd::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return TRUE;
}


int CDesignerCtlWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	return 0;
}


void CDesignerCtlWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CWnd::OnLButtonDown(nFlags, point);
	SetCapture();
	m_monitorManager.OnLButtonDown(nFlags,point,m_pParentWnd);
	Invalidate();
}


void CDesignerCtlWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_monitorManager.OnLButtonUp(m_pParentWnd);
	::ReleaseCapture();
	
	CWnd::OnLButtonUp(nFlags, point);
}


void CDesignerCtlWnd::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CWnd::OnRButtonUp(nFlags, point);
}


void CDesignerCtlWnd::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	// TODO: Add your message handler code here
	CPoint clientPoint=point;
	ScreenToClient(&clientPoint);
	CMenu popMenu;
	CMenu subMenu;
	popMenu.CreatePopupMenu();
	m_pControlerInCursor=NULL;
	m_pControlerInCursor=m_monitorManager.GetMonitorControlerInPt(clientPoint);

	if(m_pControlerInCursor)
	{
		subMenu.CreatePopupMenu();
		InitMenuItem(subMenu,m_pControlerInCursor);
		popMenu.AppendMenu(MF_POPUP | MF_STRING,(UINT)subMenu.m_hMenu,_T("Change Type"));
		popMenu.AppendMenu(MF_STRING,ID_POPMENU_EDITMONITOR,_T("Edit Monitor"));
	}
	popMenu.AppendMenu(MF_STRING,ID_POPMENU_SETCANVAS,_T("Set Canvas"));
	popMenu.TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, point.x, point.y,this);
	subMenu.DestroyMenu();
	popMenu.DestroyMenu();
}

BOOL CDesignerCtlWnd::LoadMonitorInfo()
{
	//read monitor info from OS through APIs
	//std::vector<CRect> monitors;
	m_aOrigMonitorRects.clear();
	CHostSettingDialog hostDialog;
	hostDialog.m_strAddress=m_strSrvAddress;
	hostDialog.m_strPort=m_strSrvPort;
	if(hostDialog.DoModal() != IDOK)
	{
		return FALSE;
	}
	m_strSrvAddress=hostDialog.m_strAddress;
	m_strSrvPort=hostDialog.m_strPort;
	CHostSettingDialog::HostInfo hostinfo=hostDialog.ConvertToHostInfo();
	CMonitorInfoObtainer::InitLibrary();
	CMonitorInfoObtainer obtainer;
	bool ret=obtainer.Load(hostinfo.m_nAddress,hostinfo.m_nPort);
	CMonitorInfoObtainer::ReleaseLibrary();
	if(!ret)
	{
		AfxMessageBox(_T("Load monitor info failed!"));
		return FALSE;
	}
	const std::vector<MonitorRect>& monitorRects=obtainer.GetMonitorRects();
	for(size_t i=0;i<monitorRects.size();++i)
	{
		m_aOrigMonitorRects.push_back(CRect(monitorRects[i]));
	}
	//m_aOrigMonitorRects.push_back(CRect(0,0,1920,1080));
	//m_aOrigMonitorRects.push_back(CRect(1920,0,3840,1080));
	//m_aOrigMonitorRects.push_back(CRect(0,1080,1920,2160));
	//m_aOrigMonitorRects.push_back(CRect(1920,1080,3840,2160));
	return TRUE;
}

void CDesignerCtlWnd::InitViewer()
{
	for(UINT i=0;i<m_aMonitorInfo.size();++i)
	{
		CalMonitorNewPos(m_aMonitorInfo[i]);
	}
	CalAutoViewScale();
	m_monitorManager.SetViewScale(m_aViewZoom[m_iViewZoomIndex]);

	for(UINT i=0;i<m_aMonitorInfo.size();++i)
	{
		m_monitorManager.AddMonitor(&m_aMonitorInfo[i]);
	}
	m_monitorManager.SetViewOrigOffset(GetMonitorViewOrig());
}

bool CDesignerCtlWnd::MatchTypeAndTransPos()
{
	
	return(MatchMonitorType()&& TransformMonitorPosition());
}


bool CDesignerCtlWnd::MatchMonitorType()
{
	//match types by the resolution
	const std::list<MonitorTypeElement*>& monitorTypes=g_pMonitorTypeWnd->GetTypes();
	const MonitorTypeElement* pDefaultType=g_pMonitorTypeWnd->GetDefaultType();
	if(monitorTypes.size() == 0)
	{
		AfxMessageBox(_T("Add monitor type first!"));
		return false;
	}
	int horizon,vertical;
	MonitorInfo newinfo;
	for(size_t i=0;i<m_aOrigMonitorRects.size();++i)
	{
		horizon=m_aOrigMonitorRects[i].right - m_aOrigMonitorRects[i].left;
		vertical=m_aOrigMonitorRects[i].bottom - m_aOrigMonitorRects[i].top;
		std::list<MonitorTypeElement*>::const_iterator it;
		for(it=monitorTypes.begin();it != monitorTypes.end();++it)
		{
			if((horizon == (*it)->m_type.m_nHorizonResolution ) && 
				(vertical == (*it)->m_type.m_nVerticalResolution))
			{
				newinfo.m_nID=i;
				newinfo.m_type=(*it)->m_type;
				newinfo.m_pTypeElement=(*it);
				newinfo.m_posOrig.m_posInPixel=m_aOrigMonitorRects[i].TopLeft();
				m_aMonitorInfo.push_back(newinfo);
				break;
			}
		}
		if(monitorTypes.end() == it)
		{
			AfxMessageBox(_T("match monitor type error"));
			return false;
		}
	}
	
	for(size_t i=0;i<m_aMonitorInfo.size();++i)
	{
		if((m_aMonitorInfo[i].m_type.m_nHorizonResolution == pDefaultType->m_type.m_nHorizonResolution) &&
			(m_aMonitorInfo[i].m_type.m_nVerticalResolution == pDefaultType->m_type.m_nVerticalResolution))
		{
			m_aMonitorInfo[i].m_type=pDefaultType->m_type;
			m_aMonitorInfo[i].m_pTypeElement=pDefaultType;
		}
	}
	return true;
}

bool CDesignerCtlWnd::TransformMonitorPosition()
{
	CRect containerRect=CalContainerRectBeforeTransform();
	std::list<MonitorInfo*> infoList;
	for(UINT i=0;i<m_aMonitorInfo.size();++i)
	{
		infoList.push_back(&m_aMonitorInfo[i]);
	}
	TransformInContainer(containerRect,CPoint(0,0),infoList);
	if(infoList.size() != 0)
	{
		AfxMessageBox(_T("TransformMonitorPosition error"));
		return false;
	}
	else
		return true;
}

void CDesignerCtlWnd::TransformInContainer(const CRect& container,const CPoint& lastNewBottomRight,std::list<MonitorInfo*>& infoList)
{
	if((container.bottom == container.top) || (container.left == container.right))
		return;
	CPoint topleft=container.TopLeft();
	MonitorInfo* pCurrentMonitor=NULL;
	std::list<MonitorInfo*>::iterator it;
	for(it=infoList.begin();it != infoList.end();++it)
	{
		if(((*it)->m_posOrig.m_posInPixel.x == topleft.x)&&((*it)->m_posOrig.m_posInPixel.y == topleft.y))
		{
			pCurrentMonitor=(*it);
			break;
		}
	}
	if(NULL == pCurrentMonitor)
	{
		AfxMessageBox(_T("TransformInContainer error"));
	}
	else
	{
		//calculate size add border size
		CPoint newBottomright=CPoint(lastNewBottomRight.x+pCurrentMonitor->m_type.m_nWidth+pCurrentMonitor->m_type.m_nBorderLeft+pCurrentMonitor->m_type.m_nBorderRight,
			lastNewBottomRight.y+pCurrentMonitor->m_type.m_nHeight+pCurrentMonitor->m_type.m_nBorderTop+pCurrentMonitor->m_type.m_nBorderBottom);
		//update my position
		pCurrentMonitor->m_posOrig.m_posInSize.x = lastNewBottomRight.x;
		pCurrentMonitor->m_posOrig.m_posInSize.y = lastNewBottomRight.y;
		//continue update others
		CRect rectHorizon,rectVertical,rectMiddle;
		rectHorizon=container;
		rectHorizon.left += pCurrentMonitor->m_type.m_nHorizonResolution;
		rectHorizon.bottom = rectHorizon.top+pCurrentMonitor->m_type.m_nVerticalResolution;
		rectVertical=container;
		rectVertical.top += pCurrentMonitor->m_type.m_nVerticalResolution;
		rectVertical.right = rectVertical.left+pCurrentMonitor->m_type.m_nHorizonResolution;
		rectMiddle=container;
		rectMiddle.left += pCurrentMonitor->m_type.m_nHorizonResolution;
		rectMiddle.top += pCurrentMonitor->m_type.m_nVerticalResolution;
		infoList.erase(it);
		//add border
		TransformInContainer(rectHorizon,CPoint(newBottomright.x,lastNewBottomRight.y),infoList);
		TransformInContainer(rectVertical,CPoint(lastNewBottomRight.x,newBottomright.y),infoList);
		TransformInContainer(rectMiddle,newBottomright,infoList);
	}
}

CRect CDesignerCtlWnd::CalContainerRectBeforeTransform()
{
	int left,top,right,bottom;
	left=top=INT_MAX;
	right=bottom=INT_MIN;
	for(UINT i =0;i<m_aMonitorInfo.size();++i)
	{
		left=min(left,m_aMonitorInfo[i].m_posOrig.m_posInPixel.x);
		right=max(right,m_aMonitorInfo[i].m_posOrig.m_posInPixel.x+m_aMonitorInfo[i].m_type.m_nHorizonResolution);
		top=min(top,m_aMonitorInfo[i].m_posOrig.m_posInPixel.y);
		bottom=max(bottom,m_aMonitorInfo[i].m_posOrig.m_posInPixel.y+m_aMonitorInfo[i].m_type.m_nVerticalResolution);
	}
	return CRect(left,top,right,bottom);
}

CRect CDesignerCtlWnd::CalContainerRectAfterTransform()
{
	int left,top,right,bottom;
	left=top=INT_MAX;
	right=bottom=INT_MIN;
	for(UINT i =0;i<m_aMonitorInfo.size();++i)
	{
		left=min(left,m_aMonitorInfo[i].m_posOrig.m_posInSize.x);
		right=max(right,m_aMonitorInfo[i].m_posOrig.m_posInSize.x+m_aMonitorInfo[i].m_type.m_nWidth+m_aMonitorInfo[i].m_type.m_nBorderLeft+m_aMonitorInfo[i].m_type.m_nBorderRight);
		top=min(top,m_aMonitorInfo[i].m_posOrig.m_posInSize.y);
		bottom=max(bottom,m_aMonitorInfo[i].m_posOrig.m_posInSize.y+m_aMonitorInfo[i].m_type.m_nHeight+m_aMonitorInfo[i].m_type.m_nBorderTop+m_aMonitorInfo[i].m_type.m_nBorderBottom);
	}
	return CRect(left,top,right,bottom);
}



void CDesignerCtlWnd::CalMonitorNewPos(MonitorInfo& info)
{
	int left,top,width,height;
	info.m_posNew.m_dRotation=0.0;
	left=info.m_posOrig.m_posInSize.x;
	top=info.m_posOrig.m_posInSize.y;
	width=info.m_type.m_nWidth+info.m_type.m_nBorderLeft+info.m_type.m_nBorderRight;
	height=info.m_type.m_nHeight+info.m_type.m_nBorderTop+info.m_type.m_nBorderBottom;
	info.m_posNew.m_Points[0].x=left;
	info.m_posNew.m_Points[0].y=top;
	info.m_posNew.m_Points[1].x=left+width;
	info.m_posNew.m_Points[1].y=top;
	info.m_posNew.m_Points[2].x=left+width;
	info.m_posNew.m_Points[2].y=top+height;
	info.m_posNew.m_Points[3].x=left;
	info.m_posNew.m_Points[3].y=top+height;
	info.m_posNew.m_Center.x=(2*left+width)/2;
	info.m_posNew.m_Center.y=(2*top+height)/2;
}

void CDesignerCtlWnd::CalAutoViewScale()
{
	CRect container=CalContainerRectAfterTransform();
	int screenX=GetSystemMetrics(SM_CXSCREEN);
	int screenY=GetSystemMetrics(SM_CYSCREEN);
	CSize containerSize=container.Size();
	CSize canvasSize(containerSize.cx*2,containerSize.cy*2);
	CSize scrollSize(canvasSize.cx*3/2,canvasSize.cy*3/2);
	CSize designSize(scrollSize.cx*5/4,scrollSize.cy*5/4);

	int realwidth=screenX-200-30;
	int realheight=screenY-30-50-20-80-30;



	double scale=max((double(realwidth)/designSize.cx),(double(realheight)/designSize.cy));
	m_aViewZoom[0]=scale;
	//test
	//m_aViewZoom[0]=0.25;
	/*
	int scrollX=scrollSize.cx*scale;
	int scrollY=scrollSize.cy*scale;
	SetWndScrollSize(CSize(scrollX,scrollY));
	m_Canvas.SetViewScale(1.0/scale);
	return scale;
	*/
}

CPoint CDesignerCtlWnd::GetMonitorViewOrig()
{
	return m_Canvas.GetMonitorViewOrig();
}

double CDesignerCtlWnd::GetViewScale()
{
	return m_aViewZoom[m_iViewZoomIndex];
}
void CDesignerCtlWnd::Reset()
{

	m_monitorManager.Reset();
	m_aTypeIndex.clear();
}
LRESULT CDesignerCtlWnd::OnDetect(WPARAM wp,LPARAM lp)
{
	const std::list<MonitorTypeElement*>& monitorTypes=g_pMonitorTypeWnd->GetTypes();
	if(monitorTypes.size() == 0)
	{
		AfxMessageBox(_T("Add monitor type first!"));
		return 0;
	}
	if(m_aMonitorInfo.size() == 0)
	{
		if(!LoadMonitorInfo())
		{
			//AfxMessageBox(_T("Load Monitor info failed!"));
			Reset();
			return -1;
		}
		//do match and and trans position
		if(!MatchTypeAndTransPos())
		{
			Reset();
			return -1;
		}
		//TODO::calculate display scale
		
		InitViewer();
		OnZoom(m_iViewZoomIndex,0);
		ASSERT(g_pMonitorTypeWnd);
		g_pMonitorTypeWnd->SetTypeDeletable(false);
	}
	return 0;
}

LRESULT CDesignerCtlWnd::OnOpen(WPARAM wp,LPARAM lp)
{
	return 0;
}
LRESULT CDesignerCtlWnd::OnSave(WPARAM wp,LPARAM lp)
{
	CanvasInfo canvas;
	canvas.width=1000;
	canvas.height=800;
	canvas.horizon=1920;
	canvas.vertical=1080;
	ExportInfo exporter;
	exporter.SetData(&canvas,&m_aMonitorInfo);
	DWORD flags=OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT;
	CString filter=_T("XML File(*.xml)|*.xml|All Files(*.*)|*.*||");
	CFileDialog saveFileDialog(FALSE,_T(".xml"),_T("result.xml"),flags,filter,this);
	if(saveFileDialog.DoModal() == IDOK)
	{
		CString filename=saveFileDialog.GetPathName();
		exporter.SaveFile(CString2StdString(filename).c_str());
	}
	return 0;

}
LRESULT CDesignerCtlWnd::OnSaveAs(WPARAM wp,LPARAM lp)
{
	return 0;
}
LRESULT CDesignerCtlWnd::OnRevert(WPARAM wp,LPARAM lp)
{
	return 0;
}
LRESULT CDesignerCtlWnd::OnUndo(WPARAM wp,LPARAM lp)
{
	return 0;
}
LRESULT CDesignerCtlWnd::OnRedo(WPARAM wp,LPARAM lp)
{
	return 0;
}
LRESULT CDesignerCtlWnd::OnSelectAll(WPARAM wp,LPARAM lp)
{
	return 0;
}
LRESULT CDesignerCtlWnd::OnDesignView(WPARAM wp,LPARAM lp)
{
	return 0;
}
LRESULT CDesignerCtlWnd::OnOriginalView(WPARAM wp,LPARAM lp)
{
	return 0;
}
LRESULT CDesignerCtlWnd::OnZoom(WPARAM wp,LPARAM lp)
{
	//CString str;
	m_iViewZoomIndex=wp;
	if(m_aMonitorInfo.size())
	{
		DoZoom(m_aViewZoom[wp]);
		CRect clientRect;
		GetClientRect(clientRect);
		CSize clientSize=clientRect.Size();
		UpdateScrollBar(clientSize.cx,clientSize.cy);
		m_pParentWnd->Invalidate();
		Invalidate();
	}
	return 0;
}
LRESULT CDesignerCtlWnd::OnHelp(WPARAM wp,LPARAM lp)
{
	return 0;
}

void CDesignerCtlWnd::InitMenuItem(CMenu& menu,const CMonitorControler* pControler)
{
	const MonitorInfo* pInfo=pControler->GetMyMonitorInfo();
	m_aTypeIndex.clear();
	const std::list<MonitorTypeElement*>& monitorType=g_pMonitorTypeWnd->GetTypes();
	std::list<MonitorTypeElement*>::const_iterator it;
	UINT index=0,i=0;
	for(i=0,it=monitorType.begin();it != monitorType.end();++it,++i)
	{
		if(((*it)->m_type.m_nHorizonResolution== pInfo->m_type.m_nHorizonResolution)
			&&((*it)->m_type.m_nVerticalResolution== pInfo->m_type.m_nVerticalResolution))
		{
			index=ID_MONITOR_TYPEBEGIN+i;
			menu.AppendMenu(MF_STRING,index,(*it)->m_type.m_strDescription);
			MonitorTypeIndex typeIndex;
			typeIndex.m_nIndex=index;
			typeIndex.m_pElement=(*it);
			m_aTypeIndex.push_back(typeIndex);
		}
	}
}

void CDesignerCtlWnd::OnChangeType(UINT uID)
{
	ASSERT(m_pControlerInCursor);
	for(size_t i=0;i<m_aTypeIndex.size();++i)
	{
		if((m_aTypeIndex[i].m_nIndex == uID)
			&&(m_aTypeIndex[i].m_pElement != m_pControlerInCursor->GetMyMonitorInfo()->m_pTypeElement))
		{
			m_pControlerInCursor->ChangeType(m_aTypeIndex[i].m_pElement);
			Reset();
			TransformMonitorPosition();
			InitViewer();
			OnZoom(m_iViewZoomIndex,0);
			Invalidate();
			return;
		}
	}
}

void CDesignerCtlWnd::MonitorsUpdateType(const MonitorTypeElement* pTypeElement)
{
	ASSERT(pTypeElement);
	m_monitorManager.MonitorsUpdateType(pTypeElement);
	if(m_aMonitorInfo.size())
	{
		Reset();
		TransformMonitorPosition();
		InitViewer();
		OnZoom(m_iViewZoomIndex,0);
		Invalidate();
	}
}

void CDesignerCtlWnd::OnEditMonitor()
{
	CEditMonitorDialog editDialog;
	ASSERT(m_pControlerInCursor);
	editDialog.SetElementValue(m_pControlerInCursor->GetMyMonitorInfo());
	if(editDialog.DoModal() == IDOK)
	{
		//m_pMonitorInCursor
		CEditMonitorDialog::NewPos newPos=editDialog.GetPositionValue();
		m_pControlerInCursor->MoveTo(newPos.x,newPos.y);
		m_pControlerInCursor->RotateTo(GetRoundOffValue(newPos.angle));
		Invalidate();
	}
}

void CDesignerCtlWnd::DoZoom(double dZoomValue)
{
	CRect container=CalContainerRectAfterTransform();
	CSize containerSize=container.Size();
	CSize canvasSize(containerSize.cx*2,containerSize.cy*2);
	CSize scrollSize(canvasSize.cx*3/2,canvasSize.cy*3/2);
	int scrollX=static_cast<int>(scrollSize.cx*dZoomValue);
	int scrollY=static_cast<int>(scrollSize.cy*dZoomValue);
	SetWndScrollSize(CSize(scrollX,scrollY));
	m_Canvas.SetViewScale(dZoomValue);
	m_monitorManager.SetViewScale(dZoomValue);
}


void CDesignerCtlWnd::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	//CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
	if(m_monitorManager.OnKeyDown(nChar,nRepCnt,nFlags))
	{
		Invalidate();
	}
}


BOOL CDesignerCtlWnd::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	return CWnd::PreTranslateMessage(pMsg);
}
