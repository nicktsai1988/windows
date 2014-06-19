// Dialog.h
//

#ifndef DIALOG_H
#define DIALOG_H

class Dialog
{
	public:
		Dialog();
		~Dialog();

        virtual int Create(int nResource);

		int Destroy();

		int Show(bool bShow=true);

		bool IsValid();
		int SetFocus();

		HWND GetHwnd();

		LRESULT DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:

		virtual int OnInitDialog();
		virtual int OnClose();
		virtual int OnCommand(int nItemId);

	protected:
		HWND m_hDlg;
};

#endif