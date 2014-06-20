// HostSetting.cpp : implementation file
//

#include "stdafx.h"
#include "Utils.h"
#include <afxsock.h>
#include "Layout Designer.h"
#include "HostSettingDialog.h"
#include "afxdialogex.h"



// CHostSettingDialog dialog

IMPLEMENT_DYNAMIC(CHostSettingDialog, CDialog)

CHostSettingDialog::CHostSettingDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CHostSettingDialog::IDD, pParent)
{
	//m_strPort=_T("9888");
}

CHostSettingDialog::~CHostSettingDialog()
{
}

void CHostSettingDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX,IDC_EDIT_ADDRESS,m_strAddress);
	DDX_Text(pDX,IDC_EDIT_PORT,m_strPort);
}


BEGIN_MESSAGE_MAP(CHostSettingDialog, CDialog)
END_MESSAGE_MAP()


// CHostSettingDialog message handlers

const CHostSettingDialog::HostInfo& CHostSettingDialog::ConvertToHostInfo()
{
	return m_HostInfo;
}

bool CHostSettingDialog::CheckValueValid()
{
	if(!m_strAddress.IsEmpty()
		&&(!m_strPort.IsEmpty()))
	{
		std::string strAddress=CString2StdString(m_strAddress);
		ULONG address=inet_addr(strAddress.c_str());
		if(address == INADDR_NONE)
		{
			return false;
		}
		UINT32 port=_ttoi(m_strPort);
		if(address && port)
		{
			m_HostInfo.m_nAddress=address;//network byte order
			m_HostInfo.m_nPort=htons(port&0xFFFF);//change to network byte order
			return true;
		}
		else
		{
			return false;
		}
	}
	else
		return false;
	return true;
}

void CHostSettingDialog::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class


	UpdateData(TRUE);
	if(CheckValueValid())
	{
		CDialog::OnOK();
	}
	else
	{
		AfxMessageBox(_T("Input host info error!"));
	}
}
