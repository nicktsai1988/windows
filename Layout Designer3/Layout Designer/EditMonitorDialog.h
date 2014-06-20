#pragma once
#include "Utils.h"

// CEditMonitorDialog dialog



class CEditMonitorDialog : public CDialog
{
	DECLARE_DYNAMIC(CEditMonitorDialog)

public:
	// Dialog Data
	enum { IDD = IDD_DIALOG_EDIT_MONITOR };
	struct NewPos
	{
		int x,y;
		double angle;
	};
	CEditMonitorDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEditMonitorDialog();
	void SetElementValue(const MonitorInfo* info);
	const NewPos& GetPositionValue();
	

	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	bool CheckValueValid();
	CString m_strType;
	CString m_strID;
	CString m_strPositionX;
	CString m_strPositionY;
	CString m_strAngle;
	NewPos m_PosValue;
	//const MonitorInfo* m_pMonitorInfo;
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};
