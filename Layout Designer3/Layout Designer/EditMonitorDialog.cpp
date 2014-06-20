// EditMonitorDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Layout Designer.h"
#include "EditMonitorDialog.h"
#include "afxdialogex.h"


// CEditMonitorDialog dialog

IMPLEMENT_DYNAMIC(CEditMonitorDialog, CDialog)

CEditMonitorDialog::CEditMonitorDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CEditMonitorDialog::IDD, pParent)
{
	
}

CEditMonitorDialog::~CEditMonitorDialog()
{
}

void CEditMonitorDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX,IDC_STATIC_TYPE,m_strType);
	DDX_Text(pDX,IDC_STATIC_ID,m_strID);
	DDX_Text(pDX,IDC_EDIT_POSX,m_strPositionX);
	DDX_Text(pDX,IDC_EDIT_POSY,m_strPositionY);
	DDX_Text(pDX,IDC_EDIT_ANGLE,m_strAngle);
}


BEGIN_MESSAGE_MAP(CEditMonitorDialog, CDialog)

END_MESSAGE_MAP()


// CEditMonitorDialog message handlers


BOOL CEditMonitorDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CEditMonitorDialog::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(TRUE);
	if(CheckValueValid())
	{
		CDialog::OnOK();
	}
	else
	{
		AfxMessageBox(_T("Input monitor info error!"));
	}
}

void CEditMonitorDialog::SetElementValue(const MonitorInfo* info)
{
	m_strType=info->m_type.m_strDescription;
	m_strID.Format(_T("%d"),info->m_nID);
	m_strPositionX.Format(_T("%d"),info->m_posNew.m_Center.x);
	m_strPositionY.Format(_T("%d"),info->m_posNew.m_Center.y);
	m_strAngle.Format(_T("%f"),info->m_posNew.m_dRotation);
}

const CEditMonitorDialog::NewPos& CEditMonitorDialog::GetPositionValue()
{
	return m_PosValue;
}

bool CEditMonitorDialog::CheckValueValid()
{
	if((!m_strPositionX.IsEmpty())
		&&(!m_strPositionY.IsEmpty())
		&&(!m_strAngle.IsEmpty()))
	{
			m_PosValue.x=_ttoi(m_strPositionX);
			m_PosValue.y=_ttoi(m_strPositionY);
			m_PosValue.angle=_ttof(m_strAngle);
			return true;
	}
	else
		return false;
}