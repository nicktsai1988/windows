#include "StdAfx.h"
#include "Monitor.h"
#include <d3d9.h>

#pragma comment(lib,"d3d9.lib")
BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor,HDC hdcMonitor,LPRECT lprcMonitor,LPARAM dwData)
{
	MONITOR_INFO monitorInfo;

	monitorInfo.hMonitor =  hMonitor;
	monitorInfo.MonitorRect = *lprcMonitor;

	((MonitorInfo*)dwData)->m_monitorInfoList.push_back(monitorInfo);

	((MonitorInfo*)dwData)->m_virtualPrimaryRect.left   = min(lprcMonitor->left, ((MonitorInfo*)dwData)->m_virtualPrimaryRect.left);
	((MonitorInfo*)dwData)->m_virtualPrimaryRect.top    = min(lprcMonitor->top, ((MonitorInfo*)dwData)->m_virtualPrimaryRect.top);

	((MonitorInfo*)dwData)->m_virtualPrimaryRect.right  = max(lprcMonitor->right, ((MonitorInfo*)dwData)->m_virtualPrimaryRect.right);
	((MonitorInfo*)dwData)->m_virtualPrimaryRect.bottom = max(lprcMonitor->bottom, ((MonitorInfo*)dwData)->m_virtualPrimaryRect.bottom);


	return TRUE;
}

MonitorInfo::MonitorInfo(void)
{
}


MonitorInfo::~MonitorInfo(void)
{
}

void MonitorInfo::GetMonitorInfo(std::vector<MONITOR_INFO>& monitorInfoList)
{
	m_monitorInfoList.clear();

	m_virtualPrimaryRect.left   = 0x7FFFFFFF;
	m_virtualPrimaryRect.top    = 0x7FFFFFFF;
	m_virtualPrimaryRect.right  = 0x80000000;
	m_virtualPrimaryRect.bottom = 0x80000000;

	EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, (LPARAM)this);
	monitorInfoList = m_monitorInfoList;
}


Monitor::Monitor()
{

}


void RecomputeMonitorInfo(std::vector<MONITOR_INFO>& miList , int unitMonWidth, int unitMonHeight)
{
	std::vector<MONITOR_INFO> tempList1 = miList;
	std::vector<MONITOR_INFO> tempList2;

	int leftMostValue = 0;
	int topMostValue  = 0;

	std::vector<MONITOR_INFO>::iterator it;

	for (it = tempList1.begin(); it != tempList1.end(); ++it)
	{
		if (leftMostValue > it->MonitorRect.left)
		{
			leftMostValue = it->MonitorRect.left;
		}

		if (topMostValue > it->MonitorRect.top)
		{
			topMostValue = it->MonitorRect.top;
		}
	}

	for (it = tempList1.begin(); it != tempList1.end(); ++it)
	{
		bool bLeftMost = false;
		bool bTopMost = false;

		if (leftMostValue == it->MonitorRect.left)
		{
			bLeftMost = true;
		}

		if (topMostValue == it->MonitorRect.top)
		{
			bTopMost = true;
		}

		int xCells   = 1;
		int yCells   = 1;

		float width  = (float)(it->MonitorRect.right - it->MonitorRect.left);
		float height = (float)(it->MonitorRect.bottom - it->MonitorRect.top);
		float ratio  = width /height;

		float mRatio = (float)unitMonWidth/(float)unitMonHeight;
		float mLRatio = mRatio*0.878f;
		float mHRatio = mRatio*1.1f;

		if(ratio >= mLRatio && ratio < mHRatio)
		{
			if(width > 1.8* unitMonWidth && width < 2.7* unitMonWidth)
			{
				xCells = 2;
				yCells = 2;
			}
			else if (width > 2.7 * unitMonWidth && width < 3.6 * unitMonWidth)
			{
				xCells = 3;
				yCells = 3;
			}
			else if (width > 3.6 * unitMonWidth && width < 4.5 * unitMonWidth)
			{
				xCells = 4;
				yCells = 4;
			}
		}
		else if(ratio >= 1.33333 * mLRatio && ratio < 1.5 * mHRatio)
		{
			if (width > 2.7 * unitMonWidth && width < 3.6 * unitMonWidth)
			{
				xCells = 3;
				yCells = 2;
			}
			else if (width > 3.6 * unitMonWidth && width < 4.5 * unitMonWidth)
			{
				xCells = 4;
				yCells = 3;
			}			
		}
		else if(ratio >= 2*mLRatio && ratio < 2*mHRatio)
		{
			if(width > 1.8* unitMonWidth && width < 2.7* unitMonWidth)
			{
				xCells = 2;
				yCells = 1;
			}
			else if (width > 3.6 * unitMonWidth && width < 4.5 * unitMonWidth)
			{
				xCells = 4;
				yCells = 2;
			}
		}
		else if(ratio >= 3*mLRatio && ratio < 3*mHRatio)
		{
			xCells = 3;
		}
		else if(ratio >= 4*mLRatio && ratio < 4*mHRatio)
		{
			xCells = 4;
		}
		else if (ratio >= mLRatio/1.5 && ratio < mHRatio/1.333333)
		{
			if(width > 1.8* unitMonWidth && width < 2.7* unitMonWidth)
			{
				xCells = 2;
				yCells = 3;
			}
			else if (width > 2.7 * unitMonWidth && width < 3.6 * unitMonWidth)
			{
				xCells = 3;
				yCells = 4;
			}		
		}
		else if(ratio >= mLRatio/2 && ratio < mHRatio/2)
		{
			if(width > 0.9* unitMonWidth && width < 1.8* unitMonWidth)
			{
				xCells = 1;
				yCells = 2;
			}
			else if(width > 1.8* unitMonWidth && width < 2.7* unitMonWidth)
			{
				xCells = 2;
				yCells = 4;
			}
		}
		else if(ratio >= mLRatio/3 && ratio < mHRatio/3)
		{
			yCells = 3;
		}
		else if(ratio >= mLRatio/4 && ratio < mHRatio/4)
		{
			yCells = 4;
		}


		float xBorder = 0.0f;
		float yBorder = 0.0f;

		if (xCells > 1 && bLeftMost)
		{
			xBorder = (width - unitMonWidth * xCells) / (xCells - 1);
		}
		else
		{
			xBorder = (width - unitMonWidth * xCells) / xCells;
		}

		if (yCells > 1 && bTopMost)
		{
			yBorder = (height - unitMonHeight * yCells) / (yCells - 1);
		}
		else
		{
			yBorder = (height - unitMonHeight * yCells) / yCells;
		}

		float newWidht  = width / xCells;
		float newHeight = height / yCells;

		for (int i = 0; i < xCells; ++i)
		{
			for (int j = 0; j < yCells; ++j)
			{
				MONITOR_INFO newMI;
				newMI.hMonitor = it->hMonitor;

				if (bLeftMost)
				{
					newMI.MonitorRect.left = it->MonitorRect.left + i * (unitMonWidth + xBorder);
				}
				else
				{
					newMI.MonitorRect.left   = it->MonitorRect.left + i * (unitMonWidth + xBorder) + xBorder;
				}

				if (bTopMost)
				{
					newMI.MonitorRect.top  = it->MonitorRect.top + j * (unitMonHeight + yBorder);
				}
				else
				{
					newMI.MonitorRect.top    = it->MonitorRect.top + j * (unitMonHeight + yBorder) + yBorder;
				}


				newMI.MonitorRect.right  = newMI.MonitorRect.left + unitMonWidth;
				newMI.MonitorRect.bottom = newMI.MonitorRect.top + unitMonHeight;			

				tempList2.push_back(newMI);
			}
		}
	}

	miList = tempList2;
}

