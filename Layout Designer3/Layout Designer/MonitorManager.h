#pragma once
#include "RotateRect.h"
#include <vector>
#include "MonitorControler.h"
class MonitorManager
{
public:
	MonitorManager(CWnd* parent);
	~MonitorManager();
	void Init();
	void AddMonitor(MonitorInfo* pInfo);
	void Reset();
	CMonitorControler* GetMonitorControlerInPt(const CPoint& pt);
	void MonitorsUpdateType(const MonitorTypeElement* pElement);
	//message handler
	void OnDraw(CDC* pDC);
	void SetScrollViewOffset(const CSize& offset);
	void SetViewOrigOffset(const CSize& offset);
	void SetViewScale(double scale);
	void OnMouseMove(UINT nFlags, CPoint point,CWnd* pRulerWnd);
	void OnLButtonDown(UINT nFlags, CPoint point,CWnd* pRulerWnd);
	void OnRButtonDown(UINT nFlags, CPoint point);
	void OnLButtonUp(CWnd* pRulerWnd);
	bool OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	static double CalRotateAngel(const CPoint& center,const CPoint& now,const CPoint& orig);
private:
	
	CWnd* m_pWndParent;
	CMonitorControler* m_pCurrentControler;
	std::vector<CMonitorControler*> m_aMonitorsControler;

	std::vector<CMonitorControler*> m_aSelectedControler;
	RotateRect::POSITION m_ePos;
	bool m_bLButtonDown;
	double m_dViewScale;
	CPoint m_PointHandlePrev;
	CPoint m_PointRectPrev;
};