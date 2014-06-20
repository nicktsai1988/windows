#pragma once

#include "Utils.h"
#include "resource.h"


class CMonitorTypeDialog : public CDialog
{
	DECLARE_DYNAMIC(CMonitorTypeDialog)

public:
	CMonitorTypeDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMonitorTypeDialog();
	void SetElementValue(const MonitorType& type);
	void SetWindowTitle(const CString& title);
	const MonitorType& GetMonitorType();
	

// Dialog Data
	enum { IDD = IDD_DIALOG_ADD_TYPE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	bool CheckValueValid();
	MonitorType m_MonitorType;
	CString m_strWindowTitle;

	CString m_strDescription;
	CString m_strHorizon,m_strVertical;
	CString m_strWidth,m_strHeight;
	CString m_strLeft,m_strRight,m_strTop,m_strBottom;
	COLORREF m_nColor;
	virtual void OnOK();
	CMFCColorButton m_MFCColorButton;
public:
	virtual BOOL OnInitDialog();
};
