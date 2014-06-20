#pragma once

// CHostSettingDialog dialog

class CHostSettingDialog : public CDialog
{
	DECLARE_DYNAMIC(CHostSettingDialog)

public:
	CString m_strAddress;
	CString m_strPort;
	
	struct HostInfo
	{
		ULONG m_nAddress;
		UINT32 m_nPort;
	};
	
	CHostSettingDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CHostSettingDialog();
	const HostInfo& ConvertToHostInfo();

// Dialog Data
	enum { IDD = IDD_DIALOG_HOST_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	bool CheckValueValid();
	
	HostInfo m_HostInfo;
	virtual void OnOK();
};
