// CToolCfg.cpp : implementation file
//

#include "stdafx.h"
#include "axisgdlg.h"
#include "ToolCfg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




/////////////////////////////////////////////////////////////////////////////
// CToolCfg dialog


CToolCfg::CToolCfg(CWnd* pMain, void* pvToolInfo, CWnd* pParent)
	: CDialog(CToolCfg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CToolCfg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pMain = pMain;
	m_pToolInfo = (_toolInfo*)pvToolInfo;

}


void CToolCfg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CToolCfg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CToolCfg, CDialog)
	//{{AFX_MSG_MAP(CToolCfg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolCfg message handlers

BOOL CToolCfg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_colorCB.SubclassDlgItem(IDC_COLOR, this);
	m_colorCB.SetColor(m_pToolInfo->rgbLine);

	m_styleCB.SubclassDlgItem(IDC_STYLE, this);
	m_styleCB.SetLine(m_pToolInfo->btLineStyle);

	m_widthCB.SubclassDlgItem(IDC_WIDTH, this);
	m_widthCB.SetLineWidth(m_pToolInfo->btLineWidth);

	// 왼쪽 연장
	if (m_pToolInfo->dwOption2 & TO_START_EXT)
	{
		if (m_pToolInfo->dwOption & TO_START_EXT)
			((CButton*)GetDlgItem(IDC_CHECK_START_EXT))->SetCheck(true);
	}
	else
		GetDlgItem(IDC_CHECK_START_EXT)->EnableWindow(false);

	// 오른쪽 연장
	if (m_pToolInfo->dwOption2 & TO_END_EXT)
	{
		if (m_pToolInfo->dwOption & TO_END_EXT)
			((CButton*)GetDlgItem(IDC_CHECK_END_EXT))->SetCheck(true);
	}
	else
		GetDlgItem(IDC_CHECK_END_EXT)->EnableWindow(false);

	// 연장 이름 변경
	if ((m_pToolInfo->dwOption2 & TO_START_EXT) && !(m_pToolInfo->dwOption2 & TO_END_EXT))
		GetDlgItem(IDC_CHECK_START_EXT)->SetWindowText("연장");
		

	if (m_pToolInfo->dwOption2 & TO_SHOW_ANGLE)
	{
		if (m_pToolInfo->dwOption & TO_SHOW_ANGLE)
			((CButton*)GetDlgItem(IDC_CHECK_SHOW_ANGLE))->SetCheck(true);
	}
	else
		GetDlgItem(IDC_CHECK_SHOW_ANGLE)->EnableWindow(false);

	if (m_pToolInfo->dwOption2 & TO_SHOW_POSI)
	{
		if (m_pToolInfo->dwOption & TO_SHOW_POSI)
			((CButton*)GetDlgItem(IDC_CHECK_SHOW_POSI))->SetCheck(true);
	}
	else
		GetDlgItem(IDC_CHECK_SHOW_POSI)->EnableWindow(false);

	if (m_pToolInfo->dwOption2 & TO_FILL)
	{
		if (m_pToolInfo->dwOption & TO_FILL)
			((CButton*)GetDlgItem(IDC_CHECK_FILL))->SetCheck(true);
	}
	else
		GetDlgItem(IDC_CHECK_FILL)->EnableWindow(false);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CToolCfg::OnOK() 
{
	m_pToolInfo->rgbLine = m_colorCB.GetColor();
	m_pToolInfo->btLineStyle = m_styleCB.GetLine();
	m_pToolInfo->btLineWidth = m_widthCB.GetLineWidth();
	m_pToolInfo->dwOption =	(((CButton*)GetDlgItem(IDC_CHECK_START_EXT))->GetCheck() ? TO_START_EXT : 0)
				| (((CButton*)GetDlgItem(IDC_CHECK_END_EXT))->GetCheck() ? TO_END_EXT : 0)
				| (((CButton*)GetDlgItem(IDC_CHECK_SHOW_ANGLE))->GetCheck() ? TO_SHOW_ANGLE : 0)
				| (((CButton*)GetDlgItem(IDC_CHECK_SHOW_POSI))->GetCheck() ? TO_SHOW_POSI : 0)
				| (((CButton*)GetDlgItem(IDC_CHECK_FILL))->GetCheck() ? TO_FILL : 0);
	
	CDialog::OnOK();
}
