#include "StdAfx.h"
#include "Canvas.h"
#include "DesignerCtlWnd.h"


Canvas::Canvas(CWnd * pParent)
{
	m_pWndParent=pParent;
	m_nBackColor=RGB(255,255,255);
	m_nGridColor=RGB(166,202,240);
	m_dScale=1.0;
}

Canvas::~Canvas(void)
{
}
void Canvas::SetSize(UINT width,UINT height)
{
	m_CanvasViewSize=CSize(width,height);
}

void Canvas::SetScrollViewOffset(const CSize& offset)
{
	m_sizeScrollViewOffset=offset;
}
const CSize& Canvas::GetSize()
{
	return m_CanvasViewSize;
}

const CRect& Canvas::GetCanvasViewRect()
{
	UpdateCanvas();
	return m_CanvasViewRect;
}
double Canvas::GetGridViewSize()
{
	UpdateCanvas();
	return m_dGridSize;
}
const CPoint& Canvas::GetMonitorViewOrig()
{
	UpdateCanvas();
	return m_MonitorViewOrig;
}
void Canvas::SetViewScale(double dScale)
{
	m_dScale=dScale;
}
void Canvas::Draw(CDC *pDC)
{
	UpdateCanvas();
	DrawCanvas(pDC);
	DrawGrid(pDC);
}
void Canvas::DrawCanvas(CDC* pDC)
{
	CBrush brush;
	brush.CreateSolidBrush(m_nBackColor);
	pDC->FillRect(m_CanvasViewRect,&brush);
	brush.DeleteObject();
}
void Canvas::DrawGrid(CDC *pDC)
{
	CPen pen;
	pen.CreatePen(PS_DOT,1,m_nGridColor);
	pDC->SelectObject(&pen);

	CPoint monitorOrig=GetMonitorViewOrig();
	double left=m_CanvasViewRect.left;
	double right=m_CanvasViewRect.right;
	double top=m_CanvasViewRect.top;
	double bottom=m_CanvasViewRect.bottom;

	//horizon line
	for(double x = monitorOrig.x;x < right; x += m_dGridSize)
	{
		pDC->MoveTo(static_cast<int>(x+0.5),m_CanvasViewRect.top);
		pDC->LineTo(static_cast<int>(x+0.5),m_CanvasViewRect.bottom);
	}
	for(double x = (monitorOrig.x-m_dGridSize); x > left; x -= m_dGridSize)
	{
		pDC->MoveTo(static_cast<int>(x-0.5),m_CanvasViewRect.top);
		pDC->LineTo(static_cast<int>(x-0.5),m_CanvasViewRect.bottom);
	}
	//vertical line
	for(double y= monitorOrig.y;y < bottom; y+= m_dGridSize)
	{
		pDC->MoveTo(m_CanvasViewRect.left,static_cast<int>(y+0.5));
		pDC->LineTo(m_CanvasViewRect.right,static_cast<int>(y+0.5));
	}
	for(double y= (monitorOrig.y-m_dGridSize);y > top; y-= m_dGridSize)
	{
		pDC->MoveTo(m_CanvasViewRect.left,static_cast<int>(y-0.5));
		pDC->LineTo(m_CanvasViewRect.right,static_cast<int>(y-0.5));
	}
	pen.DeleteObject();
}
void Canvas::UpdateCanvas()
{
	int width,height;
	CRect rect;
	CDesignerCtlWnd *pDesignerParent=(CDesignerCtlWnd*)m_pWndParent;
	pDesignerParent->GetClientRect(rect);
	CSize scrollSize=pDesignerParent->GetWndScrollSize();

	if(rect.Width() > scrollSize.cx)
		width = rect.Width();
	else
		width= scrollSize.cx;

	if(rect.Height() > scrollSize.cy)
		height = rect.Height();
	else
		height = scrollSize.cy;

	rect.left = (width-m_CanvasViewSize.cx)/2-m_sizeScrollViewOffset.cx;
	rect.top = (height-m_CanvasViewSize.cy)/2-m_sizeScrollViewOffset.cy;
	rect.right=rect.left+m_CanvasViewSize.cx;
	rect.bottom=rect.top+m_CanvasViewSize.cy;
	//rect += m_sizeViewOrigOffset;
	m_CanvasViewRect=rect;

	m_MonitorViewOrig=CPoint(m_CanvasViewRect.left+m_CanvasViewSize.cx/4,m_CanvasViewRect.top+m_CanvasViewSize.cy/4);
	CalGridSize();
}

void Canvas::CalGridSize()
{
/*	
	//test begin
	//m_ViewSize=(CSize(1280,1024));
	double m_ViewCanvasRatio=1.0;
	//test end
	int divisor=CalGreatestCommonDivisor(m_CanvasViewSize.cx,m_CanvasViewSize.cy);
	int physicSize=divisor,viewSize=(divisor/m_ViewCanvasRatio);
	int index=2;
	int result=viewSize;
	while(true)
	{
		physicSize=divisor/index;
		viewSize=physicSize/m_ViewCanvasRatio;

		if(viewSize < MIN_GRID_VIEW_SIZE)
			break;
		else
		{
			if((divisor % index) == 0)
			{

				result=viewSize;
			}
			++index;
		}
	}
	if(result < MIN_GRID_VIEW_SIZE)
		result=MIN_GRID_VIEW_SIZE;
	m_GridViewSize=CSize(result,result);
	*/
	
	double basic_size=100*m_dScale;
	while(basic_size < 20.0)
	{
		basic_size*=2;
	}
	m_dGridSize=basic_size;
}

int Canvas::CalGreatestCommonDivisor(int number1,int number2)
{
	if(!(number1 && number2))
		return 0;
	int remainder;
	remainder=number1 % number2;
	while(remainder != 0)
	{
		number1=number2;
		number2=remainder;
		remainder=number1%number2;
	}
	return number2;
}


