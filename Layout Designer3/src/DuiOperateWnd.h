#pragma once
#include "..\\DuiLib\\UIlib.h"


class DuiOperateWnd:public DuiLib::CWindowWnd,DuiLib::INotifyUI
{
public:
	DuiOperateWnd() { };
	~DuiOperateWnd() { };
	LPCTSTR GetWindowClassName() const 
	{ 
		return _T("UIFrame"); 
	};
	void SetSkinFile(tString SSkinFile)
	{
		m_SSkinFile=SSkinFile;
	}
	UINT GetClassStyle() const 
	{ 
		return UI_CLASSSTYLE_FRAME | CS_DBLCLKS; 
	};
	void OnFinalMessage(HWND /*hWnd*/) 
	{ 
		delete this; 
	};

	void Notify(DuiLib::TNotifyUI& msg)
	{
		if( msg.sType == _T("click") ) 
		{
			if( msg.pSender->GetName() == _T("testbutton") ) 
			{
				//Close();
				MessageBox(this->GetHWND(),_T("Hello,Duilib!"),_T("test"),MB_OK);
			}
			else if(msg.pSender->GetName() == _T("changeskinbutton"))
			{
				DuiLib::CPaintManagerUI::SetResourcePath(DuiLib::CPaintManagerUI::GetInstancePath());
				m_pm.ReloadSkin();
			}
		}
	}

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		//deal message by myself
		if( uMsg == WM_CREATE )
		{
			m_pm.Init(m_hWnd);
			DuiLib::CDialogBuilder builder;
			DuiLib::CControlUI* pRoot = builder.Create(m_SSkinFile.c_str(), (UINT)0, NULL, &m_pm);
			ASSERT(pRoot && "Failed to parse XML");
			m_pm.AttachDialog(pRoot);
			m_pm.AddNotifier(this);
			return 0;
		}
		else if( uMsg == WM_DESTROY ) 
		{
			::PostQuitMessage(0);
		}else if(uMsg == WM_NCACTIVATE )
		{

			if( !::IsIconic(m_hWnd) ) //no icon
			{
				return (wParam == 0) ? TRUE : FALSE;
			}
		}
		else if( uMsg == WM_NCCALCSIZE ) 
		{
			return 0;
		}
		else if( uMsg == WM_NCPAINT ) 
		{
			return 0;
		}

		LRESULT lRes = 0;
		if( m_pm.MessageHandler(uMsg, wParam, lParam, lRes) ) //deal message by UIManager
			return lRes;
		return CWindowWnd::HandleMessage(uMsg, wParam, lParam);//deal message by window
	}

public:
	DuiLib::CPaintManagerUI m_pm;
	tString m_SSkinFile;
};