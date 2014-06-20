// RulerCtlWnd.cpp : implementation file
//

#include "stdafx.h"
#include "Layout Designer.h"
#include "RulerCtlWnd.h"


// CRulerCtlWnd

IMPLEMENT_DYNAMIC(CRulerCtlWnd, CWnd)

CRulerCtlWnd::CRulerCtlWnd():m_wndDesigner(this)
{
	m_Zoom=0.0;
	m_BkColor=RGB(192,192,192);
	m_HoriRulerBkColor=RGB(166,202,240);
	m_VertRulerBkColor=RGB(166,202,240);
	InitRulerFont();
}

CRulerCtlWnd::~CRulerCtlWnd()
{
	m_RulerFont.DeleteObject();
}


BEGIN_MESSAGE_MAP(CRulerCtlWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


BOOL CRulerCtlWnd::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CRulerCtlWnd::Update()
{
	OnPaint();
}

void CRulerCtlWnd::InitRulerFont()
{
	m_RulerFont.CreateFont(10,0,0,0,FW_NORMAL,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_SWISS,_T("Arial"));
}

void CRulerCtlWnd::SetBkColor(int color)
{
	m_BkColor=color;
}
void CRulerCtlWnd::SetRulerBkColor(int HorizonColor,int VerticalColor)
{
	m_HoriRulerBkColor=HorizonColor;
	m_VertRulerBkColor=VerticalColor;
}

/*
void CRulerCtlWnd::DrawHorizonRulerCursor(int x,bool drawCursor)
{
	CClientDC dc(this);
	DrawHorizonRuler(&dc);
	if((x >= m_RectHorizonRuler.left) && (x < m_RectHorizonRuler.right))
	{
		
		if(drawCursor)
		{
			CPen pen;
			pen.CreatePen(PS_DOT,1,RGB(255,0,0));
			dc.SelectObject(&pen);

			dc.MoveTo(x,HORIZON_RULER_TOP);
			dc.LineTo(x,HORIZON_RULER_BOTTOM);
			pen.DeleteObject();
		}
	}
}
void CRulerCtlWnd::DrawVerticalRulerCursor(int y,bool drawCursor)
{
	CClientDC dc(this);
	DrawVerticalRuler(&dc);
	if((y >= m_RectVerticalRuler.top) &&(y < m_RectVerticalRuler.bottom))
	{
		
		if(drawCursor)
		{
			CPen pen;
			pen.CreatePen(PS_DOT,1,RGB(255,0,0));
			dc.SelectObject(&pen);

			dc.MoveTo(VERTICAL_RULER_LEFT,y);
			dc.LineTo(VERTICAL_RULER_RIGHT,y);
			pen.DeleteObject();
		}
	}
}
*/
void CRulerCtlWnd::DrawRulerCursor(int x,int y,bool drawCursor/* =true */)
{
	CClientDC clientDC(this);
	//CClientDC dc(this);
	CMemDC::m_bUseMemoryDC=TRUE;
	CMemDC memDC1(clientDC,m_RectHorizonRuler);
	CMemDC memDC2(clientDC,m_RectVerticalRuler);
	CDC& dc1=memDC1.GetDC();
	CDC& dc2=memDC2.GetDC();

	//CBrush brush;
	//CRect clientRect;
	//brush.CreateSolidBrush(m_BkColor);
	//dc.FillRect(clientRect,&brush);
	//brush.DeleteObject();

	DrawHorizonRuler(&dc1);
	DrawVerticalRuler(&dc2);

	

	if(drawCursor)
	{
		if(((x >= m_RectHorizonRuler.left) && (x < m_RectHorizonRuler.right))
			&&((y >= m_RectVerticalRuler.top) &&(y < m_RectVerticalRuler.bottom)))
		{
		
				CPen pen;
				pen.CreatePen(PS_DOT,1,RGB(255,0,0));
				dc1.SelectObject(&pen);
				dc2.SelectObject(&pen);

				dc1.MoveTo(x,HORIZON_RULER_TOP);
				dc1.LineTo(x,HORIZON_RULER_BOTTOM);

				dc2.MoveTo(VERTICAL_RULER_LEFT,y);
				dc2.LineTo(VERTICAL_RULER_RIGHT,y);
				pen.DeleteObject();
		
		}
	}
}


// CRulerCtlWnd message handlers

void CRulerCtlWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CWnd::OnPaint() for painting messages
	OnPrepareDC(&dc);
	OnDraw(&dc);
}

