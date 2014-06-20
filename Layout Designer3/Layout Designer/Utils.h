#pragma once
#include <vector>
#include <string>



class CDesignerCtlWnd;
class DuiStatusBar;
class DuiToolBar;
class CMonitorTypeWnd;


#define PI 3.1415926

struct MonitorType
{
	CString m_strDescription;
	INT m_nHorizonResolution,m_nVerticalResolution;
	INT m_nWidth,m_nHeight;
	INT m_nBorderLeft,m_nBorderRight;
	INT m_nBorderTop,m_nBorderBottom;
	COLORREF m_nBkColor;
};

struct MonitorOrigPos
{
	CPoint m_posInPixel;
	CPoint m_posInSize;
};
struct MonitorNewPos
{
	CPoint m_Points[4];
	CPoint m_Center;
	double m_dRotation;
	INT m_iHorizon;
	INT m_iVertical;
};
struct MonitorTypeElement;

struct MonitorInfo
{
	UINT m_nID;
	MonitorType m_type;
	MonitorOrigPos m_posOrig;
	MonitorNewPos m_posNew;
	const MonitorTypeElement* m_pTypeElement;
};
struct CanvasInfo
{
	int width,height;
	int horizon,vertical;
};

struct MonitorTypeElement
{
	CRect m_rectView;
	double m_dSizeRatio;
	MonitorType m_type;
};

extern DuiStatusBar* g_pDuiStatusBar;
void SetStatusText(const tString& strStatus);
void SetCanvasText(const tString& strCanvas);
void SetMonitorText(const tString& strMonitor);
void SetMonitorText(const MonitorInfo* info);

double GetRoundOffValue(double val);

extern DuiToolBar* g_pDuiToolBar;
void SetZoomText(const tString& text);
tString GetZoomText();

std::string CString2StdString(const CString& cstr);
CString StdString2CString(const std::string& str);

extern CDesignerCtlWnd* g_pDesignerCtlWnd;
extern CMonitorTypeWnd* g_pMonitorTypeWnd;



//CSize CalCanvasSize(const std::vector<MonitorInfo* >& monitorinfo);
//double CalRotateAngel(const CPoint& center,const CPoint& now,const CPoint& orig);
//CPoint PointRotate(const CPoint& center ,const CPoint& orig);