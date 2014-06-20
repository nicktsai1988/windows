#pragma once
#include "Utils.h"
class TiXmlElement;

class ExportInfo
{
public:
	ExportInfo(void);
	void SetData(const CanvasInfo* pcanvas,const std::vector<MonitorInfo>* pmonitors);
	bool SaveFile(const char* filename);
	~ExportInfo(void);
private:
	void SetCanvas(TiXmlElement* canvas,const CanvasInfo* info);
	void SetScreen(TiXmlElement* screen,const MonitorInfo* info);
	const CanvasInfo* m_pCanvas;
	const std::vector<MonitorInfo>* m_pMonitors;
};