void CRulerCtlWnd::OnDraw(CDC *pDC)
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

	DrawHorizonRuler(pDrawDC);
	DrawVerticalRuler(pDrawDC);
	if(bMemDC)
	{
		pDC->BitBlt(clientRect.left,clientRect.top,clientRect.Width(),clientRect.Height(),
			&memDC,0,0,SRCCOPY);
		memDC.DeleteDC();
		memBitmap.DeleteObject();
	}
}

void CRulerCtlWnd::DrawHorizonRuler(CDC* pDC)
{
	
	//draw ruler
	CBrush brush;
	m_wndDesigner.GetWindowRect(m_RectHorizonRuler);
	m_RectHorizonRuler.top=HORIZON_RULER_TOP;
	m_RectHorizonRuler.bottom=HORIZON_RULER_BOTTOM;
	m_RectHorizonRuler.left=VERTICAL_RULER_RIGHT ;
	m_RectHorizonRuler.right += (VERTICAL_RULER_RIGHT-2);
	brush.CreateSolidBrush(m_HoriRulerBkColor);
	pDC->FillRect(m_RectHorizonRuler,&brush);
	brush.DeleteObject();
	
	//draw mark rect
	CRect markRect=m_wndDesigner.GetCanvasViewRect();
	markRect.left += 2;
	markRect.left += VERTICAL_RULER_RIGHT;
	if(markRect.left < VERTICAL_RULER_RIGHT)
		markRect.left = VERTICAL_RULER_RIGHT;
	markRect.right += 2;
	markRect.right += VERTICAL_RULER_RIGHT;
	markRect.top=HORIZON_RULER_TOP;
	markRect.bottom=HORIZON_RULER_BOTTOM;
	brush.CreateSolidBrush(RGB(255,255,255));
	pDC->FillRect(markRect,&brush);
	brush.DeleteObject();
	//draw mark
	CRect canvasRect=m_wndDesigner.GetCanvasViewRect();
	double left=canvasRect.left+VERTICAL_RULER_RIGHT;
	double right=canvasRect.right+VERTICAL_RULER_RIGHT;
	int orig=m_wndDesigner.GetMonitorViewOrig().x+VERTICAL_RULER_RIGHT;

	//grid size
	double width=m_wndDesigner.GetGridViewSize();
	//int nstep=static_cast<int>(width+0.5);
	double num;
	int i;
	CString strNum;
	pDC->SelectObject(&m_RulerFont);

	double scale=m_wndDesigner.GetViewScale();
	for(num = orig;static_cast<int>(num) < static_cast<int>(right);num += width)
	{
		i=static_cast<int>(num+0.5);
		if(i >= VERTICAL_RULER_RIGHT)
		{	
			pDC->MoveTo(i+2,HORIZON_RULER_TOP);
			pDC->LineTo(i+2,HORIZON_RULER_TOP+8);
			DrawHorizonMiniMark(pDC,i+2,width,left,right,add);
			strNum.Format(_T("%d"),(int)((num-orig)/scale+0.5));
			CRect numRect;
			pDC->DrawText(strNum,&numRect,DT_CALCRECT);
			numRect.OffsetRect(i,0);
			numRect.OffsetRect(-numRect.Width()/2+2,HORIZON_RULER_BOTTOM-numRect.Height()-3);
			if(numRect.left >= VERTICAL_RULER_RIGHT)
			{
				pDC->DrawText(strNum,&numRect,DT_SINGLELINE|DT_VCENTER|DT_CENTER);
			}
		}
		//TODO::
		
	}
	DrawHorizonMiniMark(pDC,orig+2,width,left,right,minus);
	for(num=orig-width; static_cast<int>(num) > static_cast<int>(left);num -= width)
	{
		i=static_cast<int>(num-0.5);
		if(i>=VERTICAL_RULER_RIGHT)
		{	
			pDC->MoveTo(i+2,HORIZON_RULER_TOP);
			pDC->LineTo(i+2,HORIZON_RULER_TOP+8);
			DrawHorizonMiniMark(pDC,i+2,width,left,right,minus);
			strNum.Format(_T("%d"),(int)((num-orig)/scale-0.5));
			CRect numRect;
			pDC->DrawText(strNum,&numRect,DT_CALCRECT);
			numRect.OffsetRect(i,0);
			numRect.OffsetRect(-numRect.Width()/2+2,HORIZON_RULER_BOTTOM-numRect.Height()-3);
			if(numRect.left >= VERTICAL_RULER_RIGHT)
			{
				pDC->DrawText(strNum,&numRect,DT_SINGLELINE|DT_VCENTER|DT_CENTER);
			}
		//TODO::
		}	
	}
	pDC->DrawEdge(m_RectHorizonRuler,BF_DIAGONAL_ENDTOPRIGHT,BF_LEFT|BF_TOP|BF_BOTTOMRIGHT);
}
void CRulerCtlWnd::DrawVerticalRuler(CDC* pDC)
{
	
	CBrush brush;
	m_wndDesigner.GetWindowRect(m_RectVerticalRuler);
	m_RectVerticalRuler.top = HORIZON_RULER_BOTTOM ;
	m_RectVerticalRuler.bottom += (HORIZON_RULER_BOTTOM -2);
	m_RectVerticalRuler.right=VERTICAL_RULER_RIGHT;
	m_RectVerticalRuler.left=VERTICAL_RULER_LEFT;
	brush.CreateSolidBrush(m_VertRulerBkColor);
	pDC->FillRect(m_RectVerticalRuler,&brush);
	brush.DeleteObject();
	//draw mark rect
	CRect markRect=m_wndDesigner.GetCanvasViewRect();
	markRect.top += 2;
	markRect.top += HORIZON_RULER_BOTTOM;
	if(markRect.top < HORIZON_RULER_BOTTOM)
		markRect.top = HORIZON_RULER_BOTTOM;
	markRect.bottom += 2;
	markRect.bottom+= HORIZON_RULER_BOTTOM;
	markRect.left=VERTICAL_RULER_LEFT;
	markRect.right=VERTICAL_RULER_RIGHT;
	brush.CreateSolidBrush(RGB(255,255,255));
	pDC->FillRect(markRect,&brush);
	brush.DeleteObject();
	//draw mark
	
	CRect canvasRect=m_wndDesigner.GetCanvasViewRect();
	double top=canvasRect.top+HORIZON_RULER_BOTTOM;
	double bottom=canvasRect.bottom+HORIZON_RULER_BOTTOM;
	int orig=m_wndDesigner.GetMonitorViewOrig().y + HORIZON_RULER_BOTTOM;

	double width=m_wndDesigner.GetGridViewSize();
	//int nstep=static_cast<int>(width+0.5);
	int j;
	double num;
	CString strNum;
	pDC->SelectObject(&m_RulerFont);
	double scale=m_wndDesigner.GetViewScale();

	for(num=orig;static_cast<int>(num) < static_cast<int>(bottom);num += width)
	{
		j=static_cast<int>(num+0.5);
		if(j>=HORIZON_RULER_BOTTOM)
		{
			pDC->MoveTo(VERTICAL_RULER_LEFT,j+1);
			pDC->LineTo(VERTICAL_RULER_LEFT+8,j+1);
			DrawVerticalMiniMark(pDC,j+1,width,top,bottom,add);
			strNum.Format(_T("%d"),static_cast<int>((num-orig)/scale+0.5));

			CRect numRect;
			pDC->DrawText(strNum,&numRect,DT_CALCRECT);
			numRect.OffsetRect(0,j);
			numRect.OffsetRect(VERTICAL_RULER_RIGHT-numRect.Width()-3,-numRect.Height()/2+1);
			if(numRect.top >= HORIZON_RULER_BOTTOM)
			{
				pDC->DrawText(strNum,&numRect,DT_SINGLELINE|DT_VCENTER|DT_CENTER);
			}
		}
		//TODO::
	}
	DrawVerticalMiniMark(pDC,orig+1,width,top,bottom,minus);
	for(num=orig-width;static_cast<int>(num) > static_cast<int>(top);num -= width)
	{
		j=static_cast<int>(num-0.5);
		if(j >= HORIZON_RULER_BOTTOM)
		{
			pDC->MoveTo(VERTICAL_RULER_LEFT,j+1);
			pDC->LineTo(VERTICAL_RULER_LEFT+8,j+1);
			DrawVerticalMiniMark(pDC,j+1,width,top,bottom,minus);
			strNum.Format(_T("%d"),static_cast<int>((num-orig)/scale-0.5));
			CRect numRect;
			pDC->DrawText(strNum,&numRect,DT_CALCRECT);
			numRect.OffsetRect(0,j);
			numRect.OffsetRect(VERTICAL_RULER_RIGHT-numRect.Width()-3,-numRect.Height()/2+1);
			if(numRect.top >= HORIZON_RULER_BOTTOM)
			{
				pDC->DrawText(strNum,&numRect,DT_SINGLELINE|DT_VCENTER|DT_CENTER);
			}
		}
		//TODO::
	}
	pDC->DrawEdge(m_RectVerticalRuler,BF_DIAGONAL_ENDTOPRIGHT,BF_LEFT|BF_TOP|BF_BOTTOMRIGHT);
}

