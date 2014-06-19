
#pragma once
#include "stdafx.h"
#include <Windows.h>
#include <vector>

BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor,HDC hdcMonitor,LPRECT lprcMonitor,LPARAM dwData);

typedef struct MONITOR_INFO
{
	HMONITOR hMonitor;
	RECT     MonitorRect;
}MONITOR_INFO; 

class MonitorInfo
{
	friend BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor,HDC hdcMonitor,LPRECT lprcMonitor,LPARAM dwData);
public:
	MonitorInfo(void);
	~MonitorInfo(void);

public:
	void GetMonitorInfo(std::vector<MONITOR_INFO>& monitorInfoList);

private:
	std::vector<MONITOR_INFO> m_monitorInfoList;
	RECT                      m_virtualPrimaryRect;
};
class Monitor
{
public:
	Monitor();
	bool LoadMonitorInfo(std::vector<MONITOR_INFO> &monitor_info_list,int width = -1, int height = -1);

private:
	void LoadMonitorInfoFromOS(std::vector<MONITOR_INFO> &monitor_info_list,int width = -1, int height = -1);

};
