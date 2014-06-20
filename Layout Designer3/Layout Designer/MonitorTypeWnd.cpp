// MonitorTypeWnd.cpp : implementation file
//

#include "stdafx.h"
#include "MonitorTypeWnd.h"
#include "MonitorTypeDialog.h"
#include "DesignerCtlWnd.h"


// MonitorTypeWnd

IMPLEMENT_DYNAMIC(CMonitorTypeWnd, CWnd)

CMonitorTypeWnd::CMonitorTypeWnd()
{
	m_BkColor=RGB(255,255,255);
	m_iScrollHeight=600;
	m_iyoldpos=0;
	m_pMaxMonitorType=NULL;
	m_pDefaultMonitorType=NULL;
	m_fontMonitorType.CreateFont(20,0,0,0,FW_NORMAL,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_SWISS,_T("Arial"));
	m_penMonitorType.CreatePen(PS_INSIDEFRAME,2,RGB(0,0,0));
	m_bTypeDeletable=true;
}

CMonitorTypeWnd::~CMonitorTypeWnd()
{
	std::list<MonitorTypeElement*>::iterator it;
	for(it=m_listMonitorTypes.begin();it != m_listMonitorTypes.end();++it)
	{
		delete *it;
	}
	m_fontMonitorType.DeleteObject();
	m_penMonitorType.DeleteObject();
}

BOOL CMonitorTypeWnd::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}


BEGIN_MESSAGE_MAP(CMonitorTypeWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_MONITORTYPE_ADDTYPE,&CMonitorTypeWnd::OnAddType)
	ON_COMMAND(ID_MONITORTYPE_DELETETYPE,&CMonitorTypeWnd::OnDeleteType)
	ON_COMMAND(ID_MONITORTYPE_EDITTYPE,&CMonitorTypeWnd::OnEditType)
	ON_COMMAND(ID_MONITORTYPE_SETASDEFAULT,&CMonitorTypeWnd::OnSetAsDefault)
	ON_WM_VSCROLL()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// MonitorTypeWnd message handlers




void CMonitorTypeWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CWnd::OnPaint() for painting messages
	OnDraw(&dc);
}

void CMonitorTypeWnd::OnDraw(CDC* pDC)
{
	UpdateRectView();
	UpdateScrollBar();
	CMemDC::m_bUseMemoryDC=TRUE;
	CMemDC memDC(*pDC,this);
	CDC& dc=memDC.GetDC();
	pDC=&dc;
	CBrush brush;
	CRect rect;
	brush.CreateSolidBrush(m_BkColor);
	pDC->GetClipBox(rect);
	pDC->FillRect(rect,&brush);
	brush.DeleteObject();

	std::list<MonitorTypeElement*>::iterator it;
	
	for(it=m_listMonitorTypes.begin();it != m_listMonitorTypes.end();++it)
	{
		rect=(*it)->m_rectView;
		rect.OffsetRect(0,-m_iyoldpos);
		brush.CreateSolidBrush((*it)->m_type.m_nBkColor);
		pDC->FillRect(rect,&brush);
		brush.DeleteObject();
		CRect descRect;
		pDC->SelectObject(&m_fontMonitorType);
		pDC->SetBkMode(TRANSPARENT);
		if(m_pDefaultMonitorType != (*it))
		{
			pDC->DrawText((*it)->m_type.m_strDescription,descRect,DT_CALCRECT);
			descRect.OffsetRect(rect.left+rect.Width()/2,rect.top+rect.Height()/2);
			descRect.OffsetRect(-descRect.Width()/2,-descRect.Height()/2);
			pDC->DrawText((*it)->m_type.m_strDescription,descRect,DT_CENTER | DT_SINGLELINE | DT_VCENTER);
		}
		else
		{
			CString descStr=(*it)->m_type.m_strDescription+_T("\n(Default)");
			pDC->DrawText(descStr,descRect,DT_CALCRECT);
			descRect.OffsetRect(rect.left+rect.Width()/2,rect.top+rect.Height()/2);
			descRect.OffsetRect(-descRect.Width()/2,-descRect.Height()/2);
			pDC->DrawText(descStr,descRect,DT_CENTER | DT_VCENTER);
		}
		CPoint points[4];
		points[0]=rect.TopLeft();
		points[2]=rect.BottomRight();
		points[1]=points[0];
		points[1].x += rect.Width();
		points[3]=points[2];
		points[3].x -= rect.Width();
		pDC->SelectObject(&m_penMonitorType);
		pDC->MoveTo(points[0].x,points[0].y);
		for(UINT i=0;i<4;++i)
		{
			pDC->LineTo(points[i].x,points[i].y);
		}
		pDC->LineTo(points[0].x,points[0].y);
	}
}