void CRulerCtlWnd::OnPrepareDC(CDC *pDC)
{

}


void CRulerCtlWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if(m_wndDesigner.GetSafeHwnd())
	{
		m_wndDesigner.MoveWindow(VERTICAL_RULER_RIGHT,HORIZON_RULER_BOTTOM,cx-VERTICAL_RULER_RIGHT,cy-HORIZON_RULER_BOTTOM);
	}
}


int CRulerCtlWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	if (!m_wndDesigner.Create(NULL, NULL,  WS_CHILD | WS_VISIBLE |WS_HSCROLL | WS_VSCROLL, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST+3, NULL))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}
	g_pDesignerCtlWnd=&m_wndDesigner;
	//m_wndDesigner.SetWndScrollSize(CSize(1024,768));
	m_wndDesigner.SetActiveWindow();
	return 0;
}


void CRulerCtlWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CWnd::OnMouseMove(nFlags, point);
}


BOOL CRulerCtlWnd::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return TRUE;
}

void CRulerCtlWnd::DrawHorizonMiniMark(CDC* pDC,int x,double width,int min,int max,OperateFunc pFunc)
{
	int pos;
	if(width < 50)
	{
		pos=static_cast<int>(pFunc(x,width/2.0));
		if((pos>=min) && (pos<=max))
		{
			pDC->MoveTo(pos,HORIZON_RULER_TOP);
			pDC->LineTo(pos,HORIZON_RULER_TOP+5);
		}
		return;
	}
	else
	{
		double ministep=width/5.0;
		for(int i=0;i<5;++i)
		{
			pos=static_cast<int>(pFunc(x,ministep*i));
			if((pos>=min) && (pos<=max))
			{
				pDC->MoveTo(pos,HORIZON_RULER_TOP);
				pDC->LineTo(pos,HORIZON_RULER_TOP+5);
			}
			else
			{
				return;
			}
		}
	}
}

void CRulerCtlWnd::DrawVerticalMiniMark(CDC* pDC,int y,double width,int min,int max,OperateFunc pFunc)
{
	int pos;
	if(width < 50)
	{
		pos=static_cast<int>(pFunc(y,width/2.0));
		if((pos>=min) &&(pos<=max))
		{
			pDC->MoveTo(VERTICAL_RULER_LEFT,pos);
			pDC->LineTo(VERTICAL_RULER_LEFT+5,pos);
		}
		return;
	}
	else
	{
		double ministep=width/5.0;
		for(int i=0;i<5;++i)
		{
			pos=static_cast<int>(pFunc(y,ministep*i));
			if((pos>=min) && (pos<=max))
			{
				pDC->MoveTo(VERTICAL_RULER_LEFT,pos);
				pDC->LineTo(VERTICAL_RULER_LEFT+5,pos);
			}
			else
			{
				return;
			}
		}
	}
}

double CRulerCtlWnd::add(double x,double y)
{
	return x+y;
}
double CRulerCtlWnd::minus(double x,double y)
{
	return x-y;
}
