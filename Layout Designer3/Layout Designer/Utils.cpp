#include "StdAfx.h"
#include "Utils.h"
#include <math.h>
#include "DesignerCtlWnd.h"
#include "DuiStatusBar.h"
#include "DuiToolBar.h"
#include "MonitorTypeWnd.h"

/*
double CalRotateAngel(const CPoint& center,const CPoint& now,const CPoint& orig)
{
	double angel1,angel2;
	angel1=atan2((double(orig.y-center.y)),(double(orig.x-center.x)));
	angel2=atan2((double(now.y-center.y)),(double(now.x-center.x)));
	return ((angel1-angel2)/PI)*180 ;
}
*/

DuiStatusBar* g_pDuiStatusBar=NULL;
DuiToolBar* g_pDuiToolBar=NULL;
CDesignerCtlWnd* g_pDesignerCtlWnd=NULL;
CMonitorTypeWnd* g_pMonitorTypeWnd=NULL;

void SetStatusText(const tString& strStatus)
{
	if(g_pDuiStatusBar != NULL)
		g_pDuiStatusBar->SetStatusText(strStatus);
}
void SetCanvasText(const tString& strCanvas)
{
	if(g_pDuiStatusBar != NULL)
		g_pDuiStatusBar->SetCanvasText(strCanvas);
}
void SetMonitorText(const tString& strMonitor)
{
	if(g_pDuiStatusBar != NULL)
	{
		tString str(_T("Monitor Info:   "));
		g_pDuiStatusBar->SetMonitorText(str+strMonitor);
	}
}

void SetMonitorText(const MonitorInfo* info)
{
	ASSERT(info);
	if(NULL == g_pDuiStatusBar)
		return;
	TCHAR str[512];
	_stprintf(str,_T("Type= %s,ID= %u,X= %d,Y= %d,Angel= %f"),
		info->m_type.m_strDescription,
		info->m_nID,
		info->m_posNew.m_Center.x,
		info->m_posNew.m_Center.y,
		info->m_posNew.m_dRotation);
	SetMonitorText(str);
}

double GetRoundOffValue(double val)
{
	int x=static_cast<int>(val*100.0+5.0);
	return (static_cast<double>(x/10))/10.0;
}

void SetZoomText(const tString& text)
{
	if(g_pDuiToolBar != NULL)
		g_pDuiToolBar->SetZoomText(text);
}
tString GetZoomText()
{
	if(g_pDuiToolBar != NULL)
		return g_pDuiToolBar->GetZoomText();
	else
		return _T("");
}




std::string CString2StdString(const CString& cstr)
{
	CStringA str;
	str=cstr;
	return str.GetBuffer();
}
CString StdString2CString(const std::string& str)
{
	CString cstr;
	cstr.Format(_T("%s"),str.c_str());
	return cstr;
}

/*
CSize CalCanvasSize(const std::vector<MonitorInfo* >& monitorinfo)
{
	UINT nwidth=0,nheight=0;
	return CSize(nwidth,nheight);
}
*/