void Monitor::LoadMonitorInfoFromOS(std::vector<MONITOR_INFO> &monitor_info_list,int width, int height)
{
	MonitorInfo mi;
	mi.GetMonitorInfo(monitor_info_list);

	if (width > 0 && height > 0)
		RecomputeMonitorInfo(monitor_info_list, width, height);	
}

typedef int (*LPFN_LOAD_MONITOR_INFO_SIZE)(int *pSize, BOOL bIsReload);
typedef int (*LPFN_LOAD_MONITOR_INFO)(RECT *pCells);

bool Monitor::LoadMonitorInfo(std::vector<MONITOR_INFO> &monitor_info_list,int width, int height)
{
	bool ret = true;

	monitor_info_list.clear();

	D3DADAPTER_IDENTIFIER9 AdapterIdentify;

	IDirect3D9* pD3D9 = Direct3DCreate9(D3D_SDK_VERSION);
	if(NULL == pD3D9)
	{
		ret=false;
	}
	else
	{
		pD3D9->GetAdapterIdentifier(0,0,&AdapterIdentify);


		if((AdapterIdentify.VendorId != 0x5333) && (AdapterIdentify.VendorId != 0x00001106))
		{
			LoadMonitorInfoFromOS(monitor_info_list, width, height);
		}
		else
		{	
			HMODULE	hLib = LoadLibrary(TEXT("S3BezelSetting.dll"));
			if (hLib == NULL)
			{
				ret = false;
			}
			else
			{
				LPFN_LOAD_MONITOR_INFO pfnLoadMonitorInfo          = (LPFN_LOAD_MONITOR_INFO)GetProcAddress( hLib, "LoadMonitorInfo" );
				LPFN_LOAD_MONITOR_INFO_SIZE pfnLoadMonitorInfoSize = (LPFN_LOAD_MONITOR_INFO_SIZE)GetProcAddress( hLib, "LoadMonitorInfoSize" );

				if (pfnLoadMonitorInfo == NULL || pfnLoadMonitorInfoSize == NULL)
				{
					FreeLibrary( hLib );
					ret = false;
				}
				else
				{
					int size = 0;
					pfnLoadMonitorInfoSize(&size, FALSE);
					if (size <= 0)
					{
						ret = false;
					}
					else
					{
						RECT* rects = new RECT[size];
						pfnLoadMonitorInfo(rects);

						for (int i = 0; i < size; ++i)
						{
							MONITOR_INFO mi;
							mi.MonitorRect.left   = rects[i].left;
							mi.MonitorRect.right  = rects[i].right + 1;
							mi.MonitorRect.top    = rects[i].top;
							mi.MonitorRect.bottom = rects[i].bottom + 1;

							monitor_info_list.push_back(mi);
						}

						delete[] rects;
						FreeLibrary(hLib);
						ret = true;
					}				
				}
			}
		}
	}
	if(pD3D9)
	{
		pD3D9->Release();
		pD3D9 = NULL;
	}

	return ret;
}