#pragma once
#include "RotateRect.h"
class CMonitorControler
{
public:
	CMonitorControler(MonitorInfo* pInfo,double scale);
	~CMonitorControler(void);
	void Draw(CDC* pDC);
	void DrawHandle(CDC* pDC);
	void HightLightEdge();
	void SetScrollViewOffset(const CSize& offset);
	void SetViewOrigOffset(const CSize& offset);
	void SetViewScale(double dViewScale);
	void Move(int x,int y);
	void MoveTo(int x,int y);
	void MoveTo(const CPoint& pos);
	void Rotate(double angle); 
	void RotateTo(double newangle);
	void SetActive(bool active);
	CRgn* GetMyViewClientRgn();
	CRgn* GetMyViewHandleRgn();
	const MonitorInfo* GetMyMonitorInfo()const;
	const CPoint* GetMyViewCenter();
	bool PtInViewer(const CPoint& pt);
	void ChangeType(const MonitorTypeElement* pTypeElement);
	void UpdateType(const MonitorTypeElement* pTypeElement);
private:
	MonitorInfo* m_pMonitorInfo;
	RotateRect m_RRectViewer;
};