BOOL CMonitorTypeWnd::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
}


void CMonitorTypeWnd::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	// TODO: Add your message handler code here
	CMenu popMenu;
	popMenu.CreatePopupMenu();
	popMenu.AppendMenu(MF_STRING,ID_MONITORTYPE_IMPORT,_T("Import"));
	popMenu.AppendMenu(MF_STRING,ID_MONITORTYPE_EXPORT,_T("Export"));
	popMenu.AppendMenu(MF_SEPARATOR);

	m_pointContexMenu=point;
	ScreenToClient(&m_pointContexMenu);
	MonitorTypeElement* pElement=GetTypeInPoint(m_pointContexMenu);
	if(pElement != NULL)
	{
		if(m_bTypeDeletable)
		{
			popMenu.AppendMenu(MF_STRING,ID_MONITORTYPE_SETASDEFAULT,_T("Set As Default"));
			popMenu.AppendMenu(MF_SEPARATOR);
		}
		popMenu.AppendMenu(MF_STRING,ID_MONITORTYPE_EDITTYPE,_T("Edit type"));
		popMenu.AppendMenu(MF_STRING,ID_MONITORTYPE_ADDTYPE,_T("Add type"));
		if(m_bTypeDeletable)
		{
			popMenu.AppendMenu(MF_STRING,ID_MONITORTYPE_DELETETYPE,_T("Delete type"));
		}
	}
	else
	{
		popMenu.AppendMenu(MF_STRING,ID_MONITORTYPE_ADDTYPE,_T("Add type"));
	}
	popMenu.TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, point.x, point.y,this);
	popMenu.DestroyMenu();
}

void CMonitorTypeWnd::UpdateRectSize()
{
	if(m_pMaxMonitorType == NULL)
		return;
	std::list<MonitorTypeElement*>::iterator it;
	for(it=m_listMonitorTypes.begin();it != m_listMonitorTypes.end();++it)
	{
		double ratio=(double)(*it)->m_type.m_nWidth/(double)m_pMaxMonitorType->m_type.m_nWidth;
		(*it)->m_dSizeRatio=ratio;
	}
}

void CMonitorTypeWnd::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
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
	m_iyoldpos=curpos;
	Invalidate();
	//UpdateWindow();

	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CMonitorTypeWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here


	// TODO: Add your message handler code here

	SCROLLINFO si;  
	si.cbSize = sizeof(si);  
	si.fMask = SIF_RANGE | SIF_PAGE;  
	si.nMin = 0;  
	si.nMax = m_iScrollHeight;  
	si.nPage = cy; 
	SetScrollInfo(SB_VERT,&si,TRUE);  

	int icurypos = GetScrollPos(SB_VERT);  

	m_iyoldpos = icurypos;
	Invalidate();
	//UpdateWindow();
}

void CMonitorTypeWnd::UpdateScrollBar()
{
	CRect rect;
	GetClientRect(rect);
	SCROLLINFO si;  
	si.cbSize = sizeof(si);  
	si.fMask = SIF_RANGE | SIF_PAGE;  
	si.nMin = 0;  
	si.nMax = m_iScrollHeight;  
	si.nPage =rect.Height(); 
	SetScrollInfo(SB_VERT,&si,TRUE);  

	int icurypos = GetScrollPos(SB_VERT);  

	m_iyoldpos = icurypos;
}


MonitorTypeElement* CMonitorTypeWnd::GetTypeInPoint(CPoint& point)
{
	point.Offset(0,m_iyoldpos);
	std::list<MonitorTypeElement*>::iterator it;
	for(it=m_listMonitorTypes.begin();it != m_listMonitorTypes.end();++it)
	{
		if((*it)->m_rectView.PtInRect(point))
		{
			return *it;
		}
	}
	return NULL;
}
void CMonitorTypeWnd::OnDeleteType()
{
	MonitorTypeElement* element=GetTypeInPoint(m_pointContexMenu);
	if(element)
	{
		std::list<MonitorTypeElement*>::iterator it;
		for(it=m_listMonitorTypes.begin();it != m_listMonitorTypes.end();)
		{
			if(*it == element)
			{
				it=m_listMonitorTypes.erase(it);
				delete element;
				break;
			}
			else
			{
				++it;
			}
		}
		if(element == m_pMaxMonitorType)
		{
			m_pMaxMonitorType = NULL;
			std::list<MonitorTypeElement*>::iterator it;
			for(it=m_listMonitorTypes.begin();it != m_listMonitorTypes.end();++it)
			{
				if((m_pMaxMonitorType == NULL)||(*it)->m_type.m_nWidth > m_pMaxMonitorType->m_type.m_nWidth)
					m_pMaxMonitorType=*it;
			}
			UpdateRectSize();
		}
		if(m_pDefaultMonitorType == element)
		{
			if(m_listMonitorTypes.size())
			{
				m_pDefaultMonitorType=*m_listMonitorTypes.begin();
			}
			else
			{
				m_pDefaultMonitorType=NULL;
			}
		}
		Invalidate();
	}
}

