// dialog.cpp
//
#include "StdAfx.h"
#include "Dialog.h"
 
Dialog::Dialog()
{
	m_hDlg = NULL;
}

Dialog::~Dialog()
{
	Destroy();
}

LRESULT Dialog_DlgProcStub(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	Dialog *dlg = NULL;

	if (uMsg == WM_INITDIALOG)
	{
		dlg = (Dialog*)lParam;
		SetWindowLong(hDlg, GWL_USERDATA, (LONG)dlg);
	}

	dlg = (Dialog*)GetWindowLong(hDlg, GWL_USERDATA);

	if (dlg)
		return dlg->DlgProc(hDlg, uMsg, wParam, lParam);
	else
		return FALSE;
}

int Dialog::Create(int nResource)
{
	if (IsValid())
		return -1;

	m_hDlg = CreateDialogParam(GetModuleHandle(NULL), MAKEINTRESOURCE(nResource),
		NULL, (DLGPROC)Dialog_DlgProcStub, (LPARAM)this);

	if (!m_hDlg)
		return -1;
	return 0;
}

int Dialog::Destroy()
{
	if (!IsValid())
		return -1;

	DestroyWindow(m_hDlg);
	m_hDlg = NULL;

	return 0;
}

int Dialog::Show(bool bShow)
{
	if (!IsValid())
		return -1;
	
	ShowWindow(m_hDlg, bShow ? SW_SHOW : SW_HIDE);
	return 0;
}

bool Dialog::IsValid()
{
	if (m_hDlg)
		return true;
	else
		return false;
}

int Dialog::SetFocus()
{
	if (!IsValid())
		return -1;

	SetForegroundWindow(m_hDlg);
	return 0;
}

HWND Dialog::GetHwnd()
{
	return m_hDlg;
}

LRESULT Dialog::DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_INITDIALOG:
			{
				OnInitDialog();
			}
			return TRUE;

		case WM_CLOSE:
			OnClose();
			break;

		case WM_COMMAND:
			OnCommand(LOWORD(wParam));
			break;

		default:
			break;
	}

	return FALSE;
}

int Dialog::OnInitDialog()
{
	
	return 0;
}

int Dialog::OnClose()
{
	Destroy();
	return 0;
}

int Dialog::OnCommand(int nItemId)
{
	if (nItemId == IDOK || nItemId == IDCANCEL)
		SendMessage(m_hDlg, WM_CLOSE, 0, 0);
	return 0;
}