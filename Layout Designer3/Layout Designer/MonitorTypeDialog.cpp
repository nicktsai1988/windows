// AddMonitorTypeDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Layout Designer.h"
#include "MonitorTypeDialog.h"
#include "afxdialogex.h"


// CMonitorTypeDialog dialog

IMPLEMENT_DYNAMIC(CMonitorTypeDialog, CDialog)

CMonitorTypeDialog::CMonitorTypeDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CMonitorTypeDialog::IDD, pParent)
{
	m_strWindowTitle=_T("Add Monitor Type");

	m_nColor=-1;
}

CMonitorTypeDialog::~CMonitorTypeDialog()
{
}

void CMonitorTypeDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX,IDC_EDIT_DESCRIPTION,m_strDescription);
	DDX_Text(pDX,IDC_EDIT_HORIZON,m_strHorizon);
	DDX_Text(pDX,IDC_EDIT_VERTICAL,m_strVertical);
	DDX_Text(pDX,IDC_EDIT_WIDTH,m_strWidth);
	DDX_Text(pDX,IDC_EDIT_HEIGHT,m_strHeight);
	DDX_Text(pDX,IDC_EDIT_LEFT,m_strLeft);
	DDX_Text(pDX,IDC_EDIT_RIGHT,m_strRight);
	DDX_Text(pDX,IDC_EDIT_TOP,m_strTop);
	DDX_Text(pDX,IDC_EDIT_BOTTOM,m_strBottom);
	DDX_Control(pDX,IDC_MFCCOLORBUTTON,m_MFCColorButton);
}



BEGIN_MESSAGE_MAP(CMonitorTypeDialog, CDialog)
END_MESSAGE_MAP()

void CMonitorTypeDialog::SetElementValue(const MonitorType& type)
{
	m_strDescription=type.m_strDescription;
	m_strHorizon.Format(_T("%d"),type.m_nHorizonResolution);
	m_strVertical.Format(_T("%d"),type.m_nVerticalResolution);
	m_strWidth.Format(_T("%d"),type.m_nWidth);
	m_strHeight.Format(_T("%d"),type.m_nHeight);
	m_strLeft.Format(_T("%d"),type.m_nBorderLeft);
	m_strRight.Format(_T("%d"),type.m_nBorderRight);
	m_strTop.Format(_T("%d"),type.m_nBorderTop);
	m_strBottom.Format(_T("%d"),type.m_nBorderBottom);
	m_nColor=type.m_nBkColor;
}

const MonitorType& CMonitorTypeDialog::GetMonitorType()
{
	return m_MonitorType;
}


// CMonitorTypeDialog message handlers
 bool CMonitorTypeDialog::CheckValueValid()
 {
	 if((!m_strDescription.IsEmpty())
		 && (!m_strHorizon.IsEmpty())
		 && (!m_strVertical.IsEmpty())
		 && (!m_strWidth.IsEmpty())
		 && (!m_strHeight.IsEmpty())
		 && (!m_strLeft.IsEmpty())
		 && (!m_strRight.IsEmpty())
		 && (!m_strTop.IsEmpty())
		 && (!m_strBottom.IsEmpty()))
	 {
		int horizon=_ttoi(m_strHorizon);
		int vertical=_ttoi(m_strVertical);
		int width=_ttoi(m_strWidth);
		int height=_ttoi(m_strHeight);
		int left=_ttoi(m_strLeft);
		int right=_ttoi(m_strRight);
		int top=_ttoi(m_strTop);
		int bottom=_ttoi(m_strBottom);

		if(horizon
			&& vertical
			&& width
			&& height
			&& left
			&& right
			&& top
			&& bottom)
		{
			m_MonitorType.m_strDescription=m_strDescription;
			m_MonitorType.m_nHorizonResolution=horizon;
			m_MonitorType.m_nVerticalResolution=vertical;
			m_MonitorType.m_nWidth=width;
			m_MonitorType.m_nHeight=height;
			m_MonitorType.m_nBorderLeft=left;
			m_MonitorType.m_nBorderRight=right;
			m_MonitorType.m_nBorderTop=top;
			m_MonitorType.m_nBorderBottom=bottom;
			m_nColor=m_MFCColorButton.GetColor();
			if(m_nColor == -1)
			{
				m_nColor = m_MFCColorButton.GetAutomaticColor();
			}
			m_MonitorType.m_nBkColor=m_nColor;
			return true;
		}
		else
		{
			return false;
		}
	 }
	 else
	 {
		 return false;
	 }
	 return true;
 }

 void CMonitorTypeDialog::OnOK()
 {
	 // TODO: Add your specialized code here and/or call the base class
	 UpdateData(TRUE);
	 if(CheckValueValid())
	 {
		CDialog::OnOK();
	 }
	 else
	 {
		 AfxMessageBox(_T("Input type info error!"));
	 }
 }


 BOOL CMonitorTypeDialog::OnInitDialog()
 {
	 CDialog::OnInitDialog();

	 // TODO:  Add extra initialization here
	 SetWindowText(m_strWindowTitle);
	 m_MFCColorButton.EnableAutomaticButton(_T("default"),RGB(166,202,240));
	 m_MFCColorButton.SetColor(m_nColor);

	 return TRUE;  // return TRUE unless you set the focus to a control
	 // EXCEPTION: OCX Property Pages should return FALSE
 }

 void CMonitorTypeDialog::SetWindowTitle(const CString& title)
 {
	 m_strWindowTitle=title;
 }