void CMonitorTypeWnd::OnAddType()
{
	CMonitorTypeDialog addDialog;
	int ret=addDialog.DoModal();
	if(ret == IDOK )	
	{

		MonitorTypeElement* pNewElement=new MonitorTypeElement;
		pNewElement->m_type=addDialog.GetMonitorType();
		if(m_pMaxMonitorType != NULL)
		{
			if(pNewElement->m_type.m_nWidth > m_pMaxMonitorType->m_type.m_nWidth)
			{
				m_pMaxMonitorType=pNewElement;
				pNewElement->m_dSizeRatio=1.0;
				UpdateRectSize();
			}
			else
			{
				pNewElement->m_dSizeRatio=(double)pNewElement->m_type.m_nWidth/(double)m_pMaxMonitorType->m_type.m_nWidth;
			}
		}
		else
		{
			m_pMaxMonitorType=pNewElement;
			pNewElement->m_dSizeRatio=1.0;
		}
		if(m_pDefaultMonitorType == NULL)
		{
			m_pDefaultMonitorType=pNewElement;
		}
		m_listMonitorTypes.push_back(pNewElement);
		Invalidate(TRUE);
	}
}

void CMonitorTypeWnd::OnEditType()
{
	MonitorTypeElement* element=GetTypeInPoint(m_pointContexMenu);
	if(element)
	{
		CMonitorTypeDialog editDialog;
		editDialog.SetWindowTitle(_T("Edit Monitor Type"));
		editDialog.SetElementValue(element->m_type);
		int ret=editDialog.DoModal();
		if(ret == IDOK)
		{
			element->m_type=editDialog.GetMonitorType();
			if(m_pMaxMonitorType != NULL)
			{
				
				if(element->m_type.m_nWidth >= m_pMaxMonitorType->m_type.m_nWidth)
				{
					m_pMaxMonitorType=element;
					UpdateRectSize();
				}
				else
				{
					element->m_dSizeRatio=(double)element->m_type.m_nWidth/(double)m_pMaxMonitorType->m_type.m_nWidth;
				}
			}
			else
			{
				m_pMaxMonitorType=element;
				element->m_dSizeRatio=1.0;
			}

			Invalidate();
			if(g_pDesignerCtlWnd)
			{
				g_pDesignerCtlWnd->MonitorsUpdateType(element);
			}
			
		}
	}
}
void CMonitorTypeWnd::UpdateRectView()
{
	//the following code could work
	//but they also make the scrollbar blink
	int iLastBottom=0;
	m_iScrollHeight=600;
	CRect rect;
	GetClientRect(rect);
	int maxWidth=rect.Width()-LEFT_RIGHT_INTERVAL-LEFT_RIGHT_INTERVAL;
	int horizonCenter=(rect.left+rect.right)/2;
	std::list<MonitorTypeElement*>::iterator it;
	for(it=m_listMonitorTypes.begin();it != m_listMonitorTypes.end();++it)
	{
		int width=static_cast<int>(maxWidth*(*it)->m_dSizeRatio);
		int height=static_cast<int>(width*((double)(*it)->m_type.m_nHeight/(double)(*it)->m_type.m_nWidth));
		(*it)->m_rectView.left = horizonCenter-width/2;
		(*it)->m_rectView.right = horizonCenter+width/2;
		(*it)->m_rectView.top = iLastBottom+INTERVAL_HEIGHT;
		(*it)->m_rectView.bottom=(*it)->m_rectView.top+height;
		iLastBottom=(*it)->m_rectView.bottom;
		m_iScrollHeight += ((*it)->m_rectView.Height()+INTERVAL_HEIGHT);
	}
}

const MonitorTypeElement* CMonitorTypeWnd::GetDefaultType()const
{
	return m_pDefaultMonitorType;
}

void CMonitorTypeWnd::OnSetAsDefault()
{
	MonitorTypeElement* element=GetTypeInPoint(m_pointContexMenu);
	if(element)
	{
		m_pDefaultMonitorType=element;
		Invalidate();
	}
}

const std::list<MonitorTypeElement*>& CMonitorTypeWnd::GetTypes()const
{
	return m_listMonitorTypes;
}
void CMonitorTypeWnd::SetTypeDeletable(bool deletable)
{
	m_bTypeDeletable=deletable;
}


