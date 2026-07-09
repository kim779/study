// SetBufferDLg.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "SetBufferDLg.h"
#include "../h/axisvar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetBufferDlg dialog


CSetBufferDlg::CSetBufferDlg(CString strHome, int iIdle, int iGridDrawing, CWnd* pParent /*=NULL*/)
	: CDialog(CSetBufferDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetBufferDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_iIdle = iIdle;
	m_iGridDrawing = iGridDrawing;

	m_strPath.Format("%s\\%s\\cast_default.ini", strHome, TABDIR);


//	m_iIdleDefault = (int)GetPrivateProfileInt("SETUP", "IDLE", 0, m_strPath);
//	m_iGridDrawingDefault= (int)GetPrivateProfileInt("SETUP", "GridDrawing", 0, m_strPath);


}


void CSetBufferDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetBufferDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetBufferDlg, CDialog)
	//{{AFX_MSG_MAP(CSetBufferDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetBufferDlg message handlers



BOOL CSetBufferDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	char aacData[][16] = { "5", "10", "15", "20", "25", "30", "디폴트", "사용안함"};
	int iCount = sizeof(aacData) / 16;

	int iSelIdle = 0;
	int iSelDrawGrid = 0;
	for (int ii = 0; ii < iCount; ii++)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_SISE))->AddString(aacData[ii]);
		((CComboBox*)GetDlgItem(IDC_COMBO_GRIDDRAW))->AddString(aacData[ii]);

		if (m_iIdle == atoi(aacData[ii]))
			iSelIdle = ii;

		if (m_iGridDrawing == atoi(aacData[ii]))
			iSelDrawGrid = ii;
	}

	if (m_iIdle == -1)
		iSelIdle = 6;
	else if (m_iIdle == 0)
		iSelIdle = 7;

	if (m_iGridDrawing == -1)
		iSelDrawGrid = 6;
	else if (m_iGridDrawing == 0)
		iSelDrawGrid = 7;


	((CComboBox*)GetDlgItem(IDC_COMBO_SISE))->SetCurSel(iSelIdle);
	((CComboBox*)GetDlgItem(IDC_COMBO_GRIDDRAW))->SetCurSel(iSelDrawGrid);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CSetBufferDlg::OnOK() 
{
	// TODO: Add extra validation here

	CString strTemp;
	int iSel = ((CComboBox*)GetDlgItem(IDC_COMBO_SISE))->GetCurSel();
	
	((CComboBox*)GetDlgItem(IDC_COMBO_SISE))->GetLBText(iSel, strTemp);

	if (strTemp == "디폴트")
		m_iIdle = -1;
	else if (strTemp == "사용안함")
		m_iIdle = 0;
	else
		m_iIdle = atoi(strTemp);


	iSel = ((CComboBox*)GetDlgItem(IDC_COMBO_GRIDDRAW))->GetCurSel();
	
	((CComboBox*)GetDlgItem(IDC_COMBO_GRIDDRAW))->GetLBText(iSel, strTemp);
	if (strTemp == "디폴트")
		m_iGridDrawing = -1;
	else if (strTemp == "사용안함")
		m_iGridDrawing = 0;
	else
		m_iGridDrawing = atoi(strTemp);


//	strTemp.Format("%d", m_iSiseBuffer);
//	WritePrivateProfileString("BUFFER", "Sise", strTemp, m_strPath);

//	strTemp.Format("%d", m_iGridDrawing);
//	WritePrivateProfileString("BUFFER", "DrawGrid", strTemp, m_strPath);

	CDialog::OnOK();
}