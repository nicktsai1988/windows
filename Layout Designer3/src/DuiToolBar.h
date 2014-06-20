#pragma once
#include "..\\Layout Designer\\Utils.h"
#include "..\Layout Designer\\DesignerCtlWnd.h"
#include "..\\DuiLib\\UIlib.h"


class DuiToolBar:public DuiLib::CWindowWnd,DuiLib::INotifyUI
{
public:
	DuiToolBar() { };
	~DuiToolBar(){ };

	void SetSkinFile(tString SSkinFile)
	{
		m_SSkinFile=SSkinFile;
	}
	LPCTSTR GetWindowClassName() const 
	{ 
		return _T("UIFrame"); 
	};
	UINT GetClassStyle() const 
	{ 
		return UI_CLASSSTYLE_FRAME | CS_DBLCLKS; 
	};
	void OnFinalMessage(HWND /*hWnd*/) 
	{ 
		delete this; 
	};

	void SetZoomText(const tString& text)
	{
		DuiLib::CComboUI* combozoom=static_cast<DuiLib::CComboUI*>(m_pm.FindControl(_T("combozoom")));
		if(combozoom != NULL)
		{
			combozoom->SetText(text.c_str());
		}
	}
	tString GetZoomText()
	{
		DuiLib::CComboUI* combozoom=static_cast<DuiLib::CComboUI*>(m_pm.FindControl(_T("combozoom")));
		if(combozoom != NULL)
		{
			return combozoom->GetText().GetData();
			//return combozoom->GetCurSel();
		}
		else
			return _T("");
	}

	void Notify(DuiLib::TNotifyUI& msg)
	{
		if( msg.sType == _T("click") ) 
		{
			if( msg.pSender->GetName() == _T("detectbutton") ) 
			{
				//Close();
				//MessageBox(this->GetHWND(),_T("Hello,Duilib!"),_T("test"),MB_OK);
				if(g_pDesignerCtlWnd && g_pDesignerCtlWnd->GetSafeHwnd())
				{
					::PostMessage(g_pDesignerCtlWnd->GetSafeHwnd(),WM_USER_DETECT,0,0);
					//msg.pSender->SetEnabled(false);
				}
			}
			else if(msg.pSender->GetName() == _T("savebutton"))
			{
				if(g_pDesignerCtlWnd && g_pDesignerCtlWnd->GetSafeHwnd())
				{
					::PostMessage(g_pDesignerCtlWnd->GetSafeHwnd(),WM_USER_SAVE,0,0);
				}
			} 
		}
		else
		{
			if((msg.sType == _T("itemselect")) && (msg.pSender->GetName() == _T("combozoom")))
			{
				DuiLib::CComboUI* pCombo=static_cast<DuiLib::CComboUI*>(msg.pSender);
				if(pCombo != NULL)
				{
					if(g_pDesignerCtlWnd && g_pDesignerCtlWnd->GetSafeHwnd())
					{
						::PostMessage(g_pDesignerCtlWnd->GetSafeHwnd(),WM_USER_ZOOM,pCombo->GetCurSel(),0);
					}
				}
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
	tString m_strZoom;
};

